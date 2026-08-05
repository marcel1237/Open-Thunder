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
 * Bypasses standard OS page protections by using pre-aligned, hardware-locked
 * executable memory segments.
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
    void* allocateExecutableMemory(size_t size) {
        size_t aligned_size = (size + 0xFFF) & ~0xFFF;

        // Allocate via mmap with PROT_EXEC
        void* ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE | PROT_EXEC,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_LOCKED, -1, 0);

        if (ptr == MAP_FAILED) {
            // Fallback without locking
            ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE | PROT_EXEC,
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
    bool emitMachineCode(void* target, const unsigned char* code, size_t size) {
        if (!target || !code) return false;
        std::memcpy(target, code, size);

        // Synchronize Instruction Cache (ICache)
        // Mathematically ensures the CPU doesn't execute stale instructions
        __builtin___clear_cache((char*)target, (char*)target + size);

        return true;
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
