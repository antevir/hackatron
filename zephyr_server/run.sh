#!/bin/bash
qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -nographic -vga none -nic user,model=stellaris,hostfwd=tcp::${UID}-:4242 -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -kernel zephyr.elf
