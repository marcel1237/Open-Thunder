/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef THUNDER_HUGE_TLB_H
#define THUNDER_HUGE_TLB_H

#include <sys/mman.h>
#include <cstddef>
#include <cstdint>
#include <iostream>

namespace Td {
namespace Hardware {

/**
 * @brief Allocates hardware-pinned contiguous memory with dynamic fallback.
 * Automatically handles systems without HugePages or locked memory limits.
 */
inline void* allocHugeMemory(size_t size) {
    size_t aligned_size = (size + 0x1FFFFF) & ~0x1FFFFF;

    // Attempt 1: Explicit HugeTLB (Maximum Performance)
    void* ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_LOCKED, -1, 0);

    if (ptr != MAP_FAILED) {
        return ptr;
    }

    // Attempt 2: Standard allocation with RAM locking (if allowed by ulimit)
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, -1, 0);

    if (ptr != MAP_FAILED) {
        // Instruct the Kernel to attempt promoting this area to HugePages dynamically (Transparent Huge Pages)
        madvise(ptr, aligned_size, 0xE /* MADV_HUGEPAGE */);
        return ptr;
    }

    // Attempt 3: Standard allocation without locking (OS-Managed fallback)
    ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (ptr != MAP_FAILED) {
        madvise(ptr, aligned_size, 0xE /* MADV_HUGEPAGE */);
        return ptr;
    }

    return MAP_FAILED;
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_HUGE_TLB_H
