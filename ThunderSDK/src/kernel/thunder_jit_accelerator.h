/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_JIT_ACCELERATOR_H
#define THUNDER_JIT_ACCELERATOR_H

#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "thunder_huge_tlb.h"

namespace Td {
namespace Hardware {

/**
 * @brief TH-20: Silicon-Native JIT.
 * Provides a high-performance memory space for dynamic machine code execution.
 * Uses a W^X lifecycle: writable while emitting, executable after sealing.
 */
class SiliconNativeJIT {
public:
    static SiliconNativeJIT* instance() {
        static SiliconNativeJIT inst;
        return &inst;
    }

    /**
     * @brief Allocates an executable page in the hardware matrix.
     */
    void* allocateWritableMemory(size_t size) {
        if (size == 0 || size > SIZE_MAX - 0xFFF) return nullptr;
        size_t aligned_size = (size + 0xFFF) & ~0xFFF;

        // Allocate via mmap with PROT_EXEC
        void* ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, -1, 0);

        if (ptr == MAP_FAILED) {
            // Fallback without locking
            ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        }

        if (ptr == MAP_FAILED) {
            return nullptr;
        }

        return ptr;
    }

    /**
     * @brief "Burns" optimized machine code into the hardware execution unit.
     */
    bool emitAndSeal(void* target, size_t capacity, const unsigned char* code, size_t size) {
        if (!target || !code || size == 0 || size > capacity) return false;
        std::memcpy(target, code, size);

        // Synchronize Instruction Cache (ICache)
        // Mathematically ensures the CPU doesn't execute stale instructions
        __builtin___clear_cache((char*)target, (char*)target + size);

        const size_t alignedSize = (capacity + 0xFFF) & ~static_cast<size_t>(0xFFF);
        return mprotect(target, alignedSize, PROT_READ | PROT_EXEC) == 0;
    }

    bool release(void* target, size_t capacity) {
        if (!target || capacity == 0 || capacity > SIZE_MAX - 0xFFF) return false;
        const size_t alignedSize = (capacity + 0xFFF) & ~static_cast<size_t>(0xFFF);
        return munmap(target, alignedSize) == 0;
    }

private:
    SiliconNativeJIT() = default;
};

/**
 * @brief Global initialization for TH-20.
 */
inline void initNativeJit() {
    SiliconNativeJIT::instance();
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_JIT_ACCELERATOR_H
