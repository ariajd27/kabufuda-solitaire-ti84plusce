=============================================
Kabufuda Solitaire
as designed by Zachtronics
art and code by euphory for the TI-84 Plus CE
=============================================

=== Rules ===

Identical cards may be stacked on each other (in the tableau) and moved as a group. If all four copies of a card are stacked directly on the table, they collapse and become immovable. There are also four free cells, which can each hold either a single card or a collapsed stack. Only one free cell starts out accessible; every stack collapsed in the tableau will unlock one of the others.

Collapse all the cards into ten stacks to win! Victory is indicated by text at the top of the screen and the fact that all cards have been collapsed. At this point, the game may be exited with ON or restarted with ENTER.

=== Controls ===

arrow keys to move the cursor
2ND or ENTER to select cards to be moved, or once selected, to move those cards to the cursor position
ALPHA or CLEAR to abandon placement
DEL at any time to restart the game
ON at any time to exit the program

Stacks wrap around -- to get to the free cells, go further left than the leftmost tableau stack, or further right than the rightmost.

=== Launching the Game ===

This is an ASM program, so it must be run using the "Asm(" token found in the first page of the catalog (2nd+0). Also, for some reason it often doesn't launch the first time. Just press ENTER to run the command again, and it should work.



License: GNU General Public License
