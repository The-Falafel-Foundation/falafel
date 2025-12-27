#include "drivers/vga.h"
#include "type_helper.h"
#include "multiboot.h"
#include "pmm.h"

void kernel_main(uint32_t magic, uint32_t addr) {
    vga_init();
    char buf[11];

    if (magic != MULTIBOOT_MAGIC) {
        vga_print("Invalid Magic!\n");
        return;
    }

    vga_print("Validated magic!\n");
    pmm_init(addr);

    vga_print("PMM Initialized.\nFree Pages: ");
    uint32_to_str(free_pages, buf, 10);
    vga_print(buf);

   vga_print("\nLoaded the kernel\n");

    vga_print("\nTesting Alloc... Addr: ");
    void* ptr = pmm_alloc();
    uint32_to_str((uint32_t)ptr, buf, 16);
    vga_print(buf);

    while (1);
}
