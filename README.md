This is a freertos demo on 6ul qemu. It now include the features:
- A bootloader runs in BootROM space(0x0)
- freertos runs in DDR space(0x80000000)
- fatfs for sdcard
- a simple console, including top, taskinfo, ls, cat...
- a simple framebuffer demo

TODO:
- emWin
- lwip

# Prepare

1. create a fatfs image at current dir and mount it.
```
mkfs.msdos -F 32 -C testfs.img 131072
sudo mount -t msdos -o loop testfs.img /mnt/sdcard/
```
2. build bootloader.
```
make bootloader
```

# How to run 
make; sudo make boot

logs is as follow:
```c
qemu-system-arm: warning: nic imx.enet.0 has no peer
qemu-system-arm: warning: nic imx.enet.1 has no peer
hello imx6ul bootloader
boot_freertos entry
boot_freertos start to read os.bin
boot_freertos read os.bin done
hello 6ul freertos
initd creation success!.
print_task creation success!.
displayd creation success!.
display_task entry
tiny_console_task entry
tiny_console:$init_task entry
init_file_system f_mount res:0

tiny_console:$top
===================================================
Task Name		Runtime		CPU Rate
init_console       	15		1%
IDLE               	1080		77%
displayd           	259		18%
print_task         	1		<1%
initd              	44		3%
Tmr Svc            	6		<1%

```

# How to debug

make debug all

cgdb -d arm-linux-gdb 6ul_freertos.elf

# A tiny console to debug

![tiny console preview](https://s2.ax1x.com/2020/02/09/1fxiVO.gif)

# LCD Demo
![LCD demo](https://s2.ax1x.com/2020/02/10/15mCeU.gif)