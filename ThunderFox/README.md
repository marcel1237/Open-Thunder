# ⚡ ThunderFox: Lightweight Gecko-Rust Browser Core

ThunderFox is a high-performance browser prototype that integrates the **Gecko Rendering Engine** with a **Rust-based core**, optimized by the **Thunder Hardware Acceleration** matrix.

## 🏗️ Architecture

### 1. Gecko Engine Core
*   **Purpose:** Rendering HTML, CSS, and JavaScript.
*   **Benefit:** Preserves the independent web ecosystem (Non-Blink).
*   **Implementation:** Interfaces via FFI with `libxul` to leverage decades of Gecko optimization.

### 2. Rust-Powered Safety (Stylo)
*   **Purpose:** Parallel CSS processing and memory-safe resource management.
*   **Benefit:** Eliminates UAF (Use-After-Free) vulnerabilities and exploits Rust's "Fearless Concurrency" to saturate multi-core CPUs.
*   **Logic:** Inspired by Firefox's Project Stylo, splitting DOM styling tasks across threads.

### 3. Thunder Integration
*   **Hardware Sync:** Calls `ThunderSDK` to engage Ring-0 optimizations, CPU pinning, and SIMD acceleration.
*   **Performance:** Uses Thunder's "ApexPower" to lock CPU DMA latency during heavy rendering tasks.

## 🚀 Getting Started

1.  **Build Core:**
    ```bash
    cargo build
    ```

2.  **Run Prototype:**
    ```bash
    cargo run
    ```

## 📜 License
Part of the Thunder Multiversal Suite.
Copyright (c) 2026 Marcel Andrade.
