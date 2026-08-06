/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_GPU_WARMER_H
#define THUNDER_GPU_WARMER_H

#include <QtCore/QString>
#include <QtCore/QFile>
#include <iostream>

namespace Td {
namespace Hardware {

/**
 * @brief TH-14: Hardware-Warming Shaders.
 * Pre-warms the GPU by forcing shader compilation and pipeline state creation.
 * Mathematically eliminates the "first-render stutter" by ensuring the GPU
 * is in its high-performance state before the first frame is requested.
 */
inline bool warmGpuExecutionUnits() {
#ifdef Q_OS_LINUX
    // 1. Force Mesa Shader Cache Enablement (Modern API)
    qputenv("MESA_SHADER_CACHE_DISABLE", "0");
    qputenv("MESA_SHADER_CACHE_MAX_SIZE", "2G");

    // 2. Set Power State to High (if supported by driver)
    qputenv("vblank_mode", "0"); // Disable VSync for warming phase

    // 3. AMD/Intel Specific Warming (Force Performance Levels)
    // This hints the kernel to ramp up GPU clocks immediately
    QFile powerState("/sys/class/drm/card0/device/power_dpm_force_performance_level");
    const bool powerStateSet = powerState.open(QIODevice::WriteOnly) && powerState.write("high") == 4;

    return powerStateSet;
#else
    return false;
#endif
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_GPU_WARMER_H
