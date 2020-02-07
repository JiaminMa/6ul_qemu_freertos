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

```c
tiny_console:$top
===================================================
Task Name               Runtime         CPU Rate
init_console            738             <1%
IDLE                    77944           99%
print_task              5               <1%
Tmr Svc                 20              <1%
```