#include "type_helper.h"

char *uint32_to_str(uint32_t value, char *buffer, int base) {
   if (base < 2 || base > 36) {
       *buffer = '\0';
       return buffer;
   }

   char *ptr = buffer;
   char *low = buffer;

   if (value == 0) {
      *ptr++ = '0';
      *ptr = '\0';
      return buffer;
   }

   while (value > 0) {
      int remainder = value % base;
      *ptr++ = (remainder > 9) ? (remainder - 10) + 'A' : remainder + '0';
      value /= base;
   }

   *ptr-- = '\0';
    while (low < ptr) {
        char temp = *ptr;
        *ptr = *low;
        *low = temp;
        ptr--;
        low++;
    }

    return buffer;
}
