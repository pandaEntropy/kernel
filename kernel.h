#pragma once

#include "common.h"

#define PANIC(fmt, ...) \
    do{ \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        while(1){} \
    } while(0)

#define READ_CSR(reg) \
    ({ \
        unsigned long __tmp; \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp; \
     })

// "#" tells the preprocessor to stringify whatever is passed for reg
// "r" -> find any general purpose register and load the value of __tmp there
// "=r" -> find any gp register and load the value of the assembly into that register and then load from that register to the variable (__tmp in the case above)
#define WRITE_CSR(reg, value) \
    do{ \
        unsigned long __tmp = (value); \
        __asm__ __volatile__("csrw " #reg ", %0" :: "r"(__tmp)); \
    }while(0)

struct sbiret{
    long error;
    long value;
};

// Represents the program state saved in kernel_entry
struct trap_frame{
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed));
