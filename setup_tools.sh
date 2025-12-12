#!/bin/bash
set -e # Exit immediately if a command exits with a non-zero status

# 1. Define Versions (Easy to update later!)
# OpenOCD 0.12.0-2
OPENOCD_URL="https://github.com/xpack-dev-tools/openocd-xpack/releases/download/v0.12.0-2/xpack-openocd-0.12.0-2-linux-x64.tar.gz"
OPENOCD_DIR="openocd"

# ARM GCC 13.2.1-1.1 (Standard, modern version)
GCC_URL="https://github.com/xpack-dev-tools/arm-none-eabi-gcc-xpack/releases/download/v14.2.1-1.1/xpack-arm-none-eabi-gcc-14.2.1-1.1-linux-x64.tar.gz"
GCC_DIR="gcc"

mkdir -p tools
cd tools

# --- OPENOCD ---
if [ ! -d "$OPENOCD_DIR" ]; then
    echo "Downloading OpenOCD..."
    wget -q --show-progress "$OPENOCD_URL" -O openocd.tar.gz
    mkdir -p $OPENOCD_DIR 
    echo "Extracting OpenOCD..."
    tar xf openocd.tar.gz -C "$OPENOCD_DIR" --strip-components=1
    rm openocd.tar.gz
else
    echo "OpenOCD already installed."
fi

# --- ARM GCC ---
if [ ! -d "$GCC_DIR" ]; then
    echo "Downloading ARM GCC..."
    wget -q --show-progress "$GCC_URL" -O arm-gcc.tar.gz
    mkdir -p $GCC_DIR
    echo "Extracting ARM GCC..."
    tar xf arm-gcc.tar.gz -C "$GCC_DIR" --strip-components=1
    rm arm-gcc.tar.gz
else
    echo "ARM GCC already installed."
fi

echo "All tools setup in $(pwd)"
