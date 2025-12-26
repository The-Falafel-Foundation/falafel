#include "drivers/vga.h"

void kernel_main(void) {
   vga_init();
   vga_print("Hello, World!");

   while (1);
}
