#!/bin/bash
# ⚡ ThunderSDK Build Script
# Automatically configures and compiles the SDK and System Tray

GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}⚡ Initializing ThunderSDK Compilation...${NC}"

# Navigate to the build directory (where the script is)
cd "$(dirname "$0")"

cmake ..
if make -j$(nproc); then
    echo -e "${GREEN}✔ ThunderSDK Build Successful.${NC}"
else
    echo -e "\033[0;31m✘ Build Failed.\033[0m"
    exit 1
fi
