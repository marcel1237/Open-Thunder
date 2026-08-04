/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* Low-level Linux Kernel Integration Bridge
* ============================================================ */
#ifndef KERNEL_BRIDGE_H
#define KERNEL_BRIDGE_H

#include <QString>
#include "thundercommon.h"

namespace Td {
namespace Kernel {

/**
 * @brief Optimizes the process for the Linux Kernel.
 *
 * This includes:
 * - Setting SCHED_RR or SCHED_BATCH depending on state.
 * - Enabling Transparent Huge Pages (THP) via madvise.
 * - Setting I/O priority to high.
 */
void THUNDER_EXPORT optimizeProcess();

/**
 * @brief Initializes a Hardware-Level RAM Drive for Browser Data.
 * Uses /dev/shm (Shared Memory) to bypass physical disk I/O.
 * Matematicamente, elimina a latência de busca do SSD (0.1ms) para RAM (100ns).
 */
QString THUNDER_EXPORT initRamStorage();

/**
 * @brief Performs the Final Hardware Handshake.
 * Verifies all modules and technologies are active and synchronized.
 */
void THUNDER_EXPORT verifyHardwareHandshake();

/**
 * @brief Enables Hardware Speculation Speed (Bypassing certain mitigations).
 * Prioritizes O(1) execution over speculative security in the CPU pipeline.
 */
void THUNDER_EXPORT enableSpeculationSpeed();

/**
 * @brief Direct Hex-based Memory Alignment check.
 * Fastest way to verify if a pointer is aligned to a kernel page (4KB).
 */
inline bool isPageAligned(void* ptr) {
    return !(reinterpret_cast<uintptr_t>(ptr) & 0xFFF); // 0xFFF = 4095 (4KB - 1)
}

} // namespace Kernel
} // namespace Td

#endif // KERNEL_BRIDGE_H
