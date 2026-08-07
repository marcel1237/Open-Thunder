#!/bin/bash
# ⚡ Thunder Project - Global Build Script
# Automates the compilation of ThunderSDK and ThunderBrowser

# Set colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Find project root relative to script location
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
PROJECT_ROOT=$(cd "$SCRIPT_DIR/../.." && pwd)

echo -e "${BLUE}============================================================${NC}"
echo -e "${BLUE}⚡ THUNDER PROJECT - STARTING GLOBAL BUILD PROCESS${NC}"
echo -e "${BLUE}Root: $PROJECT_ROOT${NC}"
echo -e "${BLUE}============================================================${NC}"

SDK_DIR="$PROJECT_ROOT/ThunderSDK"
BROWSER_DIR="$PROJECT_ROOT/Thunder Linux"
EXPORT_DIR="$PROJECT_ROOT/Thunder_Final_Export"

# 1. Build ThunderSDK
echo -e "\n${BLUE}[1/2] Building ThunderSDK...${NC}"
mkdir -p "$SDK_DIR/build"
cd "$SDK_DIR/build" || exit
cmake ..
if make -j$(nproc); then
    echo -e "${GREEN}✔ ThunderSDK Build Successful.${NC}"
else
    echo -e "${RED}✘ ThunderSDK Build Failed.${NC}"
    exit 1
fi

# 2. Build ThunderBrowser
echo -e "\n${BLUE}[2/2] Building ThunderBrowser (Linked against SDK)...${NC}"
mkdir -p "$BROWSER_DIR/build"
cd "$BROWSER_DIR/build" || exit
cmake ..
if make -j$(nproc); then
    echo -e "${GREEN}✔ ThunderBrowser Build Successful.${NC}"
else
    echo -e "${RED}✘ ThunderBrowser Build Failed.${NC}"
    exit 1
fi

# 3. Synchronize Export Directory
echo -e "\n${BLUE}[+] Synchronizing Final Export...${NC}"
mkdir -p "$EXPORT_DIR/Docs"
mkdir -p "$EXPORT_DIR/Binaries"

cp "$PROJECT_ROOT/"*.md "$EXPORT_DIR/Docs/"
cp "$PROJECT_ROOT/Thunder_Tech_Architecture.csv" "$EXPORT_DIR/Docs/"
cp "$PROJECT_ROOT/Thunder_Tech_Dashboard.html" "$EXPORT_DIR/Docs/"

cp "$SDK_DIR/build/libThunderSDK.so" "$EXPORT_DIR/Binaries/"
cp "$SDK_DIR/build/ThunderSDKTray" "$EXPORT_DIR/Binaries/"
cp "$SDK_DIR/build/ThunderSDKApp" "$EXPORT_DIR/Binaries/"
cp "$BROWSER_DIR/build/ThunderBrowser" "$EXPORT_DIR/Binaries/"
cp "$BROWSER_DIR/build/Thunder_Ultimate_Benchmark" "$EXPORT_DIR/Binaries/"
cp "$BROWSER_DIR/build/Thunder_Paging_Report" "$EXPORT_DIR/Binaries/"
cp "$BROWSER_DIR/build/Thunder_Hardware_Stress" "$EXPORT_DIR/Binaries/"

echo -e "\n${GREEN}============================================================${NC}"
echo -e "${GREEN}⚡ BUILD COMPLETE - SYSTEM READY FOR DOMINANCE${NC}"
echo -e "${GREEN}============================================================${NC}"
