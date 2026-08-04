# Internal Development Guidelines for Thunder

Access to the Thunder source code is restricted to authorized developers. These guidelines ensure that the proprietary hardware-level excellence is maintained across all modules.

## 🛡️ The Golden Rule: Hardware First
Every addition to the codebase must prioritize **latency reduction** and **hardware efficiency**. High-level abstractions that introduce overhead are strictly prohibited.

## 🏗️ Development Standards

### 1. Authorized Access Only
Thunder is **Proprietary and Confidential**. All code changes must be performed within the secure authorized environment. Sharing source code or internal architectures (NitroCore, OmniLock, etc.) with unauthorized third parties is a violation of the EULA and employment terms.

### 2. Low-Level Logic & Hexadecimal
*   Constants and bitwise operations MUST use **Hexadecimal** for maximum precision and clarity in the hardware context.
*   Avoid heap allocations (`new`/`malloc`) in performance-critical paths. Use pre-allocated hardware buffers or stack-based execution.
*   Maximize the use of SIMD/AVX2 intrinsics.

### 3. CPU & Kernel Synchronization
*   Use `TD_LIKELY` and `TD_UNLIKELY` macros for every logical branch to assist hardware branch prediction.
*   New features must be verified for compatibility with the **vDSO** and **io_uring** bypass mechanisms.

### 4. Technical Documentation (Architecture CSV)
When a new hardware system or optimization is implemented:
1. Update the `Thunder_Tech_Architecture.csv` file.
2. Regenerate the `Thunder_Tech_Dashboard.html` for internal review.

## 🔒 Confidentiality & IP
By working on this project, you acknowledge that all intellectual property (IP), including but not limited to the **Vulkan-Warp Matrix** and **NitroCore Scheduler**, belongs exclusively to **Marcel Aparecido de Andrade**.

## 🚀 Quality Assurance: Hardware Handshake
Before merging any code, the `[THUNDER FINAL HARDWARE HANDSHAKE]` must return a `STATUS: ALL SYSTEMS NOMINAL`. Any drop in throughput or increase in syscall overhead will result in a rollback.

---
*Proprietary and Confidential - Internal Use Only*
