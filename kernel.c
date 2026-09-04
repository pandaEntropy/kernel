#include "kernel.h"
#include "common.h"

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// Referencing the labels from the linker script
extern char __bss[], __bss_end[], __stack_top[];

//long ensures that a variable matches the target cpu's word size
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long fid, long eid){
    register long a0 __asm__("a0") = arg0; //assign arg0 to a0, but store it in the a0 register and NOT on the stack
    register long a1 __asm__("a1") = arg1;
    register long a2 __asm__("a2") = arg2;
    register long a3 __asm__("a3") = arg3;
    register long a4 __asm__("a4") = arg4;
    register long a5 __asm__("a5") = arg5;
    register long a6 __asm__("a6") = fid;
    register long a7 __asm__("a7") = eid;

    __asm__ __volatile__("ecall"
                        : "=r"(a0), "=r"(a1)
                        : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
                        : "memory");

    return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char c){
    sbi_call(c, 0, 0, 0, 0, 0, 0, 1);
}

void *memset(void *buf, char c, size_t n){
    uint8_t *p = (uint8_t *) buf;

    while(n > 0){
        *p = c;
        p++;
        n--;
    }

    return buf;
}

void kernel_main(){
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);

    printf("\n\nhello world!\n");
    printf("peak kernel\n");

    for(;;){
        __asm__ __volatile__("wfi");
    }
}

__attribute__((section(".text.boot"))) //place the boot function at .text.boot
__attribute__((naked))
void boot(){
    __asm__ __volatile__(
            //copy the memory address in stack_top (a general purpose register in this case) into the sp register
            "mv sp, %[stack_top]\n" 
            "j kernel_main\n" // jump to the kernel_main function
            :
            : [stack_top] "r" (__stack_top) //load the value of __stack_top to any general purpose register ("r")
    );
}
