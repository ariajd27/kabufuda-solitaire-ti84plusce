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
	load(); // will call deal() as well if necessary

	// set initial variables
	cursorMode = SELECT;
	cursorStack = NUM_FREECELLS;
	maxCursorIndex();
	selectedCard = CARD_EMPTY;
}

bool canGrabCard()
{
	if (cursorStack < NUM_FREECELLS) return false;
	else return !(tableau[cursorStack - NUM_FREECELLS][cursorIndex + 1] & CARD_EXISTS);
}

bool canDropCard()
{
	if (cursorStack < NUM_FREECELLS && freeCells[cursorStack] != 12) return selectedCard == (cursorStack + freeCells[cursorStack]) % 13;
	else if (cursorIndex == 0) return true;
	else return !(tableau[cursorStack - NUM_FREECELLS][cursorIndex + 1] & CARD_EXISTS);
}

bool checkTableauCollapse(unsigned char stackToCheck)
{
	return false;
}

card_t getNewCard()
{
	while (true)
	{
		card_t card = (((rand() % 4) << 4) + (rand() % 13)) | CARD_EXISTS;
		
		unsigned char cardIndex = ((card & CARD_SUIT) >> 4) * (card & CARD_NUMBER);
		unsigned char *deckByte = deck + (cardIndex / 8);
		unsigned char pokeByte = 0x01 << (cardIndex % 8);
		if (*deckByte & pokeByte) continue;
		*deckByte |= pokeByte;
		deckCards--;
		return card;
	}
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
	while (!(tableau[cursorStack - NUM_FREECELLS][cursorIndex] & CARD_EXISTS) && cursorIndex > 0) cursorIndex--;
}

bool cursorOnCollapsed()
{
	return false;
}

bool cursorOnLocked()
{
	return false;
}

