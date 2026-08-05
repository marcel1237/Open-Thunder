/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_HEX_UTILS_H
#define THUNDER_HEX_UTILS_H

#include <cstdint>

namespace Td {
namespace Utils {

/**
 * @brief TH-29: Branchless Hex-to-ASCII conversion.
 * Pure bit-arithmetic to eliminate CPU stalls.
 */
inline char nibbleToHex(uint8_t nibble) {
    // Mathematically: c = n + '0' + (n > 9 ? 7 : 0)
    return nibble + 0x30 + (( (static_cast<int>(nibble) - 10) >> 31) & 0x0 ? 0x7 : 0x0);
}

/**
 * @brief Fast Hex-based string to integer conversion.
 */
inline uint32_t hexToUint32(const char* hex) {
    uint32_t val = 0;
    while (*hex) {
        uint8_t byte = static_cast<uint8_t>(*hex++);
        // Branchless subtraction
        uint8_t mask1 = (byte >= 0x41) & (byte <= 0x46); // A-F
        uint8_t mask2 = (byte >= 0x61) & (byte <= 0x66); // a-f
        byte -= (0x30 + (mask1 * 7) + (mask2 * 39));
        val = (val << 0x4) | (byte & 0xF);
    }
    return val;
}

} // namespace Utils
} // namespace Td

#endif // THUNDER_HEX_UTILS_H
