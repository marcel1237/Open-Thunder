#!/bin/bash
# ⚡ ThunderSDK Run Script
# Launches the Thunder System Tray service with yellow lightning icon

BLUE='\033[0;34m'
NC='\033[0m'

cd "$(dirname "$0")"

if [ ! -f "./ThunderSDKTray" ]; then
    echo -e "\033[0;31m✘ ThunderSDKTray not found. Running build.sh first...${NC}"
    ./build.sh
fi

echo -e "${BLUE}⚡ Launching Thunder Matrix Core Service...${NC}"
./ThunderSDKApp &
echo -e "\033[0;32m✔ Thunder Service Active in System Tray.${NC}"
