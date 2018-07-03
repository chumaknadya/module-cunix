#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include "filler.h"

int my_pos_x = -1;
int my_pos_y = -1;
int enemy_pos_x = -1;
int enemy_pos_y = -1;

pos_t find_enemy_pos(req_t *core) {
  pos_t enemy_pos;
  enemy_pos.x = 0;
  enemy_pos.y = 0;
  for (int i = 0; i < core->map.h; i++) {
    for(int j = 0; j < core->map.h; j++) {
      if(core->map.array[i][j] != '.' && core->map.array[i][j] != core->symbol) {
        enemy_pos.x = i;
        enemy_pos.y = j;
        return enemy_pos;
      }
    }
  }
  return enemy_pos;
}

pos_t find_my_pos(req_t *core) {
  pos_t my_pos;
  my_pos.x = 0;
  my_pos.y = 0;
  int size = core->map.h;
  for (int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      if(core->map.array[i][j] == core->symbol) {
        my_pos.x = i;
        my_pos.y = j;
        return my_pos;
      }
    }
  }
  return my_pos;
}

pos_t find_center_pos(req_t *core) {
  pos_t center_pos;
  int size = core->map.h;
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size;j++) {
      if(core->map.array[i][j] == core->symbol) {
        if( i < (3*size)/4 && i > (size/4)) {
          center_pos.x = i;
          center_pos.y = j;
          return center_pos;
        }
      }
    }
  }
  pos_t my_pos;
  my_pos.x = my_pos_x;
  my_pos.y = my_pos_y;
  return my_pos;
}

int check_top_corner(map_t map, int size, char symbol) {
  for (int j = 0; j < size; j++) {
    if(map.array[0][j] == symbol)
      return 1;
  }
  return 0;
}

int check_right_corner(map_t map, int size, char symbol) {
  for (int i = 0; i < size; i++) {
    if(map.array[i][size-1] == symbol)
      return 1;
  }
  return 0;
}

int check_bottom_corner(map_t map, int size, char symbol) {
  for (int j = 0; j < size; j++) {
    if(map.array[size - 1][j] == symbol)
      return 1;
  }
  return 0;
}

int check_left_corner(map_t map, int size, char symbol) {
  for (int i = 0; i < size; i++) {
    if(map.array[i][0] == symbol)
      return 1;
  }
  return 0;
}

int check_space(req_t *core, pos_t res) {
  if(!check_free_space(&(core->map), &(core->elem), res) &&
      !check_connection(&(core->map), &(core->elem), res, core->symbol))
    return 1;
  return 0;
}

pos_t central_diagonal_up(int h, req_t *core, pos_t res) {
  pos_t center_pos = find_center_pos(core);
  // up
  for (int j = h  - 1; j >= 0; j--) {
    for (int i = 0; i <= center_pos.x; i++) {
      res.x = j;
      res.y = i;
      if(core->map.array[i][j] != core->symbol ||
          core->map.array[i][j] != '.') {
        for (int i = 0; i < h; i++) {
          for (int j = 0; j < h; j++) {
            res.x = j;
            res.y = i;
            if(check_space(core, res) == 1)
              return res;
          }
        }
      }
      if(check_space(core, res) == 1) {
        return res;
      }
    }
  }
  return res;
}

pos_t central_diagonal_down(int h, req_t *core, pos_t res) {
  pos_t center_pos = find_center_pos(core);
  // down
  for (int j = h - 1; j > 0; j--) {
    for (int i = h  - 1; i >= center_pos.x; i--) {
      res.x = j;
      res.y = i;
      if(core->map.array[i][j] != core->symbol ||
          core->map.array[i][j] != '.') {
        for (int i = 0; i < h; i++) {
          for (int j = 0; j < h; j++) {
            res.x = j;
            res.y = i;
            if(check_space(core, res) == 1)
              return res;
          }
        }
      }
      if(check_space(core,res) == 1) {
        return res;
      }
    }
  }
  if(check_space(core,res) == 1)
    return res;

  return res;
}
pos_t left_diagonal(int h,req_t *core, pos_t res) {
  // diaginal '\'
  for(int j =  h - 1; j >= 0; j--) {
    for(int i =  h - 1; i >= 0; i--) {
      res.x = j;
      res.y = i;
      if(check_bottom_corner(core->map,h,core->symbol) == 1 ||
          check_right_corner(core->map,h,core->symbol) == 1 ) {
        for(int j = 0; j < h; j++) {
          for(int i = 0; i < h; i++) {
            res.x = j;
            res.y = i;
            if(check_left_corner(core->map,h,core->symbol) == 1 ||
                check_top_corner(core->map,h,core->symbol) == 1 ) {
              res = central_diagonal_up(h, core, res);
            }
            if(check_space(core,res) == 1)
              return res;
          }
        }
      }
      if(check_space(core,res) == 1)
        return res;
    }
  }
  return res;
}

pos_t right_diagonal(int h,req_t *core, pos_t res) {
  //  diagonal '/'
  for(int j = h-1; j >= 0; j--) {
    for(int i = 0; i < h ; i++) {
      res.x = j;
      res.y = i;
      if(check_top_corner(core->map,h,core->symbol) == 1 ||
          check_right_corner(core->map,h,core->symbol) == 1 ) {
        for(int j = 0; j < h; j ++) {
          for(int i = h-1; i >= 0; i--) {
            res.x = j;
            res.y = i;
            if(check_left_corner(core->map,h,core->symbol) == 1 ||
                check_bottom_corner(core->map,h,core->symbol) == 1 ) {
              res = central_diagonal_down(h, core, res);
            }
            if(check_space(core,res) == 1)
              return res;
          }
        }
      }
      if(check_space(core,res) == 1)
        return res;
    }
  }
  return res;
}

pos_t play(req_t *core) {
  pos_t res;
  int h = core->map.h;

  if (my_pos_x == -1 && my_pos_y == -1 && enemy_pos_x == -1 && enemy_pos_y == -1) {
    pos_t pos = find_my_pos(core);
    pos_t enemy_pos = find_enemy_pos(core);
    enemy_pos_x = enemy_pos.x;
    enemy_pos_y = enemy_pos.y;
    my_pos_x = pos.x;
    my_pos_y =  pos.y;
  }

  if((h - my_pos_x) <  my_pos_x)
    // diagonal '\'
    res = left_diagonal(h, core, res);
  else
    //  diagonal '/'
    res = right_diagonal(h, core, res);
  for(int i = 0; i < h; i++) {
    for(int j = 0; j < h;j++) {
      res.x = j;
      res.y = i;
      if(check_space(core,res) == 1)
        return res;
    }
  }
  return res;
}

