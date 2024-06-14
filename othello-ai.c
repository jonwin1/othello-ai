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
    flipedtiles = playerturn(board, 'B');
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
    free(playable);
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
  free(playable);
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

  printf("Playing %d,%d\n", bestmove.row, bestmove.col);
  board[bestmove.row][bestmove.col] = player;

  return fliptiles(board, bestmove, player);
}


