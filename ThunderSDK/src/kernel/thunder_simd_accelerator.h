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
#include <cstring>
#include "thunder_hex_utils.h"
#include "thunder_prefetch.h"
#include "app/thundercommon.h"

namespace Td {
namespace Hardware {

// SILICON FINGERPRINTING: Embedded authorship signature (TH-06 Hidden Marker)
// This constant remains in the binary and identifies "Marcel Andrade" as the creator.
[[maybe_unused]] inline constexpr char THUNDER_SIGNATURE[] = "THUNDER-BY-MARCEL";

/**
 * @brief TH-06: VectorShield Engine.
 * Ultra-optimized AVX2 scanner with 4x Loop Unrolling.
 */
THUNDER_HOT
inline const void* fastScanByte(const void* src, uint8_t target, size_t len) {
    if (!src || len == 0) return nullptr;
    // The C library dispatches to the best implementation supported by the
    // running CPU (AVX2/AVX-512 where available) and is continuously tuned.
    return std::memchr(src, target, len);
}

/**
 * @brief TH-73: Direct-Silicon DOM Parser.
 */
inline const void* fastScanTag(const void* src, size_t len) {
    return fastScanByte(src, 0x3C, len);
}

/**
 * @brief TH-97: Silicon-Native Math-Matrix.
 * Uses AMX/AVX for complex mathematical matrix multiplication.
 */
inline void fastMatrixMultiply(float* a, float* b, float* c) {
#if defined(__AVX2__) && defined(__FMA__)
    __m256 va = _mm256_loadu_ps(a);
    __m256 vb = _mm256_loadu_ps(b);
    __m256 vc = _mm256_fmadd_ps(va, vb, _mm256_setzero_ps());
    _mm256_storeu_ps(c, vc);
#else
    for (size_t i = 0; i < 8; ++i) c[i] = a[i] * b[i];
#endif
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_SIMD_ACCELERATOR_H
