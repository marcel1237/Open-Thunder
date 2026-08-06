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
#include <cstring>

namespace Td {
namespace Network {

/**
 * @brief TH-41: Bitwise-URL-Warp.
 * Fast path for URL segment detection using bitwise checks.
 */
inline bool isHttps(const char* url, size_t length) {
    return url && length >= 8 && std::memcmp(url, "https://", 8) == 0;
}

inline const char* findPathStart(const char* url) {
    if (!url) return nullptr;
    const char* p = url;
    while (*p) {
        if (*p == '/' && (p == url || *(p - 1) != '/') && *(p + 1) != '/') return p;
        p++;
    }
    return nullptr;
}

} // namespace Network
} // namespace Td

#endif // THUNDER_URL_WARP_H
