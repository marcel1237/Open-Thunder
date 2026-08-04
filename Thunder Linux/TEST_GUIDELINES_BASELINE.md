# 🐢 Standard Baseline - OS-Managed Test Guidelines

This document outlines the standard software behavior without any hardware-level enforcement from Thunder.

## 🛡️ Test Core: Standard Kernel Scheduling
The baseline test relies entirely on the default Linux Kernel scheduler (Completely Fair Scheduler - CFS).

### 1. Initialization Protocol
- **Method**: Standard C++ entry point.
- **Hardware Impact**: 
    - **Dynamic Migration**: The Kernel is free to move the process between any available CPU cores.
    - **Energy Management**: The CPU may enter C-States (Sleep) during idle periods, introducing wake-up latency.
    - **Disk Swapping**: The OS may swap memory pages to disk if the system is under pressure.
    - **Security Mitigations**: Full Spectre/Meltdown protections are active, introducing overhead in the branch predictor.

### 2. Execution Environment
- **Context**: standard user-space process.
- **Kernel Relationship**: Subject to standard syscall overhead and context switching penalties.

### 3. Baseline Metrics
- **Handshake**: None (Standard OS startup).
- **Latency**: Variable jitter depending on background system load (GNOME/KDE activity).
- **Throughput**: Shared execution resources with other processes.
