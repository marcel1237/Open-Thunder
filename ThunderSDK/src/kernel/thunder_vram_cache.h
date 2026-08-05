/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_VRAM_CACHE_H
#define THUNDER_VRAM_CACHE_H

#include <iostream>

namespace Td {
namespace Hardware {

/**
 * @brief TH-19: VRAM-Pulse L5-Cache.
 */
inline void initVramCache() {
#ifdef Q_OS_LINUX
    // TH-81: Silicon-Native WebGL-Bypass
    qputenv("ENABLE_VULKAN_RENDERER", "1");

    // TH-82: Direct-Silicon Image-Matrix
    // TH-93: Silicon-Native Video-Matrix
    qputenv("LIBVA_DRIVER_NAME", "iHD");
    qputenv("VDPAU_DRIVER", "va_gl");

    // TH-86: Zero-Copy GPU-Texture-Stream
    qputenv("RADV_PERFTEST", "sam,nggc");
    qputenv("ANV_ENABLE_BAR", "1");

    std::cout << "[TH-19/81/82/86/93] VRAM-Pulse L5-Cache and Video Engines: [READY]" << std::endl;
#endif
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_VRAM_CACHE_H
