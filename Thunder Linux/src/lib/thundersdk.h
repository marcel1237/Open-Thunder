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
#include "kernel/thunder_io_matrix.h"

namespace Td {

/**
 * @brief Initializes the entire Thunder Hardware Matrix for any application.
 * Now expanded with TH-12 (IO-Uring).
 */
inline void initializeHardwareAcceleration() {
    // TH-01, TH-09, TH-10: Core, Power, Spectre
    Td::Kernel::optimizeProcess();

    // TH-12: IO-Uring Warp
    Td::IO::ThunderIORing::instance()->init();

    // TH-10: Spectre-Speed Bypass
    Td::Kernel::enableSpeculationSpeed();

    // Diagnostic Handshake
    Td::Kernel::verifyHardwareHandshake();
}

} // namespace Td

#endif // THUNDERSDK_H
