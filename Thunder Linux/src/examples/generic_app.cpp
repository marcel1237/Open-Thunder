/* ============================================================
* Thunder SDK - Generic Application Example
* Demonstrating Hardware-Level performance for non-browser apps.
* ============================================================ */
#include "../lib/thundersdk.h"
#include <iostream>
#include <vector>
#include <chrono>

void heavy_computation() {
    // A sample CPU-heavy task
    uint64_t sum = 0;
    for (uint64_t i = 0; i < 0x10000000; ++i) {
        sum += (i ^ 0x55555555) * 0x3;
    }
    std::cout << "Computation Result: " << std::hex << sum << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "--- Thunder SDK Generic Software Accelerator ---" << std::endl;

    // 1. INITIALIZE HARDWARE MATRIX
    // This pins the process to high-performance cores, locks RAM,
    // and sets real-time scheduling.
    Td::initializeHardwareAcceleration();

    std::cout << "\nStarting heavy hardware task..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    // 2. RUN PERFORMANCE-CRITICAL CODE
    heavy_computation();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "\nTask completed in: " << diff.count() << " seconds." << std::endl;
    std::cout << "Status: Success via NitroCore Engine." << std::endl;

    return 0;
}
