/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* UI Constants using Hexadecimal for maximum precision and speed.
* ============================================================ */
#ifndef THUNDER_UI_CONSTANTS_H
#define THUNDER_UI_CONSTANTS_H

namespace Td {
namespace UI {

// Colors in Hex (ARGB format: 0xAARRGGBB)
constexpr uint32_t ColorBackground  = 0xFF1A1A1A; // Dark Grey
constexpr uint32_t ColorToolbar     = 0xFF2D2D2D; // Slightly lighter grey
constexpr uint32_t ColorAccent      = 0xFF007ACC; // Blue accent
constexpr uint32_t ColorText        = 0xFFE0E0E0; // Off-white
constexpr uint32_t ColorBorder      = 0xFF3E3E3E;

// Dimensions in Hex (Pixels)
constexpr int ToolbarHeight    = 0x28; // 40px
constexpr int ButtonSize       = 0x1C; // 28px
constexpr int AddressBarHeight = 0x18; // 24px
constexpr int PaddingSmall     = 0x04; // 4px
constexpr int PaddingMedium    = 0x08; // 8px

} // namespace UI
} // namespace Td

#endif // THUNDER_UI_CONSTANTS_H
