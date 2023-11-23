#include <stddef.h>

#define RED "\e[0;31m"

typedef void (*printf_t)(const char *, ...);
typedef void (*sleep_t)(int, int);

__attribute__((naked,section(".main"))) void main(void)
{
    // Decrease the stack pointer to make space for our program
    // If we don't do this our data will be overwritten when we call
    // a function.
    __asm("sub    sp, #256");

    static const char text[] = "                        You have been hacked\n";
    const char *pDataPtr=&text[0];

    printf_t _printf = (printf_t)0x00010394 + 1;
    sleep_t _sleep = (sleep_t)0x0000e874 + 1; // z_impl_k_sleep

    _printf(RED);
    while(1) {
        _printf(pDataPtr);
        if (*pDataPtr == 0) {
            pDataPtr=&text[0];
        } else {
            pDataPtr++;
        }
        _sleep(4,0);
    }
}