#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

int my_atoi(const char *nptr) {
    int acum = 0;
    int factor = 1;

    if(*nptr == '-') {
        factor = -1;
        nptr++;
    }

    while((*nptr >= '0')&&(*nptr <= '9')) {
      acum = acum * 10;
      acum = acum + (*nptr - 48);
      nptr++;
    }
    return (factor * acum);
}
int is_digit (const char *c) {
    return  (*c >= '0') && (*c <= '9');
}
int check_format(const char *c,char symb) {
  return *c == symb;
}
unsigned int my_strlen(char *str) {
  int i = 0;
  for(i = 0;*(str + i) != '\0'; i++);
  return i;
}

char* my_itoa(int nmb) {
  char *buffer = malloc(16 * sizeof(char));
  char *ptr = buffer;
  int i = 1;
  int n;

  if (nmb < 0) {
    nmb = -1 * nmb;
    *buffer = '-';
    buffer++;
  }

  n = nmb;
  while (n > 9) {
    n /= 10;
    i *= 10;
  }

  while (i > 0) {
    n = nmb / i;
    *buffer = '0' + n;
    buffer++;
    nmb -= i * n;
    i /= 10;
  }
  *buffer = '\0';

  return ptr;
}
char *copy_st(char *str, char*st){
  while(*st != '\0') {
    *str = *st;
    str++;
    st++;
  }
  return str;
}
char *get_str(const char* format, char *st,char *padd_str, char *str, char *res_str,
              char *padd_str_init,int padd,unsigned char padd_symb,va_list arg_list) {
  while(*format != '\0') {
    if(check_format(format,'%')) {
      padd = 0;
      padd_symb = 0;
      padd_str = padd_str_init;
      format++;
      while(!check_format(format,' ') && !check_format(format,'\0')) {
        if(check_format(format,'%')) {
          *str = '%';
          str++;
          format++;
          break;
        }
        if(check_format(format,'0')) {
          padd_symb = '0';
          format++;
        }
        while(is_digit(format)) {
          *padd_str = *format;
          padd_str++;
          format++;
        }
        *padd_str = '\0';
        padd = my_atoi(padd_str_init);
        if(check_format(format,'s'))
          st = va_arg(arg_list, char *);
        if(check_format(format,'d'))
          st = my_itoa(va_arg(arg_list, int));
        padd -= my_strlen(st);
        while(padd-- > 0) *str++ = (padd_symb)? padd_symb:' ';
        str = copy_st(str,st);
        format++;
        break;
      }
      continue;
    }
    *str = *format;
    str++;
    format++;
  }
  *str = '\0';
  return str;
}

int my_printf(const char* format, ...) {
  extern long write(int, const char*, unsigned int);
  va_list arg_list;
  char *st, *padd_str, *str,*res_str, *padd_str_init;
  int padd;
  unsigned char padd_symb;
  str = malloc(256 * sizeof(char));
  padd_str_init = malloc(20 * sizeof(char));
  res_str = str;
  va_start(arg_list, format);
  str = get_str(format,st,padd_str,str,res_str,padd_str_init,padd,padd_symb,arg_list);
  write(1,res_str, strlen(res_str));
  va_end(arg_list);
  free(padd_str_init);
  free(res_str);
  return 0;
}
