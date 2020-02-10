CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET		  ?= 6ul_freertos

CC 				:= $(CROSS_COMPILE)gcc
LD 				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP			:= $(CROSS_COMPILE)objdump

INCUDIRS		:= 	include \
					include/sdk \
					src/freertos/Source/include \
					src/fatfs


SRCDIRS			:= 	src \
					src/driver \
					src/app	\
					src/app/cmd \
					src/freertos/Source \
					src/fatfs

INCLUDE 		:= 	$(patsubst %, -I %, $(INCUDIRS))

INCLUDE 		:= 	$(patsubst %, -I %, $(INCUDIRS))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR		:= $(notdir $(SFILES))
CFILENDIR		:= $(notdir $(CFILES))

SOBJS			:= $(patsubst %, obj/%, $(SFILENDIR:.S=.o))
COBJS			:= $(patsubst %, obj/%, $(CFILENDIR:.c=.o))

OBJS			:= $(SOBJS)$(COBJS)

VPATH			:= $(SRCDIRS)

.PHONY:clean

$(TARGET).bin : $(OBJS)
	$(LD) -Tsrc/6ul_freertos.ld -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -S -m arm $(TARGET).elf > $(TARGET).dis
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).lst

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -g -O0 -mno-unaligned-access -Wall -mfloat-abi=hard -mfpu=vfpv4 -MMD -MP -fno-common -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mapcs -std=gnu99 -mcpu=cortex-a7  -DDEBUG -DCPU_MCIMX6Y2DVM05 -DFSL_RTOS_FREE_RTOS -DEVK_MCIMX6ULL $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c


	#$(CC) -Wall -nostdlib -c -O0 -g -mcpu=cortex-a7 $(INCLUDE) -o $@ $<
	$(CC) -Wall -nostdlib -c -g -O0 -mno-unaligned-access -Wall -mfloat-abi=hard -mfpu=vfpv4 -MMD -MP -fno-common -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mapcs -std=gnu99 -mcpu=cortex-a7 -DDEBUG -DCPU_MCIMX6Y2DVM05 -DFSL_RTOS_FREE_RTOS -DEVK_MCIMX6ULL $(INCLUDE) -o $@ $<

qemu: $(OBJS)
	qemu-system-arm -M mcimx6ul-evk -show-cursor -m 512M \
			-kernel 6ul_freertos.elf -serial mon:stdio \
			 -sd ../testfs.img
	 

debug: $(OBJS)
	qemu-system-arm -M mcimx6ul-evk -show-cursor -m 512M \
			-kernel 6ul_freertos.elf -serial mon:stdio -s -S \
			-sd ../testfs.img

clean:
	rm -rf $(TARGET).elf $(TARGET).bin $(TARGET).dis $(OBJS) *.lst

print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo SOBJS = $(SOBJS)
	@echo COBJS = $(COBJS)
	@echo OBJS = $(OBJS)

