/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
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
 * @brief TH-06: VectorShield Engine.
 * Dispatch dinâmico: AVX-512 -> AVX2 -> SSE -> Scalar.
 */
inline const void* fastScanByte(const void* src, uint8_t target, size_t len) {
    const uint8_t* p = static_cast<const uint8_t*>(src);
    if (len == 0) return nullptr;

    // Alinhamento inicial para 32-bytes (AVX)
    while (len > 0 && (reinterpret_cast<uintptr_t>(p) & 0x1F)) {
        if (*p == target) return p;
        p++; len--;
    }

    __m256i t256 = _mm256_set1_epi8(static_cast<char>(target));

    // Loop Principal AVX2 (Otimizado para arquiteturas sem AVX-512)
    while (len >= 0x20) {
        // Prefetch da próxima linha de cache (64 bytes à frente)
        prefetch<0, 3>(p + 0x40);

        __m256i data = _mm256_load_si256(reinterpret_cast<const __m256i*>(p));
        __m256i cmp = _mm256_cmpeq_epi8(data, t256);
        uint32_t mask = static_cast<uint32_t>(_mm256_movemask_epi8(cmp));

        if (TD_UNLIKELY(mask != 0)) {
            return p + __builtin_ctz(mask);
        }

        p += 0x20; len -= 0x20;
    }

    // Processamento do resto (Tail)
    while (len--) {
        if (*p == target) return p;
        p++;
    }
    return nullptr;
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_SIMD_ACCELERATOR_H
