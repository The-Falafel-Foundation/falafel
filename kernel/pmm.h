#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define MAX_PHYS_MEM 0xFFFFFFFF
#define BITMAP_SIZE (MAX_PHYS_MEM / PAGE_SIZE / 8)

void pmm_init(uint32_t multiboot_addr);
void* pmm_alloc(void);

extern uint32_t total_pages;
extern uint32_t free_pages;

#endif
