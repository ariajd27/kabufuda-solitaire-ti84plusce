==========================================================================

                    as designed by Zachtronics

                        KABUFUDA SOLITAIRE

             art and code by euphory for the TI-84 Plus CE

source public at https://github.com/ariajd27/kabufuda-solitaire-ti84plusce

==========================================================================

=== Rules ===

Identical cards may be stacked on each other (in the tableau) and moved
as a group. If all four copies of a card are stacked directly on the
table, they collapse and become immovable. There are also four free
cells, which can each hold either a single card or a collapsed stack.
Only one free cell starts out accessible; every stack collapsed in the
tableau will unlock one of the others.

Collapse all the cards into ten stacks to win! Victory is indicated by
text at the top of the screen and the fact that all cards have been
collapsed. At this point, the game may be exited with ON or restarted
with ENTER.

Pressing ON mid-game will save and exit. To reset, press DEL. You will
probably do this frequently; it is easy to find yourself in an
unwinnable state.

=== Controls ===

- arrow keys to move the cursor
- 2ND or ENTER to select cards to be moved, or once selected, to move
  those cards to the cursor position
- ALPHA or CLEAR to abandon placement
- DEL at any time to restart the game
- ON at any time to exit the program

Stacks wrap around. To get to the free cells, go further left than
the leftmost tableau stack, or further right than the rightmost.

=== Launching the Game ===

This is an ASM program, so it must be run using the "Asm(" token found
in the first page of the catalog (2nd+0). Also, for some reason it often
doesn't launch the first time. Just press ENTER to run the command again,
and it should work.

=== Memory Usage ===

RAM: 6071 B
 - KBFDSLTR: 5968 B
 - KBFDSAVE: 85 B
 - KBFDWINS: 18 B

ARC: 15126 B
 - KBFDGFX: 15126 B

=== License ===

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
