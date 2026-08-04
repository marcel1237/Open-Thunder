/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* SIMD (AVX2/SSE4.2) Hardware Accelerator
* ============================================================ */
#ifndef THUNDER_SIMD_ACCELERATOR_H
#define THUNDER_SIMD_ACCELERATOR_H

#include <immintrin.h>
#include <cstdint>
#include <cstddef>
#include "thunder_hex_utils.h"
#include "thunder_prefetch.h"

namespace Td {
namespace Hardware {

/**
 * @brief Uses AVX2 to scan memory for a specific byte at hardware level.
 * Processa 32 bytes (256 bits) em um único ciclo de clock.
 * Matematicamente, reduz o tempo de busca em O(N/32).
 */
inline const void* fastScanByte(const void* src, uint8_t target, size_t len) {
    const uint8_t* p = static_cast<const uint8_t*>(src);

    // Alinhamento Hexadecimal (0x1F = 31) para processamento vetorial
    while (len > 0 && (reinterpret_cast<uintptr_t>(p) & 0x1F)) {
        if (*p == target) return p;
        p++; len--;
    }

    // Carrega o alvo em todos os slots do registrador AVX de 256 bits
    __m256i target_vec = _mm256_set1_epi8(static_cast<char>(target));

    while (len >= 0x20) { // 32 bytes em hex
        // PREFETCH: Matematicamente, esconde a latência do barramento de memória (Memory Bus)
        // Antecipa as próximas duas linhas de cache (128 bytes)
        prefetch<0, 3>(p + 0x40);

        __m256i data = _mm256_load_si256(reinterpret_cast<const __m256i*>(p));
        __m256i cmp = _mm256_cmpeq_epi8(data, target_vec);

        // Gera uma máscara de bits hex do resultado da comparação
        uint32_t mask = static_cast<uint32_t>(_mm256_movemask_epi8(cmp));

        if (mask != 0x00000000) {
            // Encontra a posição exata usando contagem de zeros à direita (instrução de hardware)
            return p + __builtin_ctz(mask);
        }

        p += 0x20;
        len -= 0x20;
    }

    // Processa o restante (Tail)
    while (len > 0) {
        if (*p == target) return p;
        p++; len--;
    }

    return nullptr;
}

/**
 * @brief Hardware-level Hex string matching using SSE4.2 (STTNI).
 * Ideal para verificar regras de AdBlock instantaneamente.
 */
inline bool fastMatchHexRule(const char* data, const char* rule) {
    // __m128i é o registrador de 128 bits
    __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data));
    __m128i r = _mm_loadu_si128(reinterpret_cast<const __m128i*>(rule));

    // PCMPESTRI: Instrução de hardware para busca de strings (0x0C = Equal Each)
    int res = _mm_cmpestri(r, 0x10, d, 0x10, 0x0C);
    return res < 0x10;
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_SIMD_ACCELERATOR_H
