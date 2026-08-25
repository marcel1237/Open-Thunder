#!/bin/bash
# ThunderFox Standalone Build Script
# ⚡ THE POWER OF GECKO IN YOUR HANDS ⚡

export PYTHONPATH="./build/python"
export BINDGEN_EXTRA_CLANG_ARGS="-std=c++20"

echo "[Thunder] Building Standalone Engine..."
cargo build --features style/servo
