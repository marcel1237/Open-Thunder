/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDERSDK_H
#define THUNDERSDK_H

#include "app/thundercommon.h"
#include "kernel/kernel_bridge.h"
#include "kernel/thunder_huge_tlb.h"
#include "kernel/thunder_simd_accelerator.h"
#include "kernel/thunder_io_matrix.h"
#include "kernel/thunder_gpu_warmer.h"
#include "kernel/thunder_bios_sync.h"
#include "kernel/thunder_vram_cache.h"
#include "kernel/thunder_jit_accelerator.h"
#include "kernel/thunder_vdso_warp.h"
#include "kernel/thunder_lockless_matrix.h"
#include "kernel/thunder_cache_pool.h"
#include "kernel/thunder_ipc_warp.h"
#include "kernel/thunder_rendering_warp.h"
#include "kernel/thunder_advanced_cpu.h"
#include "kernel/thunder_security_warp.h"
#include "kernel/thunder_protection_matrix.h"
#include "kernel/thunder_dma_sync.h"
#include "network/thunder_network_latency.h"
#include "network/thunder_zero_copy.h"
#include "network/thunder_url_warp.h"
#include "network/thunder_advanced_network.h"

namespace Td {

/**
 * @brief MASTER INITIALIZER: TH-01 to TH-100.
 */
inline void initializeHardwareAcceleration() {
    // 🛡️ IP PROTECTION (TH-101)
    Td::Security::enforceIPProtection();

    // 🧬 CORE ENFORCEMENT
    Td::Kernel::optimizeProcess();
    Td::Kernel::enableSpeculationSpeed();

    // 🧠 MEMORY & CACHE
    Td::Hardware::CacheHotPool::instance()->startWarmingThread();
    Td::Hardware::initNativeJit();

    // ⚡ VECTORS & LOGIC
    Td::Hardware::AdvancedCPUAccelerator::instance()->initMathMatrix();

    // 🎮 GRAPHICS & VRAM
    Td::Hardware::warmGpuExecutionUnits();
    Td::Hardware::initVramCache();
    Td::Hardware::initLayoutAccelerator();
    Td::Hardware::initFontRasterizer();

    // 🌐 NETWORK & IO
    Td::IO::ThunderIORing::instance()->init();

    // 🔒 SYNC & SECURITY
    Td::Hardware::lockToHardwareClock();
    Td::Hardware::VDSOWarp::instance()->init();
    Td::Kernel::IPCWarp::instance();
    Td::Hardware::SecurityWarpMatrix::instance()->enforceCFI();

    // FINAL HANDSHAKE (TH-100)
    Td::Kernel::verifyHardwareHandshake();
}

} // namespace Td

#endif // THUNDERSDK_H
