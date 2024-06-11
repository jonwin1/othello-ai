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

#define ROWS 8
#define COLUMNS 8

struct pos {
  int row;
  int col;
};

void playerturn(char board[ROWS][COLUMNS], char player);
void botturn(char board[ROWS][COLUMNS]);
void fliptiles(char board[ROWS][COLUMNS], struct pos playedtile, char player);
struct pos *getplayabletiles(char board[ROWS][COLUMNS], char player);
void printboard(char board[ROWS][COLUMNS]);

int
main(void) {
  char board[ROWS][COLUMNS] = {{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
                              { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
                              { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
                              { ' ', ' ', ' ', 'W', 'B', ' ', ' ', ' ' },
                              { ' ', ' ', ' ', 'B', 'W', ' ', ' ', ' ' },
                              { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
                              { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
                              { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }};
  int boardWeights[ROWS][COLUMNS] = {{ 20, -5, 5, 3, 3, 5,-5, 20 },
                                    { -5,-10, 1, 2, 2, 1,-3, -5 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    { -5,-10, 1, 2, 2, 1,-3, -5 },
                                    { 20, -5, 5, 3, 3, 5,-5, 20 }};
            
  while (true) {
    playerturn(board, 'B');
    playerturn(board, 'W');
  }

  return EXIT_SUCCESS;
}

void
playerturn(char board[ROWS][COLUMNS], char player)
{
  int playrow, playcol, index;
  struct pos *playable = getplayabletiles(board, player);

  index = 0;
  while (playable[index].row != -1) {
    board[playable[index].row][playable[index].col] = '*';
    index++;
  }

  printboard(board);

  bool found = false;
  while (!found) {
    printf("\nEnter position to play (row,col): ");
    // WARNING: Not safe reading
    scanf("%d,%d", &playrow, &playcol);

    index = 0;
    while (playable[index].row != -1) {
      if (playable[index].row == playrow && playable[index].col == playcol) {
        found = true;
        break;
      }
      index++;
    }
  }

  board[playrow][playcol] = player;
  struct pos playedtile = {playrow, playcol};
  fliptiles(board, playedtile, player);

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      if (board[r][c] == '*') {
       board[r][c] = ' ';
      }
    }
  }

}

void 
botturn(char board[ROWS][COLUMNS])
{
}

void
fliptiles(char board[ROWS][COLUMNS], struct pos playedtile, char player)
{
  char opponent;
  if (player == 'B') {
    opponent = 'W';
  } else if (player == 'W') {
    opponent = 'B';
  } else {
    fprintf(stderr, "Invalid player char: fliptiles");
    exit(EXIT_FAILURE);
  }

 if (playedtile.row > 1 && board[playedtile.row-1][playedtile.col] == opponent) {
    for (int i = playedtile.row-2; i >= 0; i--) {
      if (board[i][playedtile.col] == ' ') {
        break;
      } else if (board[i][playedtile.col] == player) {
        for (int j = playedtile.row-1; j > i; j--) {
          board[j][playedtile.col] = player;
        }
        break;
      }
    }
  }

  if (playedtile.col > 1 && board[playedtile.row][playedtile.col-1] == opponent) {
    for (int i = playedtile.col-2; i >= 0; i--) {
      if (board[playedtile.row][i] == ' ') {
        break;
      } else if (board[playedtile.row][i] == player) {
        for (int j = playedtile.col-1; j > i; j--) {
          board[playedtile.row][j] = player;
        }
        break;
      }
    }
  }

  if (playedtile.row < 6 && board[playedtile.row+1][playedtile.col] == opponent) {
    for (int i = playedtile.row+2; i <= 7; i++) {
      if (board[i][playedtile.col] == ' ') {
        break;
      } else if (board[i][playedtile.col] == player) {
        for (int j = playedtile.row+1; j < i; j++) {
          board[j][playedtile.col] = player;
        }
        break;
      }
    }
  }

  if (playedtile.col < 6 && board[playedtile.row][playedtile.col+1] == opponent) {
    for (int i = playedtile.col+2; i <= 7; i++) {
      if (board[playedtile.row][i] == ' ') {
        break;
      } else if (board[playedtile.row][i] == player) {
        for (int j = playedtile.col+1; j < i; j++) {
          board[playedtile.row][j] = player;
        }
        break;
      }
    }
  }

  if (playedtile.row > 1 && playedtile.col > 1 && board[playedtile.row-1][playedtile.col-1] == opponent) {
    for (int r = playedtile.row-2, c = playedtile.col-2; r >= 0 && c >= 0; r--, c--) {
      if (board[r][c] == ' ') {
        break;
      } else if (board[r][c] == player) {
        for (int i = playedtile.row-1, j = playedtile.col-1; i > r && j > c; i--, j--) {
          board[i][j] = player;
        }
        break;
      }
    }
  }

  if (playedtile.row > 1 && playedtile.col < 6 && board[playedtile.row-1][playedtile.col+1] == opponent) {
    for (int r = playedtile.row-2, c = playedtile.col+2; r >= 0 && c <= 7; r--, c++) {
      if (board[r][c] == ' ') {
        break;
      } else if (board[r][c] == player) {
        for (int i = playedtile.row-1, j = playedtile.col+1; i > r && j < c; i--, j++) {
          board[i][j] = player;
        }
        break;
      }
    }
  }

  if (playedtile.row < 6 && playedtile.col > 1 && board[playedtile.row+1][playedtile.col-1] == opponent) {
    for (int r = playedtile.row+2, c = playedtile.col-2; r <= 7 && c >= 0; r++, c--) {
      if (board[r][c] == ' ') {
        break;
      } else if (board[r][c] == player) {
        for (int i = playedtile.row+1, j = playedtile.col-1; i < r && j > c; i++, j--) {
          board[i][j] = player;
        }
        break;
      }
    }
  }

  if (playedtile.row < 6 && playedtile.col < 6 && board[playedtile.row+1][playedtile.col+1] == opponent) {
    for (int r = playedtile.row+2, c = playedtile.col+2; r <= 7 && c <= 7; r++, c++) {
      if (board[r][c] == ' ') {
        break;
      } else if (board[r][c] == player) {
        for (int i = playedtile.row+1, j = playedtile.col+1; i < r && j < c; i++, j++) {
          board[i][j] = player;
        }
        break;
      }
    }
  }
}

// OPTIMIZE: adds duplicates
struct pos *
getplayabletiles(char board[ROWS][COLUMNS], char player)
{
  struct pos *playable = calloc(ROWS*COLUMNS-4+1, sizeof(struct pos));
  int index = 0;

  char opponent;
  if (player == 'B') {
    opponent = 'W';
  } else if (player == 'W') {
    opponent = 'B';
  } else {
    fprintf(stderr, "Invalid player char: getplayabletiles");
    exit(EXIT_FAILURE);
  }

  for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLUMNS; col++) {
      if (board[row][col] == player) {

        if (row > 1 && board[row-1][col] == opponent) {
          for (int i = row-2; i >= 0; i--) {
            if (board[i][col] == ' ') {
              playable[index].row = i;
              playable[index].col = col;
              index++;
              break;
            } else if (board[i][col] == player) {
              break;
            }
          }
        }

        if (col > 1 && board[row][col-1] == opponent) {
          for (int i = col-2; i >= 0; i--) {
            if (board[row][i] == ' ') {
              playable[index].row = row;
              playable[index].col = i;
              index++;
              break;
            } else if (board[row][i] == player) {
              break;
            }
          }
        }

        if (row < 6 && board[row+1][col] == opponent) {
          for (int i = row+2; i <= 7; i++) {
            if (board[i][col] == ' ') {
              playable[index].row = i;
              playable[index].col = col;
              index++;
              break;
            } else if (board[i][col] == player) {
              break;
            }
          }
        }

        if (col < 6 && board[row][col+1] == opponent) {
          for (int i = col+2; i <= 7; i++) {
            if (board[row][i] == ' ') {
              playable[index].row = row;
              playable[index].col = i;
              index++;
              break;
            } else if (board[row][i] == player) {
              break;
            }
          }
        }

        if (row > 1 && col > 1 && board[row-1][col-1] == opponent) {
          for (int r = row-2, c = col-2; r >= 0 && c >= 0; r--, c--) {
            if (board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (board[r][c] == player) {
              break;
            }
          }
        }

        if (row > 1 && col < 6 && board[row-1][col+1] == opponent) {
          for (int r = row-2, c = col+2; r >= 0 && c <= 7; r--, c++) {
            if (board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (board[r][c] == player) {
              break;
            }
          }
        }

        if (row < 6 && col > 1 && board[row+1][col-1] == opponent) {
          for (int r = row+2, c = col-2; r <= 7 && c >= 0; r++, c--) {
            if (board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (board[r][c] == player) {
              break;
            }
          }
        }

        if (row < 6 && col < 6 && board[row+1][col+1] == opponent) {
          for (int r = row+2, c = col+2; r <= 7 && c <= 7; r++, c++) {
            if (board[r][c] == ' ') {
              playable[index].row = r;
              playable[index].col = c;
              index++;
              break;
            } else if (board[r][c] == player) {
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

void
printboard(char board[ROWS][COLUMNS])
{
  printf("    0   1   2   3   4   5   6   7\n");
  for (int row = 0; row < ROWS; row++) {
    printf("  ---------------------------------\n");
    printf("%d ", row);
    for (int col = 0; col < COLUMNS; col++) {
       printf("| %c ", board[row][col]);
    }
    printf("|\n");
  }
  printf("  ---------------------------------\n");
}
