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
    nibble &= 0x0F;
    return static_cast<char>(nibble + '0' + (nibble > 9 ? 7 : 0));
}

/**
 * @brief Fast Hex-based string to integer conversion.
 */
inline bool hexToUint32(const char* hex, uint32_t& value) {
    if (!hex || !*hex) return false;
    uint32_t val = 0;
    while (*hex) {
        const char c = *hex++;
        uint8_t digit;
        if (c >= '0' && c <= '9') digit = static_cast<uint8_t>(c - '0');
        else if (c >= 'A' && c <= 'F') digit = static_cast<uint8_t>(c - 'A' + 10);
        else if (c >= 'a' && c <= 'f') digit = static_cast<uint8_t>(c - 'a' + 10);
        else return false;
        if (val > 0x0FFFFFFFU) return false;
        val = (val << 4U) | digit;
    }
    value = val;
    return true;
}

} // namespace Utils
} // namespace Td

#endif // THUNDER_HEX_UTILS_H
