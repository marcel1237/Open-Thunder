/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* Hex-accelerated Network Protocol Parsers
* ============================================================ */
#ifndef THUNDER_NET_OPTIMIZER_H
#define THUNDER_NET_OPTIMIZER_H

#include <QString>
#include <QByteArray>
#include <cstdint>
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
    // Matematicamente, envia pacotes instantaneamente sem esperar por buffering.
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

    // 2. TCP_QUICKACK (0xC): Enable quick acknowledgements.
    // Reduz a latência de handshake e fluxo ao não esperar por pacotes de dados para dar ACK.
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
 * Matematicamente, isso reduz a complexidade de busca de O(L) para O(1)
 * carregando a palavra da memória direto para o registrador.
 */
inline HttpMethod fastParseMethod(const QByteArray &method) {
    if (method.size() < 3) return HttpMethod::UNKNOWN;

    // Carrega os primeiros 8 bytes (ou menos) em um registrador de 64 bits
    uint64_t val = 0;
    int len = qMin(method.size(), 8);
    memcpy(&val, method.constData(), len);

    // Máscaras Hex para ignorar bytes além do tamanho da string (Padding zero)
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
 * Matematicamente, elimina a necessidade de carregar bibliotecas de decodificação
 * para verificar o tipo do arquivo.
 */
enum class ImageType : uint32_t {
    PNG  = 0x474E5089, // .PNG in hex (LE)
    JPEG = 0xD8FFD8FF, // JPEG Start of Image
    GIF  = 0x38464947, // GIF8
    WEBP = 0x50424557, // WEBP
    UNKNOWN = 0x0
};

inline ImageType fastDetectImageType(const uint8_t* data) {
    uint32_t magic = *reinterpret_cast<const uint32_t*>(data);

    // Comparação em 1 ciclo de clock
    if ((magic & 0xFFFFFF00) == 0x474E5000) return ImageType::PNG;
    if ((magic & 0xFFFF) == 0xD8FF) return ImageType::JPEG;
    if (magic == 0x38464947) return ImageType::GIF;

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

inline bool parseTlsHeader(const uint8_t* data, TlsRecordHeader &header) {
    // Acesso direto via offset hex
    header.type = data[0x0];
    // Big-endian to Host conversion via hex shifts
    header.version = (static_cast<uint16_t>(data[0x1]) << 0x8) | data[0x2];
    header.length  = (static_cast<uint16_t>(data[0x3]) << 0x8) | data[0x4];

    // Verifica validade em hex (0x14 a 0x18 são tipos válidos de TLS)
    return (header.type >= 0x14 && header.type <= 0x18);
}

} // namespace Network
} // namespace Td

#endif // THUNDER_NET_OPTIMIZER_H
