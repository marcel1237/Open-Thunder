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
    if (isDebuggerPresent()) {
        std::cerr << "[CRITICAL] Reverse Engineering Attempt Detected. Thunder Engines Locked." << std::endl;
        _exit(1); // Immediate termination
    }
}

} // namespace Security
} // namespace Td

#endif // THUNDER_PROTECTION_MATRIX_H
