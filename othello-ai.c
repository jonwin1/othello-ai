/* othello-ai - A othello playing ai using the minmax algorithm.
 * Copyright (C) 2024  Jonatan Wincent <jonatan.wincent@proton.me>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 8
#define HEIGHT 8

struct gamestate {
  char board[HEIGHT][WIDTH];
  int **boardWeights;
  char player;
  char opponent;
};

struct pos {
  int row;
  int col;
};

void playerturn(void);
void opponentturn(void);
struct pos *getplayabletiles(struct gamestate *s);
struct gamestate *initgamestate(void);
void printboard(char board[HEIGHT][WIDTH]);
void place(struct gamestate *s, int row, int col);

int
main(void) {
  int playrow, playcol;
  struct gamestate *gs = initgamestate();
            
  while (true) {
    struct pos *playable = getplayabletiles(gs);

    int index = 0;
    while (playable[index].row != -1) {
      gs->board[playable[index].row][playable[index].col] = '*';
      index++;
    }

    printboard(gs->board);

    printf("\nEnter position to play (row,col): ");
    // WARNING: Not safe reading
    scanf("%d,%d", &playrow, &playcol);
    gs->board[playrow][playcol] = 'W';

    for (int r = 0; r < HEIGHT; r++) {
      for (int c = 0; c < WIDTH; c++) {
        if (gs->board[r][c] == '*') {
         gs->board[r][c] = ' ';
        }
      }
    }
  }

  return EXIT_SUCCESS;
}

void
playerturn(void)
{
};

void 
opponentturn(void)
{
}

struct pos *
getplayabletiles(struct gamestate *gs)
{
  struct pos *playable = calloc(HEIGHT*WIDTH-4+1, sizeof(struct pos));
  int index = 0;

  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      if (gs->board[row][col] == gs->player) {

        if (row > 1 && gs->board[row-1][col] == gs->opponent) {
          for (int i = row-2; i >= 0; i--) {
            if (gs->board[i][col] == ' ') {
              playable[index].row = i;
              playable[index].col = col;
              index++;
              break;
            } else if (gs->board[i][col] == gs->player) {
              break;
            }
          }
        }

        if (col > 1 && gs->board[row][col-1] == gs->opponent) {
          for (int i = col-2; i >= 0; i--) {
            if (gs->board[row][i] == ' ') {
              playable[index].row = row;
              playable[index].col = i;
              index++;
              break;
            } else if (gs->board[row][i] == gs->player) {
              break;
            }
          }
        }

        if (row < 6 && gs->board[row+1][col] == gs->opponent) {
          for (int i = row+2; i <= 7; i++) {
            if (gs->board[i][col] == ' ') {
              playable[index].row = i;
              playable[index].col = col;
              index++;
              break;
            } else if (gs->board[i][col] == gs->player) {
              break;
            }
          }
        }

        if (col < 6 && gs->board[row][col+1] == gs->opponent) {
          for (int i = col+2; i <= 7; i++) {
            if (gs->board[row][i] == ' ') {
              playable[index].row = row;
              playable[index].col = i;
              index++;
              break;
            } else if (gs->board[row][i] == gs->player) {
              break;
            }
          }
        }

        if (row > 1 && col > 1 && gs->board[row-1][col-1] == gs->opponent) {
          for (int r = row-2, c = col-2; r >= 0 && c >= 0; r--, c--) {
            if (gs->board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (gs->board[r][c] == gs->player) {
              break;
            }
          }
        }

        if (row > 1 && col < 6 && gs->board[row-1][col+1] == gs->opponent) {
          for (int r = row-2, c = col+2; r >= 0 && c <= 0; r--, c++) {
            if (gs->board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (gs->board[r][c] == gs->player) {
              break;
            }
          }
        }

        if (row < 6 && col > 1 && gs->board[row+1][col-1] == gs->opponent) {
          for (int r = row+2, c = col-2; r <= 0 && c >= 0; r++, c--) {
            if (gs->board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (gs->board[r][c] == gs->player) {
              break;
            }
          }
        }

        if (row < 6 && col < 6 && gs->board[row+1][col+1] == gs->opponent) {
          for (int r = row+2, c = col+2; r <= 0 && c <= 0; r++, c++) {
            if (gs->board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (gs->board[r][c] == gs->player) {
              break;
            }
          }
        }

      }
    }
  }

  playable[index].row = -1;
  playable[index].col = -1;
  return playable;
}

struct gamestate *
initgamestate(void)
{
  struct gamestate *gs = malloc(sizeof(struct gamestate));
  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      gs->board[row][col] = ' ';
    }
  }
  gs->board[3][4] = 'B';
  gs->board[4][3] = 'B';
  gs->board[3][3] = 'W';
  gs->board[4][4] = 'W';
  gs->player = 'W';
  gs->opponent = 'B';
  int boardWeights[HEIGHT][WIDTH] = {{ 20, -5, 5, 3, 3, 5,-5, 20 },
                                    { -5,-10, 1, 2, 2, 1,-3, -5 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    { -5,-10, 1, 2, 2, 1,-3, -5 },
                                    { 20, -5, 5, 3, 3, 5,-5, 20 }};
  gs->boardWeights = (int **)boardWeights;
  return gs;
}

void
printboard(char board[HEIGHT][WIDTH])
{
  printf("    0   1   2   3   4   5   6   7\n");
  for (int row = 0; row < HEIGHT; row++) {
    printf("  ---------------------------------\n");
    printf("%d ", row);
    for (int col = 0; col < WIDTH; col++) {
       printf("| %c ", board[row][col]);
    }
    printf("|\n");
  }
  printf("  ---------------------------------\n");
}

void
place(struct gamestate *gs, int row, int col) {
  if (gs->player == 'W') {
  } else if ( gs->player == 'B') {
    gs->board[row][col] = '*';
  }
}
