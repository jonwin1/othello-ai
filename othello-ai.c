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

#include "othello-ai.h"

int
main(void) {
  int playrow, playcol;
  state *s = newstate();
            
  while (true) {
    printboard(s->board);

    char buf[1];

    pos *playable = getplayabletiles(s);

    int index = 0;
    while (playable[index].row != -1) {
      s->board[playable[index].row][playable[index].col] = '*';
      index++;
    }

    printboard(s->board);

    printf("\nEnter position to play (row,col): ");
    // Not safe reading
    scanf("%d,%d", &playrow, &playcol);
    s->board[playrow][playcol] = 'W';
  }

  return EXIT_SUCCESS;
}

void
playerturn(void) {
};

void 
opponentturn(void) {
}

pos *
getplayabletiles(state *s)
{
  pos *playable = calloc(HEIGHT*WIDTH+1, sizeof(pos));
  int index = 0;

  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      if (col < 7) {
        if (s->board[row][col+1] == s->opponent) {
          for (int i = col+1; i <= 7; i++) {
            if (s->board[row][i] == ' ') {
              break;
            } else if (s->board[row][i] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
              break;
            }
          }
        }
      }
      if (col > 0) {
        if (s->board[row][col-1] == s->opponent) {
          for (int i = col-1; i >= 0; i--) {
            if (s->board[row][i] == ' ') {
              break;
            } else if (s->board[row][i] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
              break;
            }
          }
        }
      }
      if (row < 7) {
        if (s->board[row+1][col] == s->opponent) {
          for (int i = row+1; i <= 7; i++) {
            if (s->board[i][col] == ' ') {
              break;
            } else if (s->board[i][col] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
              break;
            }
          }
        }
      }
      if (row > 0) {
        if (s->board[row-1][col] == s->opponent) {
          for (int i = row-1; i >= 0; i--) {
            if (s->board[i][col] == ' ') {
              break;
            } else if (s->board[i][col] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
              break;
            }
          }
        }
      }
      if (row < 7 && col < 7) {
        if (s->board[row+1][col+1] == s->opponent) {
          for (int r = row+1, c = col+1; r <= 7 && c <= 7; r++, c++) {
            if (s->board[r][c] == ' ') {
              break;
            } else if (s->board[r][c] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
              break;
            }
          }
        }
      }
      if (row < 7 && col > 0) {
        if (s->board[row+1][col-1] == s->opponent) {
          for (int r = row+1, c = col-1; r <= 7 && c >= 0; r++, c--) {
            if (s->board[r][c] == ' ') {
              break;
            } else if (s->board[r][c] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
              break;
            }
          }
        }
      }
      if (row > 0 && col < 7) {
        if (s->board[row-1][col+1] == s->opponent) {
          for (int r = row-1, c = col+1; r >= 0 && c <= 7; r--, c++) {
            if (s->board[r][c] == ' ') {
              break;
            } else if (s->board[r][c] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
              break;
            }
          }
        }
      }
      if (row > 0 && col > 0) {
        if (s->board[row-1][col-1] == s->opponent) {
          for (int r = row-1, c = col-1; r >= 0 && c >= 0; r--, c--) {
            if (s->board[r][c] == ' ') {
              break;
            } else if (s->board[r][c] == s->player) {
              playable[index].row = row;
              playable[index].col = col;
              index++;
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

state *
newstate(void)
{
  state *s = malloc(sizeof(state));
  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      s->board[row][col] = ' ';
    }
  }
  s->board[3][4] = 'B';
  s->board[4][3] = 'B';
  s->board[3][3] = 'W';
  s->board[4][4] = 'W';
  s->player = 'W';
  s->opponent = 'B';
  int boardWeights[HEIGHT][WIDTH] = {{ 20, -5, 5, 3, 3, 5,-5, 20 },
                                    { -5,-10, 1, 2, 2, 1,-3, -5 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    { -5,-10, 1, 2, 2, 1,-3, -5 },
                                    { 20, -5, 5, 3, 3, 5,-5, 20 }};
  s->boardWeights = (int **)boardWeights;
  return s;
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
place(state *s, int row, int col) {
  if (s->player == 'W') {
  } else if ( s->player == 'B') {
    s->board[row][col] = '*';
  }
}
