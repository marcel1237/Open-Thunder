/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_HUGE_TLB_H
#define THUNDER_HUGE_TLB_H

#include <sys/mman.h>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

namespace Td {
namespace Hardware {

/**
 * @brief TH-22: HugePage-Prefaulting Matrix.
 * Wams up memory pages to eliminate first-touch latency.
 */
inline void prefaultMemory(void* ptr, size_t size) {
    uint8_t* p = static_cast<uint8_t*>(ptr);
    // Touch every 4KB (standard page) or 2MB (huge page)
    // to force the Kernel to map the physical page immediately.
    for (size_t i = 0; i < size; i += 4096) {
        p[i] = 0;
    }

    // TH-26: KSM-Silence Engine
    // Disable Kernel Samepage Merging for this block to avoid scan overhead.
    madvise(ptr, size, 0xF /* MADV_UNMERGEABLE */);
}

/**
 * @brief Allocates hardware-pinned contiguous memory with dynamic fallback.
 * Includes TH-22 Page Prefaulting.
 */
inline void* allocHugeMemory(size_t size) {
    if (size == 0 || size > SIZE_MAX - 0x1FFFFF) return MAP_FAILED;
    size_t aligned_size = (size + 0x1FFFFF) & ~0x1FFFFF;

    // Attempt 1: Explicit HugeTLB
    void* ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_LOCKED, -1, 0);

    if (ptr != MAP_FAILED) {
        prefaultMemory(ptr, aligned_size);
        return ptr;
    }

    // Attempt 2: Standard with RAM locking
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, -1, 0);

    if (ptr != MAP_FAILED) {
        madvise(ptr, aligned_size, 0xE /* MADV_HUGEPAGE */);
        prefaultMemory(ptr, aligned_size);
        return ptr;
    }

    // Attempt 3: Standard fallback
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr != MAP_FAILED) {
        madvise(ptr, aligned_size, 0xE /* MADV_HUGEPAGE */);
        prefaultMemory(ptr, aligned_size);
        return ptr;
    }

    return MAP_FAILED;
}

inline bool freeHugeMemory(void* ptr, size_t size) {
    if (!ptr || ptr == MAP_FAILED || size == 0 || size > SIZE_MAX - 0x1FFFFF)
        return false;
    const size_t alignedSize = (size + 0x1FFFFF) & ~static_cast<size_t>(0x1FFFFF);
    return munmap(ptr, alignedSize) == 0;
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_HUGE_TLB_H
