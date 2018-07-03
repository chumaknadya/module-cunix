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

int create_map_array(req_t *req, int m, char *all) {
  req->map.array = (char **) malloc(req->map.h * sizeof(char*));
  print_log("filler_new.log","a","First size: %d %d\n", req->map.h, req->map.w);

  for(int i = 0; i < req->map.h; i++)
	  req->map.array[i] = (char *) malloc((req->map.w + 1) * sizeof(char));

  for(int i = 0; i < req->map.h; i++)
	  for(int j = 0; j < req->map.w + 1; j++) {
		  *(*(req->map.array + i) + j) = *(all + m);
		  print_log("filler_new.log","a","%c", *(*(req->map.array + i)+j));
  	  m++;
    }

  return m;
}

void create_element_array(req_t *req, int m, char *all) {
  req->elem.array = (char **) malloc(req->elem.h * sizeof(char*));
  print_log("filler_new.log","a","second size: %d %d\n", req->elem.h, req->elem.w);

  for(int i = 0; i < req->elem.h; i++)
	  req->elem.array[i] = (char *) malloc((req->elem.w+1) * sizeof(char));

  for(int i = 0; i < req->elem.h; i++)
    for(int j = 0; j < req->elem.w + 1; j++) {
  	  *(*(req->elem.array + i) + j) = *(all + m);
  	  print_log("filler_new.log","a","%c", *(*(req->elem.array + i) + j));
  	  m++;
    }
}

int get_map_size(char *pos1, char *all, int m) {
  m += 2;
  int n = 0;
  do {
    *(pos1 + n) = *(all + m);
    m++;
    n++;
  } while(*(all + m - 1) != '\n');

  return m;
}

req_t *parse_all(char *all) {
  req_t 	*req;
  pos_t 	pos;
  int 	  m = 0, n = 0;
  char *pos1 = (char *) malloc(BUF_SIZE * sizeof(char));
  char *pos2 = (char *) malloc(BUF_SIZE * sizeof(char));

  req = create_req();
  memset(pos1, '\0', BUF_SIZE);
  memset(pos2, '\0', BUF_SIZE);

  req->symbol = *all;
  print_log("filler_new.log","a","------------Start parse----\n");
  print_log("filler_new.log","a","My symbol %c\n",req->symbol);

  m = get_map_size(pos1, all, m);
  pos = parse_size(pos1);
  req->map.h = pos.x;
  req->map.w = pos.y;

  m = create_map_array(req, m, all);
  do {
    *(pos2 + n) = *(all + m);
    m++;
    n++;
  } while(*(all + m - 1) != '\n');
  pos = parse_size(pos2);
  req->elem.h = pos.x;
  req->elem.w = pos.y;

  create_element_array(req, m, all);
  print_log("filler_new.log","a", "----------End-----");

  free(pos1);
  free(pos2);

  return req;
}

pos_t parse_size(char *answer) {
  int  i, size;
  char *height, *width;
  pos_t pos;

  size = strlen(answer);
  height = malloc(size);
  width = malloc(size);

  memset(height, '\0', size);
  memset(width, '\0', size);

  for(i = 0; i < size && answer[i] != ' '; i++);

  height = strncpy(height, answer, i);
  width = strcpy(width, answer + i + 1);
  pos.x = atoi(height);
  pos.y = atoi(width);

  print_log("filler.log","a","Position %d %d", pos.x, pos.y);

  free(height);
  free(width);

  return pos;
}

int	find_size(char res[32], char *buf, int start_pos) {
  int i = 0;
  do {
    res[i] = buf[start_pos];
    start_pos++;
    i++;
  } while (buf[start_pos-1] != '\n');

  res[i] = '\0';
  start_pos++;
  return i - 1;
}

