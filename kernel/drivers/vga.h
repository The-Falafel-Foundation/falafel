#ifndef VGA_H
#define VGA_H

#include <stdint.h>

typedef struct {
   uint8_t width;
   uint8_t height;
   uint16_t *buffer;
} vga_info_t;

extern vga_info_t vga_info;

void vga_init(void);
void vga_print(const char *str);
void vga_put_char(char c, uint8_t color, int x, int y);
void vga_clear(void);

#endif // VGA_H
