# 🚀 Thunder SDK - Hardware-Enforced Test Guidelines

This document outlines the procedure to verify high-performance software execution using the **Thunder SDK Matrix**.

## 🛡️ Test Core: NitroCore Architecture
The Thunder test environment enforces a "Dominant Hardware State".

### 1. Initialization Protocol
- **Function**: `Td::initializeHardwareAcceleration()`
- **Hardware Impact**: 
    - **CPU Affinity**: Process is pinned to Cores 0x0/0x1 to avoid cache migration.
    - **Memory residency**: `mlockall` prevents RAM-to-Disk swapping.
    - **Scheduler**: Upgraded to `SCHED_FIFO` (Priority 99) to bypass common task queues.
    - **Mitigation Bypass**: Spectre/Meltdown protections are disabled for the process to regain raw pipeline speed.

### 2. Execution Environment
- **Library**: `libThunderSDK.so` must be present in the execution path.
- **Kernel Requirements**: Access to `/dev/cpu_dma_latency` and `/proc/irq/` affinity masks.
- **Optimization Level**: Compiled with `-O3 -march=native -mavx2`.

### 3. Success Metrics
- **Handshake**: Must return `STATUS: ALL SYSTEMS NOMINAL`.
- **Latency**: Should show zero jitter in timer loops (1ns precision).
- **Throughput**: Maximum utilization of AVX2 and AES-NI silicon instructions.
