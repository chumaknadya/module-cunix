#include "filler.h"
#include "my_string.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

stream_t *string_init() {
  stream_t 	*str;
  str = (stream_t*) malloc(sizeof(stream_t));
  str->limit = 64;
  str->str = malloc(str->limit * sizeof(char));
  str->size = 0;
  return str;
}

void string_add(stream_t *s, char* str) {
  int len;
  len = strlen(str);

  for(int i = 0; i < len; i++)
    s->str[s->size + i] = str[i];
  s->size += len;
  s->str[s->size] = '\0';
}

stream_t *string_create(char *str) {
  stream_t	*s;
  s = string_init();
  string_append(s,str);
  return s;
}

void string_append(stream_t *ptr, char *str) {
  int len;
  len = strlen(str);

  if(ptr->size + len >= ptr->limit) {
    ptr->limit = ptr->limit + len + 1;
    ptr->str = realloc(ptr->str, ptr->limit * sizeof(char));
    ptr->str[ptr->size + len] = '\0';
  }
  string_add(ptr, str);
}

void string_destroy(stream_t *str) {
  free(str->str);
  free(str);
  str = NULL;
}

