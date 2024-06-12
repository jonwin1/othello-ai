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
#include <memory.h>

#define ROWS 8
#define COLUMNS 8
#define MAXDEPTH 200

struct pos {
  int row;
  int col;
};

/*
 * @param board   The game board
 * @param player  Who is playing ('B' or 'W')
 * @return        Number of tiles fliped or -1 if no possible moves
 */
int playerturn(char board[ROWS][COLUMNS], char player);
/* 
 * @param board   The game board
 * @param player  Who is playing ('B' or 'W')
 * @return        Number of tiles fliped or -1 if no possible moves
 */
int botturn(char board[ROWS][COLUMNS], char player);
/* Find the best move for the player
 * @param board   The game board
 * @param player  Who is playing ('B' or 'W')
 * @return        The move to make
 */
struct pos findbestmove(char board[ROWS][COLUMNS], char player);
/*
 *
 */
int 
minimax(char board[ROWS][COLUMNS], int depth, bool ismax, char player,
        int alpha, int beta);
/*
 */
int
evaluate(char board[ROWS][COLUMNS], char player);
/* Flip tiles after placing a tile
 * @param board       The game board
 * @param playedtile  The position of the tile that has been placed
 * @param player      Who is playing ('B' or 'W')
 * @return            Number of tiles fliped
 */
int fliptiles(char board[ROWS][COLUMNS], struct pos playedtile, char player);
/* Get playable tiles
 * @param board     The game board
 * @param player    Who is playing ('B' or 'W')
 * @return          Array of playable positions terminated with (-1, -1)
 */
struct pos *getplayabletiles(char board[ROWS][COLUMNS], char player);
/* Print the board
 * @param board   The game board
 */
void printboard(char board[ROWS][COLUMNS]);

int
main(void) {
  int blacktiles = 2, whitetiles = 2, flipedtiles;
  bool gameover = false;
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
    flipedtiles = botturn(board, 'B');
    if (flipedtiles == 0) {
      fprintf(stderr, "Error: Invalid move placed\n");
      return EXIT_FAILURE;
    } else if (flipedtiles == -1) {
      if (gameover == true) {
        break;
      }
      gameover = true;
    } else {
      blacktiles += flipedtiles + 1;
      whitetiles -= flipedtiles;
      gameover = false;
    }
    printf("Score: %d / %d\n", blacktiles, whitetiles);

    flipedtiles = botturn(board, 'W');
    if (flipedtiles == 0) {
      fprintf(stderr, "Error: Invalid move placed\n");
      return EXIT_FAILURE;
    } else if (flipedtiles == -1) {
      if (gameover == true) {
        break;
      }
      gameover = true;
    } else {
      whitetiles += flipedtiles + 1;
      blacktiles -= flipedtiles;
      gameover = false;
    }
    printf("Score: %d / %d\n", blacktiles, whitetiles);
  }

  if (blacktiles > whitetiles) {
    printf("B Won\n");
  } else if (whitetiles > blacktiles) {
    printf("W Won\n");
  } else {
    printf("Draw\n");
  }

  return EXIT_SUCCESS;
}

int
playerturn(char board[ROWS][COLUMNS], char player)
{
  int playrow, playcol, index;
  struct pos *playable = getplayabletiles(board, player);
  if (playable[0].row == -1) {
    printf("No possible moves, next player\n");
    return -1;
  }

  index = 0;
  while (playable[index].row != -1) {
    board[playable[index].row][playable[index].col] = '*';
    index++;
  }

  printboard(board);

  bool found = false;
  while (!found) {
    printf("Player %c\n", player);
    printf("Enter position to play (row,col): ");
    // WARNING: Not safe reading
    scanf("%d,%d", &playrow, &playcol);
    printf("\n");

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

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      if (board[r][c] == '*') {
       board[r][c] = ' ';
      }
    }
  }

  return fliptiles(board, playedtile, player);
}

int 
botturn(char board[ROWS][COLUMNS], char player)
{
  struct pos bestmove;

  printboard(board);
  printf("Player %c, thinking...\n", player);

  bestmove = findbestmove(board, player);

  if (bestmove.row == -1)
    return -1;

  board[bestmove.row][bestmove.col] = player;

  return fliptiles(board, bestmove, player);
}

struct pos
findbestmove(char board[ROWS][COLUMNS], char player)
{
  int index, bestval = -10000, moveval;
  char opponent;
  char tempboard[ROWS][COLUMNS];
  struct pos bestmove = {-1,-1};

  if (player == 'B') {
    opponent = 'W';
  } else if (player == 'W') {
    opponent = 'B';
  } else {
    fprintf(stderr, "Invalid player char: fliptiles");
    exit(EXIT_FAILURE);
  }

  struct pos *playable = getplayabletiles(board, player);
  if (playable[0].row == -1) {
    printf("No possible moves, next player\n");
    return bestmove;
  }

  index = 0;
  while (playable[index].row != -1) {
    memcpy(tempboard, board, sizeof(tempboard));

    tempboard[playable[index].row][playable[index].col] = player;
    struct pos playedtile = {playable[index].row, playable[index].col};
    fliptiles(tempboard, playedtile, player);

    moveval = minimax(tempboard, 0, false, player, -1000, 1000);
    
    if (moveval > bestval) {
      bestmove.row = playedtile.row;
      bestmove.col = playedtile.col;
      bestval = moveval;
    }
    
    index++;
  }

  return bestmove;
}

int 
minimax(char board[ROWS][COLUMNS], int depth, bool ismax, char player,
        int alpha, int beta)
{
  fprintf(stderr, "minimax at depth %d\n", depth);
  int index, best, val;
  int score = evaluate(board, player);
  char opponent;
  char tempboard[ROWS][COLUMNS];

  if (score == 100) {
    return 100 - depth;
  }

  if (score == -100) {
    return -100;
  }

  if (score == -1) {
    return 0;
  }

  if (depth >= MAXDEPTH) {
    // NOTE: Do somethin here
    int count = 0;
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLUMNS; c++) {
        if (board[r][c] == player) {
          count++;
        }
      }
    }
    return count;
  }

  if (player == 'B') {
    opponent = 'W';
  } else if (player == 'W') {
    opponent = 'B';
  } else {
    fprintf(stderr, "Invalid player char: fliptiles");
    exit(EXIT_FAILURE);
  }

  if (ismax) {
    int best = -1000;

    struct pos *playable = getplayabletiles(board, player);
    if (playable[0].row == -1) {
      printf("No possible moves, next player\n");
      return best;
    }

    index = 0;
    while (playable[index].row != -1) {
      memcpy(tempboard, board, sizeof(tempboard));

      tempboard[playable[index].row][playable[index].col] = player;
      struct pos playedtile = {playable[index].row, playable[index].col};
      fliptiles(tempboard, playedtile, player);

      val = minimax(tempboard, ++depth, !ismax, opponent, alpha, beta);
      
      if (val > best) {
        best = val;
      }
      if (best > alpha) {
        alpha = best;
      }
      if (beta <= alpha) {
        break;
      }
      
      index++;
    }
    return best;
  } else {
    int best = 1000;

    struct pos *playable = getplayabletiles(board, player);
    if (playable[0].row == -1) {
      printf("No possible moves, next player\n");
      return best;
    }

    index = 0;
    while (playable[index].row != -1) {
      memcpy(tempboard, board, sizeof(tempboard));

      tempboard[playable[index].row][playable[index].col] = player;
      struct pos playedtile = {playable[index].row, playable[index].col};
      fliptiles(tempboard, playedtile, player);

      val = minimax(tempboard, ++depth, !ismax, opponent, alpha, beta);
      
      if (val > best)
        best = val;
      if (best > beta)
        beta = best;
      if (beta <= alpha)
        break;
      
      index++;
    }
    return best;
  }
}

int
evaluate(char board[ROWS][COLUMNS], char player)
{
  int blacktiles = 0, whitetiles = 0;
  if (getplayabletiles(board, 'B')[0].row == -1 &&
      getplayabletiles(board, 'W')[0].row == -1) {
    return -1; // gameover
  } else {
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLUMNS; c++) {
        if (board[r][c] == 'B') {
          blacktiles++;
        } else if (board[r][c] == 'W') {
          whitetiles++;
        }
      }
    }
    if (player == 'B') {
      if (blacktiles > whitetiles) {
        return(100);
      } else if (blacktiles < whitetiles) {
        return(-100);
      } else {
        return(0);
      }
    } else {
      if (blacktiles > whitetiles) {
        return(-100);
      } else if (blacktiles < whitetiles) {
        return(00);
      } else {
        return(0);
      }
    }
  }
}

int
fliptiles(char board[ROWS][COLUMNS], struct pos playedtile, char player)
{
  int flipedtiles = 0;
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
          flipedtiles++;
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
          flipedtiles++;
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
          flipedtiles++;
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
          flipedtiles++;
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
          flipedtiles++;
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
          flipedtiles++;
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
          flipedtiles++;
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
          flipedtiles++;
        }
        break;
      }
    }
  }

  return flipedtiles;
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
