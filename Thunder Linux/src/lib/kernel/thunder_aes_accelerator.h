/* ============================================================
* Thunder - Qt web browser
* Copyright (C) 2025 Marcel
*
* AES-NI Hardware Cryptography Accelerator
* ============================================================ */
#ifndef THUNDER_AES_ACCELERATOR_H
#define THUNDER_AES_ACCELERATOR_H

#include <wmmintrin.h>
#include <immintrin.h>
#include <cstdint>

namespace Td {
namespace Hardware {

/**
 * @brief AES-NI Hardware Encryption for a single 128-bit block.
 * Executa a rodada de criptografia AES diretamente nos registradores XMM.
 */
inline __m128i aesEncryptBlock(__m128i data, const __m128i* keys, int rounds) {
    __m128i tmp = _mm_xor_si128(data, keys[0]);
    for (int i = 1; i < rounds; i++) {
        tmp = _mm_aesenc_si128(tmp, keys[i]);
    }
    return _mm_aesenclast_si128(tmp, keys[rounds]);
}

/**
 * @brief AES-NI Hardware Decryption for a single 128-bit block.
 * Matematicamente, o custo de descriptografia HTTPS cai para quase zero.
 */
inline __m128i aesDecryptBlock(__m128i data, const __m128i* keys, int rounds) {
    __m128i tmp = _mm_xor_si128(data, keys[0]);
    for (int i = 1; i < rounds; i++) {
        tmp = _mm_aesdec_si128(tmp, keys[i]);
    }
    return _mm_aesdeclast_si128(tmp, keys[rounds]);
}

/**
 * @brief Check for AES-NI support using CPUID (Hex level)
 */
inline bool hasAesNi() {
    uint32_t ecx;
    // CPUID EAX=1, ECX bit 25 is AES-NI
    __asm__("cpuid" : "=c"(ecx) : "a"(1) : "ebx", "edx");
    return (ecx & 0x02000000) != 0;
}

} // namespace Hardware
} // namespace Td

#endif // THUNDER_AES_ACCELERATOR_H
