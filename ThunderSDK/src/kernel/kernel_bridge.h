/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef KERNEL_BRIDGE_H
#define KERNEL_BRIDGE_H

#include <QString>
#include <cstdint>
#include "thundercommon.h"

namespace Td {
namespace Kernel {

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
