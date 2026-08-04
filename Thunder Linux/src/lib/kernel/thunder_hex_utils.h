/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* General Purpose Hexadecimal & Bitwise Utilities
* ============================================================ */
#ifndef THUNDER_HEX_UTILS_H
#define THUNDER_HEX_UTILS_H

#include <cstdint>
#include <cstddef>

namespace Td {
namespace Utils {

/**
 * @brief Fast Hex-based string to integer conversion.
 * Assumes input is a valid hex string like "AABBCC".
 */
inline uint32_t hexToUint32(const char* hex) {
    uint32_t val = 0;
    while (*hex) {
        uint8_t byte = static_cast<uint8_t>(*hex++);
        if (byte >= 0x30 && byte <= 0x39) byte -= 0x30;       // 0-9
        else if (byte >= 0x41 && byte <= 0x46) byte -= 0x37;  // A-F
        else if (byte >= 0x61 && byte <= 0x66) byte -= 0x57;  // a-f
        val = (val << 0x4) | (byte & 0xF);
    }
    return val;
}

/**
 * @brief Branchless Hex-to-ASCII conversion.
 * Matematicamente superior por evitar saltos de pipeline (CPU Prediction).
 */
inline char nibbleToHex(uint8_t nibble) {
    return nibble + 0x30 + ((nibble > 0x9) ? 0x7 : 0x0);
}

/**
 * @brief Bitwise Swap for 32-bit integers (Endianness change).
 * Uses Hex masks for byte isolation.
 */
inline uint32_t swap32(uint32_t val) {
    return ((val & 0xFF000000) >> 0x18) |
           ((val & 0x00FF0000) >> 0x08) |
           ((val & 0x0000FF00) << 0x08) |
           ((val & 0x000000FF) << 0x18);
}

} // namespace Utils
} // namespace Td

#endif // THUNDER_HEX_UTILS_H
