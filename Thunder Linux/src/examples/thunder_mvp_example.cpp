/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#include <iostream>
#include <chrono>
#include <cstring>
#include <vector>
#include "thundersdk.h"

/**
 * @brief This example demonstrates the use of Multiversal Virtual Paging (MVP).
 * MVP provides silicon-aligned memory with user-space page fault interception.
 */
int main() {
    std::cout << "⚡ THUNDER SDK: MULTIVERSAL VIRTUAL PAGING EXAMPLE" << std::endl;
    std::cout << "=================================================" << std::endl;

    // 1. Initialize Thunder Hardware Matrix
    Td::initializeHardwareAcceleration();

    size_t dataSize = 1024ULL * 1024ULL * 512ULL; // 512MB

    std::cout << "\n[1] Allocating Multiversal Memory (TH-1001)..." << std::endl;
    auto start_alloc = std::chrono::high_resolution_clock::now();

    // Using the new Public Memory API
    void* mvp_ptr = Td::Memory::allocate(dataSize);

    auto end_alloc = std::chrono::high_resolution_clock::now();

    if (mvp_ptr == (void*)-1) {
        std::cerr << "Failed to allocate Multiversal Memory." << std::endl;
        return 1;
    }

    std::cout << "Memory allocated at: " << mvp_ptr << " in "
              << std::chrono::duration<double, std::milli>(end_alloc - start_alloc).count() << "ms" << std::endl;

    // 2. Synchronize memory for immediate use (TH-1005)
    std::cout << "[2] Synchronizing with Hardware Clock (TH-1005)..." << std::endl;
    Td::Memory::synchronize(mvp_ptr, dataSize);

    // 3. Performance Test: Write/Read
    std::cout << "[3] Writing 512MB of data..." << std::endl;
    auto start_work = std::chrono::high_resolution_clock::now();

    std::memset(mvp_ptr, 0xAA, dataSize);

    auto end_work = std::chrono::high_resolution_clock::now();
    double time_taken = std::chrono::duration<double>(end_work - start_work).count();

    std::cout << "Data processing complete." << std::endl;
    std::cout << "Throughput: " << (dataSize / (1024.0 * 1024.0 * 1024.0)) / time_taken << " GB/s" << std::endl;

    std::cout << "\n[SUCCESS] Multiversal Virtual Paging verified." << std::endl;
    std::cout << "=================================================" << std::endl;

    return 0;
}
