#include <stdio.h>
#include <stdlib.h>
#include "filler.h"
#include <stdarg.h>

void print_pos(pos_t p) {
  dprintf(1, "%d %d", p.x, p.y);
  print_log("filler.log", "a", "Position: (%d;%d)\n", p.x, p.y);
}

void print_log(const char *filename, const char *mode, const char *format, ...) {
  va_list   arg;
  FILE      *log;

  log = fopen(filename, mode);

  va_start(arg, format);
  vfprintf(log, format, arg);
  va_end(arg);

  fclose(log);
}

