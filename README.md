# How to run 
make qemu all

logs is as follow:
```c
qemu-system-arm -M mcimx6ul-evk -show-cursor -m 512M \
                -kernel 6ul_freertos.elf -nographic -serial mon:stdio
main print init success
 hello_task:0
hello_task2:0
hello_task:1
hello_task2:1
hello_task:2
hello_task2:2
hello_task:3
hello_task2:3
hello_task:4
hello_task2:4
hello_task:5
hello_task2:5
hello_task:6
hello_task2:6
hello_task:7
hello_task2:7
```

# How to debug

make debug all
cgdb -d arm-linux-gdb 6ul_freertos.elf

# A tiny console to debug

## trace task info

```c
tiny_console:$taskinfo
===================================================
Task Name               Stat    Prio    RStack  TID
tiny_console_task       R       3       955     1
IDLE                    R       0       60      3
print_task              S       4       92      2
Tmr Svc                 S       4       138     4
```