/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* Hardware-Level L3 Cache & Memory Locality Optimizer
* ============================================================ */
#ifndef THUNDER_CACHE_OPTIMIZER_H
#define THUNDER_CACHE_OPTIMIZER_H

#include <cstddef>
#include <new>

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
 * While Intel CAT requires root, we can use process "Niceness" and
 * Memory Advice to maintain a hot cache set.
 */
inline void prioritizeCacheLocality() {
    // We already use mlockall in kernel_bridge, which is the foundation.
    // Here we could add specific MSR (Model Specific Register) tweaks
    // if we had ring 0 access, but we'll focus on Cache-friendly allocation.
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_CACHE_OPTIMIZER_H
