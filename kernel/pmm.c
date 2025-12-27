#include "pmm.h"
#include "multiboot.h"

extern uint32_t _kernel_end;

// 1 bit per 4KB page (4GB / 4096 / 8 = 131072 bytes)
static uint8_t pmm_bitmap[131072];

uint32_t total_pages = 0;
uint32_t free_pages = 0;

static void pmm_mark_used(uint32_t page_addr) {
    uint32_t index = page_addr / PAGE_SIZE;
    pmm_bitmap[index / 8] |= (1 << (index % 8));
}

static void pmm_mark_free(uint32_t page_addr) {
    uint32_t index = page_addr / PAGE_SIZE;
    pmm_bitmap[index / 8] &= ~(1 << (index % 8));
}

void* pmm_alloc(void) {
    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        if (pmm_bitmap[i] != 0xFF) {
            for (int j = 0; j < 8; j++) {
                if (!(pmm_bitmap[i] & (1 << j))) {
                    uint32_t addr = (i * 8 + j) * PAGE_SIZE;
                    pmm_mark_used(addr);
                    free_pages--;
                    return (void*)addr;
                }
            }
        }
    }
    return 0;
}

void pmm_init(uint32_t multiboot_addr) {
    for (uint32_t i = 0; i < BITMAP_SIZE; i++)
        pmm_bitmap[i] = 0xFF;

    struct multiboot_tag *tag = (struct multiboot_tag *)(multiboot_addr + 8);

    while (tag->type != 0) {
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            struct multiboot_mmap_entry *mmap =
                (struct multiboot_mmap_entry *)((uint8_t *)tag + 16);

            uint32_t entries = (tag->size - 16) / 24;

            for (uint32_t i = 0; i < entries; i++) {
                if (mmap[i].type == 1) { // Available RAM
                    uint32_t start = (uint32_t)mmap[i].addr;
                    uint32_t end   = start + (uint32_t)mmap[i].len;

                    for (uint32_t a = start; a < end; a += PAGE_SIZE) {
                        pmm_mark_free(a);
                        free_pages++;
                    }
                }
            }
        }
        tag = (struct multiboot_tag *)(
            (uint8_t *)tag + ((tag->size + 7) & ~7)
        );
    }

    uint32_t k_end = (uint32_t)&_kernel_end;
    for (uint32_t a = 0; a < k_end; a += PAGE_SIZE) {
        if (!(pmm_bitmap[(a / PAGE_SIZE) / 8] &
              (1 << ((a / PAGE_SIZE) % 8)))) {
            pmm_mark_used(a);
            free_pages--;
        }
    }
}

