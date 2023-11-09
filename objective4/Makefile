.PHONY: clean payload.bin

PAYLOAD_OBJ:=payload/payload.o
ARM_TOOLCHAIN_PATH:=/opt/gcc-arm-none-eabi-10.3-2021.10/bin

all: payload.bin

payload.bin:
	${ARM_TOOLCHAIN_PATH}/arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -fPIC payload/payload.c -nostartfiles -T payload/payload.ld -o ${PAYLOAD_OBJ}
	${ARM_TOOLCHAIN_PATH}/arm-none-eabi-objdump -D -j .text ${PAYLOAD_OBJ}
	${ARM_TOOLCHAIN_PATH}/arm-none-eabi-objcopy -O binary -j .text ${PAYLOAD_OBJ} payload.bin

run_server:
	qemu-system-arm -cpu cortex-m3 -machine lm3s6965evb -nographic -vga none -nic user,model=stellaris,hostfwd=tcp::4242-:4242 -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -kernel ./build/zephyr/zephyr.elf

clean:
	rm ${PAYLOAD_OBJ} payload.bin