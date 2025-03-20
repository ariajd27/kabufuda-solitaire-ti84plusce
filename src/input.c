// Calculation Solitaire / CALCSLTR for the TI-84 Plus CE
// Copyright (C) 2025 euphory
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "input.h"

#include <stdlib.h>
#include <keypadc.h>
#include "variables.h"
#include "ops.h"
#include "save.h"

unsigned char down, left, right, up;

bool doInput()
{
	const bool prevSecond = kb_IsDown(kb_Key2nd);
	const bool prevAlpha = kb_IsDown(kb_KeyAlpha);
	const bool prevClear = kb_IsDown(kb_KeyClear);

	kb_Scan();

	up = kb_IsDown(kb_KeyUp) ? up + 1 : 0;
	down = kb_IsDown(kb_KeyDown) ? down + 1 : 0;
	left = kb_IsDown(kb_KeyLeft) ? left + 1 : 0;
	right = kb_IsDown(kb_KeyRight) ? right + 1 : 0;

	const bool select = (kb_IsDown(kb_Key2nd) && !prevSecond);
	const bool draw = (kb_IsDown(kb_KeyAlpha) && !prevAlpha);
	const bool clear = (kb_IsDown(kb_KeyClear) && !prevClear);

	if (select)
	{
		if (cursorMode == SELECT && canGrabCard())
		{
			orgStack = cursorStack - NUM_FREECELLS;
			orgIndex = cursorIndex;

			selectedCard = tableau[cursorStack - NUM_FREECELLS][cursorIndex];
			tableau[cursorStack - NUM_FREECELLS][cursorIndex] = CARD_EMPTY;

			cursorMode = DROP;
		}
		else if (canDropCard())
		{
			dropCard();
			cursorMode = SELECT;
		}
	}
	else if (clear)
	{
		if (orgStack != DECK_ORG)
		{
			tableau[orgStack][orgIndex] = selectedCard;

			cursorMode = SELECT;
			selectedCard = CARD_EMPTY;
		}
	}
	else if (draw)
	{
		if (cursorMode == SELECT)
		{
			selectedCard = getNewCard();
			orgStack = DECK_ORG;
			cursorMode = DROP;
		}
	}

	unsigned char prevCursorStack = cursorStack;

	if (down == 1 || down > HOLD_TIME)
	{
		if (cursorStack < NUM_FREECELLS || !(tableau[cursorStack][cursorIndex + 1] & CARD_EXISTS)) cursorIndex++;
	}
	else if (left == 1 || left > HOLD_TIME)
	{
		if (cursorStack == 0) cursorStack = NUM_FREECELLS + NUM_TABLSLOTS - 1; // wrap
		else cursorStack--;
	}
	else if (right == 1 || right > HOLD_TIME)
	{
		if (cursorStack > NUM_FREECELLS + NUM_TABLSLOTS - 2) cursorStack = 0; // wrap
		else cursorStack++;
	}
	else if (up == 1 || up > HOLD_TIME)
	{
		if (cursorIndex > 0) cursorIndex--;
	}

	if (cursorStack < NUM_FREECELLS)
	{
		cursorIndex = 0;
	}
	else if (cursorMode == DROP)
	{
		maxCursorIndex();
		if (cursorStack >= NUM_FREECELLS && (tableau[cursorStack - NUM_FREECELLS][0] & CARD_EXISTS)) cursorIndex++;
	}
	else if (cursorStack != prevCursorStack)
	{
		maxCursorIndex();
	}

	if (kb_IsDown(kb_KeyDel))
	{
		deleteSave();
		start();
	}

	return !kb_On && progress < 10;
}
