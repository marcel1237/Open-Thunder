/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * Licensed under the terms of the Multi-License Agreement (13 licenses).
 * See LICENSE.md in the project root for full license details.
 */
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main() {
    std::cout << "\033[1;36m============================================================\033[0m" << std::endl;
    std::cout << "\033[1;36m       THUNDER MATRIX - SYSTEM PAGING HARDWARE REPORT       \033[0m" << std::endl;
    std::cout << "\033[1;36m============================================================\033[0m" << std::endl;

    try {
        // Get top 20 processes by memory usage
        std::string output = exec("ps -eo comm,pid,rss --sort=-rss --no-headers | head -n 20");

        std::cout << "\033[1;33m" << std::left << std::setw(25) << "RESOURCE NAME"
                  << std::setw(10) << "PID"
                  << "SILICON PAGING (MB)" << "\033[0m" << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;

        char comm[256];
        int pid;
        long rss;
        double total_mb = 0;

        FILE* stream = fmemopen((void*)output.c_str(), output.size(), "r");
        while (fscanf(stream, "%s %d %ld", comm, &pid, &rss) == 3) {
            double mb = rss / 1024.0;
            total_mb += mb;
            std::cout << std::left << std::setw(25) << comm
                      << std::setw(10) << pid
                      << std::fixed << std::setprecision(2) << mb << " MB" << std::endl;
        }
        fclose(stream);

        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "\033[1;32mTOTAL HARDWARE PAGING TRACKED: " << total_mb << " MB\033[0m" << std::endl;
        std::cout << "\033[1;36mSTATUS: SILICON MONITORING ACTIVE - OMNICORE SYNCED\033[0m" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error generating report: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
