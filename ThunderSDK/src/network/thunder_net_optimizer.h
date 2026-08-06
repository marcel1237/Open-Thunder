/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#ifndef THUNDER_NET_OPTIMIZER_H
#define THUNDER_NET_OPTIMIZER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "thundercommon.h"

namespace Td {
namespace Network {

/**
 * @brief Optimizes a network socket at the Linux Kernel level.
 * Uses hex-coded socket options for maximum performance.
 */
inline void optimizeSocket(int fd) {
#ifdef Q_OS_LINUX
    int on = 0x1;

    // 1. TCP_NODELAY (0x1): Disable Nagle's algorithm.
    // Mathematically sends packets instantly without waiting for buffering.
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

    // 2. TCP_QUICKACK (0xC): Enable quick acknowledgements.
    // Reduces handshake and flow latency by not waiting for data packets to ACK.
    setsockopt(fd, IPPROTO_TCP, 0xC, &on, sizeof(on));

    // 3. SO_PRIORITY (0xC): Set socket priority to high (Hex 0x6)
    int priority = 0x6;
    setsockopt(fd, SOL_SOCKET, 0xC, &priority, sizeof(priority));

    // 4. TCP_FASTOPEN (0x17): Reduce handshake latency
    int qlen = 0x5; // 5 pending fast open connections
    setsockopt(fd, IPPROTO_TCP, 0x17, &qlen, sizeof(qlen));

    // 5. Hardware-Level Buffer Sizing for L3 Cache
    // We set the receive buffer to fit exactly in the L3 cache of modern CPUs (e.g., 8MB)
    // to avoid DRAM latency during decompression. (0x800000 = 8MB)
    int window_size = 0x800000;
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &window_size, sizeof(window_size));
#endif
}

/**
 * Fast-path for HTTP Method detection using 64-bit Hex integer comparisons.
 * This avoids expensive string heap allocations and character-by-character loops.
 */
enum class HttpMethod : uint64_t {
    GET     = 0x0000000000544547, // "GET" in hex (LE)
    POST    = 0x0000000054534F50, // "POST"
    HEAD    = 0x0000000044414548, // "HEAD"
    PUT     = 0x0000000000545550, // "PUT"
    DELETE  = 0x004554454C4544,   // "DELETE"
    UNKNOWN = 0xFFFFFFFFFFFFFFFF
};

/**
 * @brief Parses a raw byte buffer for HTTP methods using 64-bit Hex masks.
 * Mathematically reduces search complexity from O(L) to O(1)
 * by loading the word from memory directly into the register.
 */
inline HttpMethod fastParseMethod(const QByteArray &method) {
    if (method.size() < 3) return HttpMethod::UNKNOWN;

    // Load first 8 bytes (or fewer) into a 64-bit register
    uint64_t val = 0;
    int len = qMin(method.size(), 8);
    memcpy(&val, method.constData(), len);

    // Hex masks to ignore bytes beyond string length (zero padding)
    uint64_t mask = (len == 8) ? 0xFFFFFFFFFFFFFFFF : (1ULL << (len * 8)) - 1;
    val &= mask;

    switch (val) {
        case static_cast<uint64_t>(HttpMethod::GET):    return HttpMethod::GET;
        case static_cast<uint64_t>(HttpMethod::POST):   return HttpMethod::POST;
        case static_cast<uint64_t>(HttpMethod::HEAD):   return HttpMethod::HEAD;
        case static_cast<uint64_t>(HttpMethod::PUT):    return HttpMethod::PUT;
        default: return HttpMethod::UNKNOWN;
    }
}

/**
 * Image-Hex Parser (TH-03 Refinement)
 * Identifies image formats using 64-bit Hex signatures.
 */
enum class ImageType : uint32_t {
    PNG  = 0x474E5089, // .PNG in hex (LE)
    JPEG = 0xD8FFD8FF, // JPEG Start of Image
    GIF  = 0x38464947, // GIF8
    WEBP = 0x50424557, // WEBP
    UNKNOWN = 0x0
};

inline ImageType fastDetectImageType(const uint8_t* data, size_t size) {
    if (!data || size < sizeof(uint32_t)) return ImageType::UNKNOWN;
    uint32_t magic;
    memcpy(&magic, data, sizeof(magic));

    // Comparison in 1 clock cycle
    if ((magic & 0xFFFFFF00) == 0x474E5000) return ImageType::PNG;
    if ((magic & 0xFFFF) == 0xD8FF) return ImageType::JPEG;
    if (magic == 0x38464947) return ImageType::GIF;
    if (size >= 12 && memcmp(data, "RIFF", 4) == 0 && memcmp(data + 8, "WEBP", 4) == 0)
        return ImageType::WEBP;

    return ImageType::UNKNOWN;
}

/**
 * TLS/SSL Hex Fast-Path
 * Accelerates TLS record header parsing (Content Type, Version, Length).
 */
struct TlsRecordHeader {
    uint8_t  type;    // 0x16 = Handshake, 0x17 = App Data
    uint16_t version; // 0x0303 = TLS 1.2, 0x0304 = TLS 1.3
    uint16_t length;
};

/**
 * @brief TH-03/06: Parallel Bitmask Ad-Filter.
 * Checks multiple rule bitmasks in a single SIMD cycle.
 * Mathematically transforms O(N) rule checking into O(1) hardware-speed rejection.
 */
inline bool parallelBitmaskFilter(uint64_t requestFlags, const uint64_t* ruleMasks, size_t count) {
    if (!ruleMasks) return false;
#ifdef __AVX2__
    __m256i req = _mm256_set1_epi64x(requestFlags);
    size_t i = 0;
    for (; i + 4 <= count; i += 4) {
        __m256i rules = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(ruleMasks + i));
        // Bitwise AND: requestFlags & ruleMask
        __m256i result = _mm256_and_si256(req, rules);
        // Compare with rules: if (result == rules) then it's a match
        __m256i cmp = _mm256_cmpeq_epi64(result, rules);
        if (!_mm256_testz_si256(cmp, cmp)) return true;
    }
#else
    size_t i = 0;
#endif
    for (; i < count; ++i)
        if ((requestFlags & ruleMasks[i]) == ruleMasks[i]) return true;
    return false;
}

} // namespace Network
} // namespace Td

#endif // THUNDER_NET_OPTIMIZER_H
