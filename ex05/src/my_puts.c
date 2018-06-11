#include <stdio.h>
int my_putchar(char c) {
  extern long write(int, const char *, unsigned long);
  return write(1, &c, 1);
}
int my_puts(const char *s) {
    while(*s) {
      if(my_putchar(*s++) == EOF) return EOF;
    }
    if(my_putchar('\n') == EOF) return EOF;
    return 0;
}
