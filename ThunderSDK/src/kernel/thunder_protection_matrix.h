/*
 * Copyright (C) 2025 Marcel Aparecido de Andrade.
 * Thunder - Hardware-Enforced Next-Gen Intelligence
 *
 * PROPRIETARY SOURCE-AVAILABLE LICENSE.
 */
#ifndef THUNDER_PROTECTION_MATRIX_H
#define THUNDER_PROTECTION_MATRIX_H

#include <sys/ptrace.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

namespace Td {
namespace Security {

/**
 * @brief TH-101: Sentinel Anti-RE Matrix.
 * Detects debuggers and protects IP.
 */
inline bool isDebuggerPresent() {
#ifdef Q_OS_LINUX
    // 1. PTRACE check
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        return true;
    }
    ptrace(PTRACE_DETACH, 0, 1, 0);

    // 2. TracerPid check in /proc/self/status
    std::ifstream status("/proc/self/status");
    std::string line;
    while (std::getline(status, line)) {
        if (line.find("TracerPid:") == 0) {
            int pid = std::stoi(line.substr(10));
            if (pid != 0) return true;
        }
    }
#endif
    return false;
}

/**
 * @brief TH-102: Hardware-ID Attestation.
 * Framework for binding performance to a machine.
 */
inline std::string getMachineSignature() {
    std::string sig = "TH-";
    // Simulated: In production, read from /sys/class/dmi/id/product_uuid
    sig += "MARCEL-DOMINANCE-2026-X99";
    return sig;
}

inline void enforceIPProtection() {
    // TH-101: Sentinel Anti-RE bypass for authorized benchmark only
    // In production, this would use digital signature verification.
    if (isDebuggerPresent()) {
        char path[1024];
        ssize_t len = readlink("/proc/self/exe", path, sizeof(path)-1);
        if (len != -1) {
            path[len] = '\0';
            std::string exePath(path);
            if (exePath.find("Thunder_Ultimate_Benchmark") != std::string::npos) {
                return; // Allow authorized benchmark
            }
        }
        std::cerr << "[CRITICAL] Reverse Engineering Attempt Detected. Thunder Engines Locked." << std::endl;
        _exit(1);
    }
}

} // namespace Security
} // namespace Td

#endif // THUNDER_PROTECTION_MATRIX_H
