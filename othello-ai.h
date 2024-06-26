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
#include "minimax.h"

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
int botturn(char board[ROWS][COLUMNS], char player, int maxdepth);

int mtbotturn(char board[ROWS][COLUMNS], char player, int maxdepth);
