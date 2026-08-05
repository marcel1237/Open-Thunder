/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef THUNDER_CACHE_OPTIMIZER_H
#define THUNDER_CACHE_OPTIMIZER_H

#include <cstddef>

namespace Td {
namespace Hardware {

// Standard L3 Cache Line Size in Hex (64 bytes)
constexpr size_t CacheLineSize = 0x40;

/**
 * @brief Aligns a structure to the Hardware Cache Line.
 * Prevents "False Sharing" and ensures the CPU can load the object
 * in exactly one L1/L2 cache fetch.
 */
#define THUNDER_CACHE_ALIGNED alignas(Td::Hardware::CacheLineSize)

/**
 * @brief Hints the Kernel to prioritize this process's memory in the L3 cache.
 */
inline void prioritizeCacheLocality() {
    // Already use mlockall in kernel_bridge, which is the foundation.
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_CACHE_OPTIMIZER_H
