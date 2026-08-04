/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef THUNDER_PREFETCH_H
#define THUNDER_PREFETCH_H

#include <cstddef>

namespace Td {
namespace Hardware {

/**
 * @brief Prefetches a memory block into the CPU Cache (L1/L2/L3).
 * Mathematically hides memory latency (T_mem) by overlapping it
 * with CPU processing (T_cpu).
 *
 * RW: 0 = Read, 1 = Write
 * Locality: 0 = None, 1 = L3, 2 = L2, 3 = L1
 */
template <int RW = 0, int Locality = 3>
inline void prefetch(const void* addr) {
    __builtin_prefetch(addr, RW, Locality);
}

/**
 * @brief Prefetches a range of memory.
 * Cache line size is usually 0x40 (64 bytes).
 */
inline void prefetchRange(const void* addr, size_t len) {
    const char* p = static_cast<const char*>(addr);
    for (size_t i = 0; i < len; i += 0x40) {
        prefetch(p + i);
    }
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_PREFETCH_H
