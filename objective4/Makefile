.PHONY: clean payload.bin run_server

PAYLOAD_OBJ:=payload/payload.o
ARM_TOOLCHAIN_PATH:=/opt/gcc-arm-none-eabi-10.3-2021.10/bin

all: payload.bin

payload.bin:
	${ARM_TOOLCHAIN_PATH}/arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -fPIC payload/payload.c -nostartfiles -T payload/payload.ld -o ${PAYLOAD_OBJ}
	${ARM_TOOLCHAIN_PATH}/arm-none-eabi-objdump -D -j .text ${PAYLOAD_OBJ}
	${ARM_TOOLCHAIN_PATH}/arm-none-eabi-objcopy -O binary -j .text ${PAYLOAD_OBJ} payload.bin

run_server:
	../zephyr_server/run.sh

clean:
	rm ${PAYLOAD_OBJ} payload.bin