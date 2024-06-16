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

#include "minimax.h"

struct pos
findbestmove(char board[ROWS][COLUMNS], char player)
{
  int index, bestval = -10000, moveval;
  char tempboard[ROWS][COLUMNS];
  struct pos bestmove = {-1,-1};
  int boardWeights[ROWS][COLUMNS] = {{ 20, -5, 5, 3, 3, 5,-5, 20 },
                                    { -5,-10, 1, 2, 2, 1,-10, -5 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    { -5,-10, 1, 2, 2, 1,-10, -5 },
                                    { 20, -5, 5, 3, 3, 5,-5, 20 }};

  struct pos *playable = getplayabletiles(board, player);
  if (playable[0].row == -1) {
    printf("No possible moves, next player\n");
    free(playable);
    return bestmove;
  }

  index = 0;
  while (playable[index].row != -1) {
    memcpy(tempboard, board, sizeof(tempboard));

    tempboard[playable[index].row][playable[index].col] = player;
    struct pos playedtile = {playable[index].row, playable[index].col};
    fliptiles(tempboard, playedtile, player);

    moveval = minimax(tempboard, 0, false, player, -1000, 1000) + boardWeights[playedtile.row][playedtile.col];
    
    if (moveval > bestval) {
      bestmove.row = playedtile.row;
      bestmove.col = playedtile.col;
      bestval = moveval;
    }
    
    index++;
  }

  free(playable);
  return bestmove;
}

int 
minimax(char board[ROWS][COLUMNS], int depth, bool ismax, char player,
        int alpha, int beta)
{
  int index, best, val, score;
  char opponent;
  char tempboard[ROWS][COLUMNS];
  int boardWeights[ROWS][COLUMNS] = {{ 20, -5, 5, 3, 3, 5,-5, 20 },
                                    { -5,-10, 1, 2, 2, 1,-10, -5 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    { -5,-10, 1, 2, 2, 1,-10, -5 },
                                    { 20, -5, 5, 3, 3, 5,-5, 20 }};

  if (player == 'B') {
    opponent = 'W';
  } else if (player == 'W') {
    opponent = 'B';
  } else {
    fprintf(stderr, "Invalid player char: fliptiles\n");
    exit(EXIT_FAILURE);
  }

  score = evaluate(board, player);

  if (score == 100) {
    // player has won
    printf("player has won\n");
    return 100 - depth;
  }

  if (score == -100) {
    // opponent has won
    printf("opponent has won\n");
    return -100;
  }

  if (score == 0) {
    // draw
    printf("draw\n");
    return 0;
  }

  if (depth >= MAXDEPTH) {
    // NOTE: Improve
    int count = 0;
    for (int r = 0; r < ROWS; r++) {
      for (int c = 0; c < COLUMNS; c++) {
        if (board[r][c] == player) {
          count++;
        }
        if (board[r][c] == opponent) {
          count--;
        }
      }
    }
    return count;
  }

  if (ismax) {
    int best = -1000;

    struct pos *playable = getplayabletiles(board, player);
    if (playable[0].row == -1) {
      printf("No possible moves, next player\n");
      free(playable);
      return best;
    }

    index = 0;
    while (playable[index].row != -1) {
      memcpy(tempboard, board, sizeof(tempboard));

      tempboard[playable[index].row][playable[index].col] = player;
      struct pos playedtile = {playable[index].row, playable[index].col};
      fliptiles(tempboard, playedtile, player);

      val = minimax(tempboard, ++depth, !ismax, opponent, alpha, beta) + boardWeights[playedtile.row][playedtile.col];
      
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
    free(playable);
    return best;
  } else {
    int best = 1000;

    struct pos *playable = getplayabletiles(board, player);
    if (playable[0].row == -1) {
      printf("No possible moves, next player\n");
      free(playable);
      return best;
    }

    index = 0;
    while (playable[index].row != -1) {
      memcpy(tempboard, board, sizeof(tempboard));

      tempboard[playable[index].row][playable[index].col] = player;
      struct pos playedtile = {playable[index].row, playable[index].col};
      fliptiles(tempboard, playedtile, player);

      val = minimax(tempboard, ++depth, !ismax, opponent, alpha, beta) - boardWeights[playedtile.row][playedtile.col];
      
      if (val < best) {
        best = val;
      }
      if (best < beta) {
        beta = best;
      }
      if (beta <= alpha) {
        break;
      }
      
      index++;
    }
    free(playable);
    return best;
  }
}

int
evaluate(char board[ROWS][COLUMNS], char player)
{
  int blacktiles = 0, whitetiles = 0;
  struct pos *bplayable, *wplayable;
  int boardWeights[ROWS][COLUMNS] = {{ 20, -5, 5, 3, 3, 5,-5, 20 },
                                    { -5,-10, 1, 2, 2, 1,-10, -5 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  3,  2, 2, 1, 1, 2, 2,  3 },
                                    {  5,  1, 3, 2, 2, 3, 1,  5 },
                                    { -5,-10, 1, 2, 2, 1,-10, -5 },
                                    { 20, -5, 5, 3, 3, 5,-5, 20 }};

  bplayable = getplayabletiles(board, 'B');
  wplayable = getplayabletiles(board, 'W');

  if(bplayable[0].row != -1 && wplayable[0].row != -1) {
    free(bplayable);
    free(wplayable);
    // TODO: value if game not over
    return -1; // No winner yet
  } else {
    free(bplayable);
    free(wplayable);
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
        return(100);
      } else {
        return(0);
      }
    }
  }
}
