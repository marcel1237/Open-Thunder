/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef THUNDERSDK_H
#define THUNDERSDK_H

#include "app/thundercommon.h"
#include "kernel/kernel_bridge.h"
#include "kernel/thunder_aes_accelerator.h"
#include "kernel/thunder_cache_optimizer.h"
#include "kernel/thunder_prefetch.h"
#include "kernel/thunder_branch_optimizer.h"
#include "kernel/thunder_huge_tlb.h"
#include "kernel/thunder_simd_accelerator.h"

namespace Td {

/**
 * @brief Initializes the entire Thunder Hardware Matrix for any application.
 *
 * Calling this function will:
 * 1. Pining process to specific CPU Cores.
 * 2. Lock memory to RAM (Resident mode).
 * 3. Set Real-Time scheduling priorities.
 * 4. Optimize Kernel IRQ affinity.
 * 5. Enable Spectre/Meltdown bypass for speed.
 */
inline void initializeHardwareAcceleration() {
    // TH-01: NitroCore Kernel-Warp
    Td::Kernel::optimizeProcess();

    // TH-10: Spectre-Speed Bypass
    Td::Kernel::enableSpeculationSpeed();

    // Diagnostic Handshake
    Td::Kernel::verifyHardwareHandshake();
}

} // namespace Td

#endif // THUNDERSDK_H
