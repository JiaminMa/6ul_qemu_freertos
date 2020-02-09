# How to run 
make qemu all

logs is as follow:
```c
qemu-system-arm -M mcimx6ul-evk -show-cursor -m 512M \
                -kernel 6ul_freertos.elf -nographic -serial mon:stdio
hello 6ul freertos
print_task creation success!.
tiny_console_task entry
tiny_console:$clear             
tiny_console:$hellp
invalid command
tiny_console:$hello             
hello cmd
tiny_console:$
```

# How to debug

make debug all

cgdb -d arm-linux-gdb 6ul_freertos.elf

# A tiny console to debug

![](https://s2.ax1x.com/2020/02/09/1fxiVO.gif)