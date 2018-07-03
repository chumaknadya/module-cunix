#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include "filler.h"
#include "my_string.h"

int is_finished(stream_t *buff) {
  int 	pos = 0;
  int   elem_pos = 0;
  bool 	first_size = true;
  char 	arr_elem_size[32];
  pos_t el_size;

  while(pos <= (int)buff->size) {
    if(buff->str[pos] > '9' || buff->str[pos] < '0')
      pos++;
    else if (first_size) {
      while(pos <= (int)buff->size && buff->str[pos] != '\n')
        pos++;
      pos++;
      first_size = false;
    }
    else {
      elem_pos = pos;
      while(pos <= (int)buff->size && buff->str[pos] != '\n')
        pos++;
      if(buff->str[pos] != '\n')
        return -1;
      else {
        find_size(arr_elem_size, buff->str, elem_pos);
        el_size = parse_size(arr_elem_size);
        if((int)buff->size <= pos + el_size.x*(el_size.y + 1))
          return -1;
        else return 0;
      }
    }
  }
  return -1;
}

void read_input(filler_t *filler) {
  char buff[BUF_SIZE];

  while(1) {
    memset(buff, '\0', BUF_SIZE);
    read(0, buff, BUF_SIZE - 1);
    if(!strlen(buff))
      break;
    if(filler->current_stream == NULL)
      filler->current_stream = string_create(buff);
    else
      string_append(filler->current_stream, buff);
  }
  print_log("filler_read.log","a","String: %s\n", filler->current_stream->str);
}

req_t *read_request(filler_t *filler) {
  req_t *req = NULL;
  read_input(filler);

  if(is_finished(filler->current_stream) == 0)
    req = parse_all(filler->current_stream->str);
  return req;
}

