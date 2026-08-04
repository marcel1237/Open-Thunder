/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#include <iostream>
#include <chrono>
#include <cstdint>

void heavy_computation() {
    uint64_t sum = 0;
    for (uint64_t i = 0; i < 0x10000000; ++i) {
        sum += (i ^ 0x55555555) * 0x3;
    }
    std::cout << "Computation Result: " << std::hex << sum << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "--- Standard Software Baseline (No Acceleration) ---" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    heavy_computation();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Baseline task completed in: " << diff.count() << " seconds." << std::endl;
    return 0;
}
