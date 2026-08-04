/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_UI_CONSTANTS_H
#define THUNDER_UI_CONSTANTS_H

#include <cstdint>

namespace Td {
namespace UI {

constexpr uint32_t ColorBackground  = 0xFF1A1A1A;
constexpr uint32_t ColorToolbar     = 0xFF2D2D2D;
constexpr uint32_t ColorAccent      = 0xFF007ACC;
constexpr uint32_t ColorText        = 0xFFE0E0E0;
constexpr uint32_t ColorBorder      = 0xFF3E3E3E;

constexpr int ToolbarHeight    = 0x28;
constexpr int ButtonSize       = 0x1C;
constexpr int AddressBarHeight = 0x18;
constexpr int PaddingSmall     = 0x04;
constexpr int PaddingMedium    = 0x08;

} // namespace UI
} // namespace Td

#endif // THUNDER_UI_CONSTANTS_H
