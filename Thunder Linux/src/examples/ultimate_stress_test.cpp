/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * Licensed under the terms of the Multi-License Agreement (13 licenses).
 * See LICENSE.md in the project root for full license details.
 * This code is public for visibility but use is governed by the TSAL v1.0.
 * Unauthorized commercial use or redistribution is strictly prohibited.
 */
#include <iostream>
#include <chrono>
#include <vector>
#include <sys/mman.h>
#include "thundersdk.h"

int main() {
    std::cout << "=== THUNDER ULTIMATE HARDWARE STRESS TEST ===" << std::endl;
    Td::initializeHardwareAcceleration();

    // Allocate 128MB of HugeTLB Hardware RAM
    size_t test_size = 0x8000000;
    void* hardware_ram = Td::Hardware::allocHugeMemory(test_size);

    if (hardware_ram == MAP_FAILED) {
        std::cerr << "[STRESS] Critical Error: Hardware memory allocation failed." << std::endl;
        return 1;
    }

    std::cout << "[STRESS] Hardware RAM Secured at: " << hardware_ram << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "[STRESS] Starting runtime-dispatched byte scans..." << std::endl;

    for (int i = 0; i < 100; ++i) {
        // Intensive Hardware Scan (AVX-512/AVX2)
        const void* result = Td::Hardware::fastScanByte(hardware_ram, 0xAA, test_size);
        asm volatile("" : : "r"(result) : "memory");
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "[RESULT] Vector Throughput: " << (static_cast<double>(test_size) * 100 / (1024 * 1024) / diff.count()) << " MB/s" << std::endl;
    std::cout << "STATUS: HARDWARE DOMINATED." << std::endl;

    Td::Hardware::freeHugeMemory(hardware_ram, test_size);
    return 0;
}
