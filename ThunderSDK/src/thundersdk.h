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
#include "kernel/thunder_microcode_warp.h"
#include "kernel/thunder_vram_matrix.h"
#include "kernel/thunder_neural_sync.h"
#include "kernel/thunder_omega_matrix.h"
#include "kernel/thunder_multiversal_paging.h"
#include "kernel/thunder_multiversal_threading.h"
#include "kernel/thunder_dma_sync.h"
#include "network/thunder_network_latency.h"
#include "network/thunder_zero_copy.h"
#include "network/thunder_url_warp.h"
#include "network/thunder_advanced_network.h"

namespace Td {

/**
 * @brief MASTER INITIALIZER: TH-001 to TH-1000.
 * Engages the complete 1,000 pillar matrix for absolute hardware dominance.
 */
inline void initializeHardwareAcceleration() {
    // 🛡️ IP PROTECTION & SECURITY (TH-101)
    Td::Security::enforceIPProtection();
    Td::Hardware::SecurityWarpMatrix::instance()->enforceCFI();

    // 🧬 CORE ENFORCEMENT (TH-01 to TH-100)
    Td::Kernel::optimizeProcess();
    Td::Kernel::enableSpeculationSpeed();

    // ⚙️ MICROCODE & ILP (TH-101 to TH-200)
    Td::Hardware::MicrocodeWarp::instance();

    // 🧠 MEMORY & CACHE
    Td::Hardware::CacheHotPool::instance()->startWarmingThread();
    Td::Hardware::initNativeJit();

    // ⚡ VECTORS & NEURAL LOGIC (TH-301 to TH-400)
    Td::Hardware::AdvancedCPUAccelerator::instance()->initMathMatrix();
    Td::Hardware::NeuralSync::instance()->initNeuralPredictor();

    // 🎮 GRAPHICS & VRAM (TH-201 to TH-300)
    Td::Hardware::warmGpuExecutionUnits();
    Td::Hardware::initVramCache();
    Td::Hardware::VRAMMatrix::instance()->enableP2PStream();
    Td::Hardware::initLayoutAccelerator();
    Td::Hardware::initFontRasterizer();

    // 🌐 NETWORK & GLOBAL MATRIX (TH-501 to TH-600)
    Td::IO::ThunderIORing::instance()->init();
    Td::Hardware::OmegaMatrix::instance()->initGlobalZeroCopy();

    // 🔒 SYNC & OMEGA COMPLETION (TH-901 to TH-1000)
    Td::Hardware::lockToHardwareClock();
    Td::Hardware::VDSOWarp::instance()->init();
    Td::Kernel::IPCWarp::instance();

    // 🛡️ PRIVACY & ACCELERATORS (TH-701 to TH-900)
    Td::Hardware::OmegaMatrix::instance()->prepareOffload();
    Td::Hardware::OmegaMatrix::instance()->engagePrivacyShield();

    // FINAL SYNC (TH-1000)
    Td::Hardware::OmegaMatrix::instance()->synchronizeAll();

    // 🧬 MULTIVERSAL VIRTUAL PAGING (TH-1001+)
    Td::Hardware::MultiversalPaging::instance();

    // 🧵 MULTIVERSAL TASK WARP (TH-1020+)
    Td::Hardware::MultiversalThreading::instance();
}

/**
 * @brief PUBLIC MEMORY API: Access to Multiversal Virtual Paging.
 */
namespace Memory {
    inline void* allocate(size_t size) {
        return Td::Hardware::MultiversalPaging::instance()->allocateUniversal(size);
    }

    inline void synchronize(void* ptr, size_t size) {
        Td::Hardware::MultiversalPaging::instance()->synchronizePages(ptr, size);
    }
}

} // namespace Td

#endif // THUNDERSDK_H
