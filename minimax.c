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

struct threaddata {
  char board[ROWS][COLUMNS];
  struct pos *playable;
  char player;
  int maxdepth;
  int index;
};

struct pos
findbestmove(char board[ROWS][COLUMNS], char player, int maxdepth)
{
  int index, bestval = -10000, moveval;
  char tempboard[ROWS][COLUMNS];
  struct pos bestmove = {-1,-1};

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

    moveval = minimax(tempboard, maxdepth, false, player, -1000, 1000);
    
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

struct pos
mtfindbestmove(char board[ROWS][COLUMNS], char player, int maxdepth)
{
  int index, bestval = -10000, bufsize = 10;
  struct pos bestmove = {-1,-1};
  pthread_t *threads = calloc(bufsize, sizeof(pthread_t));
  struct pos *playable = getplayabletiles(board, player);
  
  if (playable[0].row == -1) {
    printf("No possible moves, next player\n");
    free(playable);
    free(threads);
    return bestmove;
  }

  index = 0;
  while (playable[index].row != -1) {
    struct threaddata *arg = malloc(sizeof(struct threaddata));
    memcpy(arg->board, board, sizeof(char)*ROWS*COLUMNS);
    arg->playable = playable;
    arg->player = player;
    arg->maxdepth = maxdepth;
    arg->index = index;

    if (index >= bufsize) {
      bufsize += 10;
      threads = realloc(threads, bufsize * sizeof(pthread_t));
    }

    pthread_create(&threads[index], NULL, minimaxthread, arg);
    
    index++;
  }

  for (int i = 0; i < index; i++) {
    void *moveval;
    pthread_join(threads[i], &moveval);
    if (*(int *)moveval > bestval) {
      bestmove.row = playable[i].row;
      bestmove.col = playable[i].col;
      bestval = *(int *)moveval;
    }
    free(moveval);
  }

  free(threads);
  free(playable);
  return bestmove;
}

void *
minimaxthread(void *vargp)
{
  int *moveval = malloc(sizeof(int));
  struct threaddata *arg = vargp;

  arg->board[arg->playable[arg->index].row][arg->playable[arg->index].col] = arg->player;
  struct pos playedtile = {arg->playable[arg->index].row, arg->playable[arg->index].col};
  fliptiles((char (*)[COLUMNS])arg->board, playedtile, arg->player);

  *moveval = minimax((char (*)[COLUMNS])arg->board, arg->maxdepth, false, arg->player, -1000, 1000);
  free(vargp);
  return moveval;
}

int 
minimax(char board[ROWS][COLUMNS], int maxdepth, bool ismax, char player,
        int alpha, int beta)
{
  int index, best, val;
  char opponent;
  char tempboard[ROWS][COLUMNS];
  struct pos *pplayable, *oplayable;

  if (player == 'B') {
    opponent = 'W';
  } else if (player == 'W') {
    opponent = 'B';
  } else {
    fprintf(stderr, "Invalid player char: minimax\n");
    exit(EXIT_FAILURE);
  }

  pplayable = getplayabletiles(board, player);
  oplayable = getplayabletiles(board, opponent);

  if (maxdepth <= 0 || (pplayable[0].row == -1 && oplayable[0].row == -1)) {
    free(pplayable);
    free(oplayable);
    return evaluate(board, player);
  }

  if (ismax) {
    best = -1000;

    if (pplayable[0].row == -1) {
      free(pplayable);
      free(oplayable);
      return best;
    }

    index = 0;
    while (pplayable[index].row != -1) {
      memcpy(tempboard, board, sizeof(tempboard));

      tempboard[pplayable[index].row][pplayable[index].col] = player;
      struct pos playedtile = {pplayable[index].row, pplayable[index].col};
      fliptiles(tempboard, playedtile, player);

      val = minimax(tempboard, --maxdepth, !ismax, opponent, alpha, beta);
      
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
    free(pplayable);
    free(oplayable);
    return best;
  } else {
    best = 1000;

    if (pplayable[0].row == -1) {
      free(pplayable);
      free(oplayable);
      return best;
    }

    index = 0;
    while (pplayable[index].row != -1) {
      memcpy(tempboard, board, sizeof(tempboard));

      tempboard[pplayable[index].row][pplayable[index].col] = player;
      struct pos playedtile = {pplayable[index].row, pplayable[index].col};
      fliptiles(tempboard, playedtile, player);

      val = minimax(tempboard, --maxdepth, !ismax, opponent, alpha, beta);
      
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
    free(pplayable);
    free(oplayable);
    return best;
  }
}

// https://github.com/Roodaki/Minimax-Powered-Othello-Game/blob/master/src/ai_agent.py

int
evaluate(char board[ROWS][COLUMNS], char player)
{
  float tileparityweight = 1.0;
  float mobilityweight = 2.0;
  float cornerweight = 5.0;
  float stabilityweight = 3.0;
  float edgeweight = 2.5;
  
  int tileparity, mobility, corner, stability, edge;
  int ptiles = 0, otiles = 0;
  char opponent;
  struct pos *pplayable, *oplayable;

  if (player == 'B') {
    opponent = 'W';
  } else if (player == 'W') {
    opponent = 'B';
  } else {
    fprintf(stderr, "Invalid player char in evaluate\n");
    exit(EXIT_FAILURE);
  }

  // tileparity
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      if (board[r][c] == player) {
        ptiles++;
      } else if (board[r][c] == opponent) {
        otiles++;
      }
    }
  }
  tileparity = ptiles - otiles;

  // mobility
  pplayable = getplayabletiles(board, player);
  oplayable = getplayabletiles(board, opponent);
  mobility = pplayable - oplayable;
  free(pplayable);
  free(oplayable);

  // corner occupancy
  corner = 0;
  if (board[0][0] == player)
    corner++;
  if (board[0][7] == player)
    corner++;
  if (board[7][0] == player)
    corner++;
  if (board[7][7] == player)
    corner++;

  // stability
  stability = calcstability(board, player);

  // edge occupancy
  edge = 0;
  for (int i = 1; i < 7; i++) {
    if (board[0][i] == player) 
      edge++;
    if (board[7][i] == player) 
      edge++;
    if (board[i][0] == player) 
      edge++;
    if (board[i][7] == player) 
      edge++;
  }

  return tileparity * tileparityweight + mobility * mobilityweight +
         corner * cornerweight + stability * stabilityweight +
         edge * edgeweight;
}

bool
isvalidpos(int row, int col)
{
  return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool
isstable(char board[ROWS][COLUMNS], char player, int row, int col)
{
  int dr, dc;
  int neighbors[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},          {0, 1},
    {1, -1}, {1, 0}, {1, 1},
  };
  
  for (int i = 0; i < 8; i++) {
    dr = neighbors[i][0];
    dc = neighbors[i][1];
    int r = row + dr;
    int c = col + dc;

    if (isvalidpos(r, c) && board[r][c] == player) {
      continue;
    } else {
      return false;
    }
  }

  return true;
}

int
calcstability(char board[ROWS][COLUMNS], char player)
{
  int stablecount = 0;

  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLUMNS; c++) {
      if (board[r][c] == player && isstable(board, player, r, c)) {
        stablecount++;
      }
    }
  }
  return stablecount;
}
