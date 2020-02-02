CROSS_COMPILE ?= arm-linux-gnueabihf-
TARGET		  ?= 6ul_freertos

CC 				:= $(CROSS_COMPILE)gcc
LD 				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP			:= $(CROSS_COMPILE)objdump

INCUDIRS		:= 	include \
					include/sdk \


SRCDIRS			:= 	src \
					src/driver \
					src/driver/sdk

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
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.S
	$(CC) -Wall -nostdlib -c -O0 -g $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O0 -g $(INCLUDE) -o $@ $<


qemu: $(OBJS)
	./qemu/bin/qemu-system-arm -M mcimx6ul-evk -show-cursor -m 512M \
			-kernel 6ul_freertos.elf -nographic -serial mon:stdio 

clean:
	rm -rf $(TARGET).elf $(TARGET).bin $(TARGET).dis $(OBJS)

print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo SOBJS = $(SOBJS)
	@echo COBJS = $(COBJS)
	@echo OBJS = $(OBJS)

