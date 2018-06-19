#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"../include/queens.h"

int have_conflict(int row, int col, Cell **table, int size) {
    int i,j;
    for(i = 0; i < row ; i++) {
        if(table[i][col].figure == 'Q')
            return 1;
    }
    //check upper left diagonal
    for(i = row,j = col; i >= 0 && j >= 0; i--,j--) {
        if(table[i][j].figure == 'Q')
            return 1;
    }

    //check upper right diagonal
    for(i = row, j = col; i >= 0 && j < size; i--,j++) {
        if(table[i][j].figure == 'Q')
            return 1;
    }

    return 0;
}

//target is to place one queen in each row so that no one collides with each other
//Here queen_row parameter shows the current we're going to place a Queen
int  backtrack(int queen_row, Cell **table, int size) {
    if(queen_row == size) {
        return 1;
    }

    for(int i = 0; i < size; i++) {
        if(!have_conflict(queen_row, i, table, size)) {
           table[queen_row][i].figure = 'Q';
           if(!backtrack(queen_row + 1, table, size))
              table[queen_row][i].figure = 'E';
           else
             return 1;
        }
    }
    return 0;
}
void place_queens(Cell **table, int size) {
  backtrack(1,table,size);
}
