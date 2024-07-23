#include "input.h"

#include <stdlib.h>
#include <keypadc.h>
#include "variables.h"
#include "ops.h"

unsigned char down, left, right, up;

bool doInput()
{
	const bool prevSecond = kb_IsDown(kb_Key2nd);
	const bool prevEnter = kb_IsDown(kb_KeyEnter);
	const bool prevAlpha = kb_IsDown(kb_KeyAlpha);
	const bool prevClear = kb_IsDown(kb_KeyClear);

	kb_Scan();

	up = kb_IsDown(kb_KeyUp) ? up + 1 : 0;
	down = kb_IsDown(kb_KeyDown) ? down + 1 : 0;
	left = kb_IsDown(kb_KeyLeft) ? left + 1 : 0;
	right = kb_IsDown(kb_KeyRight) ? right + 1 : 0;

	const bool select = (kb_IsDown(kb_Key2nd) && !prevSecond) || (kb_IsDown(kb_KeyEnter) && !prevEnter);
	const bool clear = (kb_IsDown(kb_KeyClear) && !prevClear) || (kb_IsDown(kb_KeyAlpha) && !prevAlpha);

	if (select)
	{
		if (cursorMode == SELECT && canGrabCard())
		{
			orgStack = cursorStack;
			orgIndex = cursorIndex;
			cursorMode = DROP;
		}
		else if (canDropCard())
		{
			dropCard();
			cursorMode = SELECT;
		}
		else if (cursorStack == orgStack)
		{
			cursorMode = SELECT;
		}
	}
	else if (clear) cursorMode = SELECT;

	unsigned char prevCursorStack = cursorStack;

	if (down == 1 || down > HOLD_TIME)
	{
		cursorIndex++;
	}
	else if (left == 1 || left > HOLD_TIME)
	{
		do
		{
			if (cursorStack == 0) cursorStack = NUM_FREECELLS + NUM_TABLSLOTS - 1; // wrap
			else cursorStack--;
		}
		while (cursorOnCollapsed() || cursorOnLocked()); // skip useless stacks
	}
	else if (right == 1 || right > HOLD_TIME)
	{
		do
		{
			if (cursorStack > NUM_FREECELLS + NUM_TABLSLOTS - 2) cursorStack = 0; // wrap
			else cursorStack++;
		}
		while (cursorOnCollapsed() || cursorOnLocked()); // skip useless stacks
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
		// if dropping cards, the cursor should always be at the top of the stack it's on
		// except maxCursorIndex() doesn't work so well on empty tableau stacks
		if (cursorStack != orgStack)
		{
			if (tableau[cursorStack - NUM_FREECELLS][0] == 11)
			{
				cursorIndex = 0;
			}
			else
			{
				maxCursorIndex();
				cursorIndex++;
			}
		}
		else cursorIndex = orgIndex; // also this makes it more clear if we are trying to drop
									 // back on the original cards
	}
	else if (cursorStack != prevCursorStack)
	{
		maxCursorIndex();
	}
	else
	{
		while (tableau[cursorStack - NUM_FREECELLS][cursorIndex] == 11 && cursorIndex > 0) cursorIndex--;
	}

	if (cursorMode == SELECT)
	{
		selectedQty = 1;
		if (cursorStack < NUM_FREECELLS) selectedCard = freeCells[cursorStack];
		else 
		{
			selectedCard = tableau[cursorStack - NUM_FREECELLS][cursorIndex];
			for (int i = cursorIndex + 1; tableau[cursorStack - NUM_FREECELLS][i] == selectedCard; i++) 
				selectedQty++;
		}

		if (selectedCard > 11)
		{
			selectedCard -= 12;
			selectedQty = 4;
		}
	}

	return (!kb_On) && (progress < 10);
}
