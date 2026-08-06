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

// Master Color Palette (Cyber-Scientist Theme)
constexpr uint32_t ColorBackground  = 0xFF05070A;
constexpr uint32_t ColorToolbar     = 0xFF0D1117;
constexpr uint32_t ColorAccent      = 0xFF00F2FF;
constexpr uint32_t ColorSecondary   = 0xFFFF00EA; // Pink/Magenta for special highlights
constexpr uint32_t ColorText        = 0xFFE6EDF3;
constexpr uint32_t ColorTextMuted   = 0xFF8B949E;
constexpr uint32_t ColorBorder      = 0xFF30363D;

// Geometry (Modern Spacing)
constexpr int ToolbarHeight    = 54;
constexpr int ButtonSize       = 36;
constexpr int AddressBarHeight = 36;
constexpr int PaddingSmall     = 6;
constexpr int PaddingMedium    = 12;
constexpr int BorderRadius     = 10;

} // namespace UI
} // namespace Td

#endif // THUNDER_UI_CONSTANTS_H
