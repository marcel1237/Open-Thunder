# Thunder - Hardware-Enforced Next-Gen Intelligence

Thunder is a Linux/Qt performance SDK and browser prototype. It combines measured SIMD paths with optional Linux process tuning. Privileged optimizations are capability-dependent and must be verified at runtime.

## ⚡ Core Philosophy: The Hardware-Direct Path
Thunder prefers measurable low-overhead paths. It does not run in Ring-0 and does not bypass the Linux security model; kernel requests may fail and are reported as capabilities.

## 🚀 Key Architectures (Thunder-Tech)

Thunder's power is derived from its unified matrix of hardware-level systems:

*   **TH-01: NitroCore CPU-Matrix**: A unified processor engine that controls real-time scheduling (SCHED_FIFO), pinning tasks to physical cores, and utilizing vDSO/TSC for nanosecond-precision timing.
*   **TH-02: OmniLock RAM Architecture**: Ensures 100% RAM residency using HugePages (2MB) and hardware memory guarding (`mlockall`), eliminating disk I/O latency.
*   **TH-03: Network helpers**: bounded protocol parsing and optional socket tuning.
*   **TH-04: Graphics hints**: opt-in Qt/driver configuration; no custom Vulkan renderer is currently implemented.
*   **TH-07: Dark Volt Architecture**: An early-boot kernel integration that initializes Thunder immediately after the kernel, bypassing display managers for instant availability.

For a full list of technologies, see the [Thunder Tech Architecture Dashboard](./Thunder_Tech_Dashboard.html).

## 🛠️ Project Structure

*   **Thunder Linux/**: The primary implementation for the Linux Kernel, featuring deep hardware hooks.
*   **Dark Volt Kernel Technology/**: experimental systemd/EGLFS integration components.

## 🔒 Licensing
Thunder is **Multi-Licensed Open Source Software**. It is available under the terms of 13 different licenses, including GPL v3.0, MIT, Apache 2.0, MPL 2.0, and others. You may choose the license that best suits your needs.

See [LICENSE.md](./LICENSE.md) for the full list of licenses and their terms.

## 🏗️ Development (Internal Use Only)

### Core Dependencies
*   Qt 6.4+ (Core, WebEngine, Widgets)
*   CMake 3.16+
*   Linux with optional `io_uring`, huge-page and realtime capabilities

### Build and validation

```bash
cmake -S ThunderSDK -B build-sdk -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build-sdk -j
ctest --test-dir build-sdk --output-on-failure
```

Use `-DTHUNDER_NATIVE_OPTIMIZATIONS=ON` only for host-specific performance builds. Use `-DTHUNDER_ENABLE_SANITIZERS=ON` in a separate debug build.

Adblock rules are loaded only when `THUNDER_ADBLOCK_RULES=/path/to/list.txt` is set. Experimental Chromium flags are never forced; advanced users may provide version-specific flags through `THUNDER_EXPERIMENTAL_GPU_FLAGS` before startup.

## 🛡️ Hardware Handshake
Thunder prints a capability report showing observed scheduler, affinity, DMA latency lock and compiled SIMD path. It does not claim unavailable capabilities as active.

---
**Author:** Marcel Aparecido de Andrade  
**Version:** 1.0.0 (February 2025)
**Copyright:** (C) 2025 Marcel Aparecido de Andrade. All Rights Reserved.
