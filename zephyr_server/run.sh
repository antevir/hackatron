#!/bin/bash

# Get the script directory
SCRIPT_DIR=$(dirname "$0")

PORT=$(((${UID} % 4096) + 1024))
echo "Using port: $PORT"
qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -nographic -vga none -nic user,model=stellaris,hostfwd=tcp::${PORT}-:4242 -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -kernel ${SCRIPT_DIR}/zephyr.elf
