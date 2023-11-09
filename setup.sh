#!/bin/bash

set -e

# Get the script directory
SCRIPT_DIR=$(dirname "$0")

if [ -t 0 ]; then # Check if stdin is a terminal
    HEADLESS=0
else
    HEADLESS=1
fi

question()
{
    TEXT=$1
    RESP=$2 # default value
    retval=0
    # When HEADLESS==1 no question will be asked, the default value will be used instead
    if [ $HEADLESS -eq 0 ]; then
        read -p "$1 (y/n): " RESP
    fi
    if [ ! "$RESP" = "y" ]; then
        retval=1
    fi
    return $retval
}

echo
if question "Do you want to download and install the ARM GCC toolchain?" "n"; then
    ARM_TAR_FILE="gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2"
    wget -O "${HOME}/Downloads/${ARM_TAR_FILE}" "https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/${ARM_TAR_FILE}?rev=78196d3461ba4c9089a67b5f33edf82a&hash=5631ACEF1F8F237389F14B41566964EC"
    echo "Extracting ARM toolchain to /opt..."
    sudo tar xf "${HOME}/Downloads/${ARM_TAR_FILE}" -C /opt
    echo "done"
else
    echo "Skipping ARM GCC toolchain..."
fi

if question "Do you want to install qemu for ARM?" "n"; then
    sudo apt install qemu-system-arm
else
    echo "Skipping ARM GCC toolchain..."
fi
