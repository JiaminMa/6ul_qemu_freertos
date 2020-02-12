TOOL_CHAIN = arm-none-eabi-
CC = ${TOOL_CHAIN}gcc
AS = ${TOOL_CHAIN}as
LD = ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS 		:= -Wall -g -fno-builtin -gdwarf-2 -gstrict-dwarf -Iinclude -mcpu=cortex-a7
LDFLAGS 	:= -g

objs := start.o \
		entry.o \
		driver/imx_uart.o \
		driver/imx_usdhc.o \
		device/qemu_print.o \
		device/sd_card.o \
		fatfs/diskio.o \
		fatfs/ff.o \
		fatfs/ffsystem.o \
		fatfs/ffunicode.o

6ul_bootloader.bin: $(objs)
	${LD} -T 6ul_bootloader.ld -o 6ul_bootloader.elf $^
	${OBJCOPY} -O binary -S 6ul_bootloader.elf $@
	${OBJDUMP} -D -m arm 6ul_bootloader.elf > 6ul_bootloader.dis


%.o:%.c
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC}  $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis driver/*.o device/*.o fatfs/*.o
