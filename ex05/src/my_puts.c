#include <stdio.h>
int my_putchar(char c) {
  extern long write(int, const char *, unsigned long);
  return write(1, &c, 1);
}
int my_puts(const char *s) {
    while(*s) {
      my_putchar(*s++);
    }
    my_putchar('\n');
    return 0;
}
