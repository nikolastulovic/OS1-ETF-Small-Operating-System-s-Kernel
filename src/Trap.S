# 1 "src/Trap.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/Trap.S"
.extern routineHandler
.extern console_handler
.extern timeHandler
.global supervisorTrap
.align 4
supervisorTrap:
    j ecallHandler
    j timerHandler
    sret
    sret
    sret
    sret
    sret
    sret
    j consoleHandler
    j consoleHandler

ecallHandler:
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8 (sp)
    .endr

    csrw sscratch, sp

    call routineHandler

    .irp index, 0,1,2,3,4,5,6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
     ld x\index, \index * 8 (sp)
    .endr
    addi sp, sp, 256
    sret

timerHandler:
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8 (sp)
    .endr

    csrw sscratch, sp

    call timeHandler

    .irp index, 0,1,2,3,4,5,6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8 (sp)
    .endr
    addi sp, sp, 256
    sret

consoleHandler:
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8 (sp)
    .endr

    csrw sscratch, sp

    call console_handler

    .irp index, 0,1,2,3,4,5,6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
     ld x\index, \index * 8 (sp)
    .endr
    addi sp, sp, 256
    sret
