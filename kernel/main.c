void kernel_main(void) {
   const char *str = "TEST";
   char *vga = (char *) 0xb8000;
   for (int i = 0; str[i] != '\0'; i++) {
      vga[i*2] = str[i];
      vga[i*2+1] = 0x0F;
   }
   while (1);
}
