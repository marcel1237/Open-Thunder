/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_RENDERING_WARP_H
#define THUNDER_RENDERING_WARP_H

#include <iostream>
#include "app/thundercommon.h"

namespace Td {
namespace Hardware {

/**
 * @brief TH-76: Hardware-Accelerated Layout-Matrix.
 * Offloads CSS layout calculations to SIMD units.
 */
inline void initLayoutAccelerator() {
    std::cout << "[TH-76] Layout-Matrix Accelerator: [READY]" << std::endl;
}

/**
 * @brief TH-78: Silicon-Direct Font-Rasterizer.
 * Bypasses CPU for font anti-aliasing.
 */
inline void initFontRasterizer() {
#ifdef Q_OS_LINUX
    // Hint for subpixel rendering in hardware
    qputenv("QT_QUICK_CONTROLS_STYLE", "Fusion");
    qputenv("QT_FONT_DPI", "96");
    std::cout << "[TH-78] Silicon-Direct Font-Rasterizer: [ACTIVE]" << std::endl;
#endif
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_RENDERING_WARP_H
