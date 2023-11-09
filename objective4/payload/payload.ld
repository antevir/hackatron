buffer_address = 0x20000000 ; /* <= You need to change this address: */

MEMORY
{
  data     (rwx) : ORIGIN = buffer_address, LENGTH = 0x00000040
  xip      (rx)  : ORIGIN = ORIGIN(data) + 4 /* return address */ + LENGTH(data), LENGTH = 0x00000100
}

SECTIONS
{
    .text :
    {
        *(.text*)
        *(.rodata*)
    } > xip
    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
    } > data
}
