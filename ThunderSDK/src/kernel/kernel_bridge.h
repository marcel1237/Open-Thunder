/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef KERNEL_BRIDGE_H
#define KERNEL_BRIDGE_H

#include <QString>
#include <cstdint>
#include <QStringList>
#include "thundercommon.h"

namespace Td {
namespace Kernel {

struct THUNDER_EXPORT OptimizationReport {
    bool cpuAffinity = false;
    bool memoryLocked = false;
    bool timerSlack = false;
    bool realtimeScheduling = false;
    bool dmaLatency = false;
    bool addressLimit = false;
    int successfulGovernors = 0;
    QStringList errors;

    bool anyApplied() const { return cpuAffinity || memoryLocked || timerSlack ||
        realtimeScheduling || dmaLatency || addressLimit || successfulGovernors > 0; }
};

OptimizationReport THUNDER_EXPORT optimizeProcessWithReport();
void THUNDER_EXPORT optimizeProcess();
QString THUNDER_EXPORT initRamStorage();
void THUNDER_EXPORT verifyHardwareHandshake();
void THUNDER_EXPORT enableSpeculationSpeed();

inline bool isPageAligned(void* ptr) {
    return !(reinterpret_cast<std::uintptr_t>(ptr) & 0xFFF);
}

} // namespace Kernel
} // namespace Td

#endif // KERNEL_BRIDGE_H
