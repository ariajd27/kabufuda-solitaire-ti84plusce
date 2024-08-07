// Kabufuda Solitaire / KBFDSLTR for the TI-84 Plus CE
// Copyright (C) 2024 euphory
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

#include "ops.h"

#include <stdlib.h>
#include <fileioc.h>

#include "variables.h"
#include "save.h"

unsigned char cursorStack;
unsigned char cursorIndex;
enum cursorMode_t cursorMode;
unsigned char selectedCard;
unsigned char selectedQty;
unsigned char orgStack;
unsigned char orgIndex;

void start()
{
	loadWins();

	// fill tableau and free cells with empty space
	for (unsigned char i = 0; i < NUM_FREECELLS; i++) 
		freeCells[i] = 11;
	for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
		for (unsigned char j = 0; j < TABL_STACK_SIZE; j++)
			tableau[i][j] = 11;

	load(); // will call deal() as well if necessary

	// set initial variables
	cursorMode = SELECT;
	cursorStack = NUM_FREECELLS;
	maxCursorIndex();
	selectedCard = tableau[cursorStack][cursorIndex];
}

bool canGrabCard()
{
	if (cursorStack < NUM_FREECELLS) return freeCells[cursorStack] < 11;

	if (tableau[cursorStack - NUM_FREECELLS][0] > 11) return false;
	for (unsigned char i = cursorIndex + 1; i < TABL_STACK_SIZE; i++)
	{
		const unsigned char x = tableau[cursorStack - NUM_FREECELLS][i];
		if (x == 11) return true;
		if (x != selectedCard) return false;
	}

	return true;
}

bool canDropCard()
{
	if (cursorStack < NUM_FREECELLS) 
	{
		const unsigned char x = freeCells[cursorStack];
		if (cursorStack < fcUnlocked)
		{
			if (selectedQty == 1 || selectedQty == 4) return x == 11;
			else return x == selectedCard && selectedQty == 3;
		}
		else return false;
	}
	else if (cursorIndex == 0) return true;
	else return tableau[cursorStack - NUM_FREECELLS][cursorIndex - 1] == selectedCard;
}

bool checkTableauCollapse(unsigned char stackToCheck)
{
	const unsigned char x = tableau[stackToCheck][0];
	for (unsigned char i = 1; i < 5; i++)
	{
		const unsigned char y = tableau[stackToCheck][i];
		if (i == 4 && y == 11) return true;
		if (y != x) return false;
	}

	return false;
}

void dropCard()
{
	if (cursorStack == orgStack) return;

	unsigned char const prevProgress = progress;

	if (cursorStack < NUM_FREECELLS)
	{
		if (selectedQty == 1) freeCells[cursorStack] = selectedCard;
		else
		{
			progress++;
			freeCells[cursorStack] = 12 + selectedCard;
		}
	}
	else
	{
		for (unsigned char i = cursorIndex; i < cursorIndex + selectedQty; i++)
			tableau[cursorStack - NUM_FREECELLS][i] = selectedCard;
		if (checkTableauCollapse(cursorStack - NUM_FREECELLS))
		{
			fcUnlocked++;
			progress++;
			tableau[cursorStack - NUM_FREECELLS][0] = selectedCard + 12;
			for (int i = 1; i < 4; i++) tableau[cursorStack - NUM_FREECELLS][i] = 11;
		}
	}

	if (orgStack < NUM_FREECELLS) freeCells[orgStack] = 11;
	else for (unsigned char i = orgIndex; i < orgIndex + selectedQty; i++)
		tableau[orgStack - NUM_FREECELLS][i] = 11;

	if (progress == 10 && prevProgress < 10) numWins++;
}

void maxCursorIndex()
{
	if (cursorStack < NUM_FREECELLS) return;

	cursorIndex = TABL_STACK_SIZE - 1;
	while (tableau[cursorStack - NUM_FREECELLS][cursorIndex] == 11 && cursorIndex > 0) cursorIndex--;
}

bool cursorOnCollapsed()
{
	return cursorStack < NUM_FREECELLS ? freeCells[cursorStack] > 11 
									   : tableau[cursorStack - NUM_FREECELLS][0] > 11;
}

bool cursorOnLocked()
{
	return cursorStack >= fcUnlocked && cursorStack < NUM_FREECELLS;
}

