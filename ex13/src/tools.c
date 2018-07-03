#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "filler.h"
#include "my_string.h"

int set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void fatal(char *msg) {
  dprintf(2,"Message: %s", msg);
  exit(1);
}

void create_filler(filler_t *filler) {
  filler->current_stream = NULL;
  filler->status = 0;
}

void destroy_filler(filler_t *filler) {
  string_destroy(filler->current_stream);
}

req_t *create_req() {
  req_t *req = (req_t*)malloc(sizeof(req_t));
  return req;
}

void destroy_req(req_t *req) {
  free(req->map.array);
  free(req->elem.array);
  free(req);
  req = NULL;
}

