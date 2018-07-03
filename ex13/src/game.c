#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "filler.h"
#include "my_string.h"

void timeout_reset(struct timeval *timer) {
  timer->tv_sec = 1;
  timer->tv_usec = 0;
}

void start_game(filler_t *filler) {
  req_t      *req;
  pos_t      pos;
  fd_set     rfds, wfds;
  struct     timeval timeout;

  print_log("filler.log", "w", "STARTING !!!!!\n");
  set_nonblocking(0);
  req = create_req();
  while(1) {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);

    switch(filler->status) {
      case 0: FD_SET(0, &rfds); break;
      case 1: FD_SET(1, &wfds); break;
    }

    print_log("filler.log", "a", "Status: %s\n", filler->status ? "writing" : "reading");
    timeout_reset(&timeout);
    select(2, &rfds, &wfds, NULL, &timeout);

    if(FD_ISSET(0, &rfds)) {
      req = read_request(filler);

      if(req) {
        pos = play(req);
        filler->status = 1;
      }
    }

    if(FD_ISSET(1, &wfds)) {
      print_log("filler.log", "a", "Position: (%i;%i)\n", pos.x, pos.y);
      print_pos(pos);

      filler->status = 0;
      string_destroy(filler->current_stream);
      filler->current_stream = NULL;

      destroy_req(req);
      req = NULL;
    }
  }
}

int check_free_space(map_t *map, elem_t *new_elem, pos_t p) {
  for(int i = 0; i < new_elem->h; i++)
    for(int j = 0; j < new_elem->w; j++)
      if(new_elem->array[i][j] == '*') {
        if(i + p.y < map->h && j + p.x < map->w && i + p.y >= 0 && j + p.x >= 0) {
          if(map->array[i + p.y][j + p.x] != '.')
            return -1;
        }
        else
          return -1;
      }
  return 0;
}

int check_connection(map_t *map, elem_t *new_elem, pos_t p, char symbol) {
  for(int i = 0; i < new_elem->h; i++)
    for(int j = 0; j < new_elem->w; j++)
      if(new_elem->array[i][j] != '.') {
        if(i + p.y + 1 < map->h && map->array[i + p.y + 1][j + p.x] == symbol)
          return 0;
        if (i + p.y - 1 >= 0 && map->array[i + p.y - 1][j + p.x] == symbol)
          return 0;
        if (j + p.x + 1 < map->w && map->array[i + p.y][j + p.x + 1] == symbol)
          return 0;
        if (j + p.x - 1 >= 0 && map->array[i + p.y][j + p.x - 1] == symbol)
          return 0;
      }
  return -1;
}
