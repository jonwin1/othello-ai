# othello-ai - A othello playing ai using the minmax algorithm.
# Copyright (C) 2024  Jonatan Wincent <jonatan.wincent@proton.me>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

CC=gcc
CFLAGS=-g -Wall -Wextra -Wpedantic -Wmissing-declarations -Wmissing-prototypes -Wold-style-definition -pthread

SRC=othello-ai.c minimax.c board.c
OBJ=${SRC:.c=.o}

othello-ai: ${OBJ}
	${CC} -o $@ ${OBJ} ${CFLAGS}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f othello-ai ${OBJ}

