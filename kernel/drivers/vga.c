#include "vga.h"

vga_info_t vga_info = {80, 25, (uint16_t *)0xB8000};

static int cursor_x = 0;
static int cursor_y = 0;

void vga_put_char(char c, uint8_t color, int x, int y) {
    if (!vga_info.buffer) return;
    int index = y * vga_info.width + x;
    vga_info.buffer[index] = (color << 8) | c;
}

void vga_print(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= vga_info.height)
                cursor_y = vga_info.height - 1;
            continue;
        }
        vga_put_char(str[i], 0x0F, cursor_x, cursor_y);
        cursor_x++;
        if (cursor_x >= vga_info.width) {
            cursor_x = 0;
            cursor_y++;
            if (cursor_y >= vga_info.height)
                cursor_y = vga_info.height - 1;
        }
    }
}

void vga_clear(void) {
    for (int y = 0; y < vga_info.height; y++) {
        for (int x = 0; x < vga_info.width; x++) {
            vga_put_char(' ', 0x0F, x, y);
        }
    }
    cursor_x = 0;
    cursor_y = 0;
}

void vga_init(void) {
    if (!vga_info.buffer) {
        vga_info.width = 80;
        vga_info.height = 25;
        vga_info.buffer = (uint16_t *)0xB8000;
    }
    vga_clear();
}
