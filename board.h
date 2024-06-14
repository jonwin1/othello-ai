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

#define ROWS 8
#define COLUMNS 8

struct pos {
  int row;
  int col;
};

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
