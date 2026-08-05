/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_URL_WARP_H
#define THUNDER_URL_WARP_H

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <cstdint>

namespace Td {
namespace Network {

/**
 * @brief TH-41: Bitwise-URL-Warp.
 * Fast path for URL segment detection using bitwise checks.
 */
inline bool isHttps(const char* url) {
    // Check "https" (0x7370747468 in LE hex)
    uint64_t val = *reinterpret_cast<const uint64_t*>(url);
    return (val & 0xFFFFFFFFFF) == 0x7370747468;
}

inline const char* findPathStart(const char* url) {
    // Bypasses QString::indexOf
    const char* p = url;
    while (*p) {
        if (*p == '/' && *(p-1) != '/' && *(p+1) != '/') return p;
        p++;
    }
    return nullptr;
}

} // namespace Network
} // namespace Td

#endif // THUNDER_URL_WARP_H
