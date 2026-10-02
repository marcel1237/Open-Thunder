# Development Guidelines for Thunder

Thunder is an open-source project multi-licensed under 13 different licenses. These guidelines ensure that the hardware-level excellence is maintained across all modules while fostering community collaboration.

## 🛡️ The Golden Rule: Hardware First
Every addition to the codebase must prioritize **latency reduction** and **hardware efficiency**. High-level abstractions that introduce overhead are discouraged in critical paths.

## 🏗️ Development Standards

### 1. Open Collaboration
Thunder is **Multi-Licensed Open Source**. We welcome contributions that maintain the high standards of performance and hardware-level synchronization. All code changes should follow the established patterns (NitroCore, OmniLock, etc.).

### 2. Low-Level Logic & Hexadecimal
*   Constants and bitwise operations SHOULD use **Hexadecimal** for maximum precision and clarity in the hardware context.
*   Avoid heap allocations (`new`/`malloc`) in performance-critical paths. Use pre-allocated hardware buffers or stack-based execution where possible.
*   Maximize the use of SIMD/AVX2 intrinsics.

### 3. CPU & Kernel Synchronization
*   Use `TD_LIKELY` and `TD_UNLIKELY` macros for every logical branch to assist hardware branch prediction.
*   New features must be verified for compatibility with the **vDSO** and **io_uring** bypass mechanisms.

### 4. Technical Documentation (Architecture CSV)
When a new hardware system or optimization is implemented:
1. Update the `Thunder_Tech_Architecture.csv` file.
2. Regenerate the `Thunder_Tech_Dashboard.html` for review.

## 🔒 Intellectual Property
All contributions to this project are subject to the multi-licensing model defined in [LICENSE.md](./LICENSE.md). The core architecture and technologies (NitroCore, OmniLock, etc.) remain protected by the selected licenses.

## 🚀 Quality Assurance: Hardware Handshake
Before merging any code, the `[THUNDER FINAL HARDWARE HANDSHAKE]` must return a `STATUS: ALL SYSTEMS NOMINAL`. Any drop in throughput or increase in syscall overhead will result in a rollback.

---
*Proprietary and Confidential - Internal Use Only*
