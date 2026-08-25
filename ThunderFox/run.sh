#!/bin/bash
# ThunderFox Standalone Run Script

export PYTHONPATH="./build/python"
export BINDGEN_EXTRA_CLANG_ARGS="-std=c++20"

echo "[Thunder] Launching Multiversal Core..."
cargo run --features style/servo
