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

#include "ops.h"

#include <stdlib.h>
#include <fileioc.h>

#include "variables.h"
#include "save.h"
#include "drawing.h"

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
	else if (tableau[cursorStack - NUM_FREECELLS][cursorIndex + 1] & CARD_EXISTS) return false;
	else return tableau[cursorStack - NUM_FREECELLS][cursorIndex] & CARD_EXISTS;
}

bool canDropCard()
{
	if (cursorStack < NUM_FREECELLS)
	{
		if ((freeCells[cursorStack] & CARD_NUMBER) == CARD_KING) return false;
		else return (selectedCard & CARD_NUMBER) == (cursorStack + (freeCells[cursorStack] & CARD_NUMBER) + 1) % 13;
	}
	else if (orgStack != DECK_ORG)
	{
		return cursorStack == orgStack;
	}
	else if (cursorIndex == 0)
	{
		return true;
	}
	else
	{
		return !(tableau[cursorStack - NUM_FREECELLS][cursorIndex + 1] & CARD_EXISTS);
	}
}

void getNewCard()
{
	while (true)
	{
		selectedCard = (rand() & CARD_SUIT) | (rand() % 13) | CARD_EXISTS;
		if (!removeFromDeck(selectedCard)) break;
	}

	animateDraw();
}

bool removeFromDeck(card_t toRemove)
{
	unsigned char cardIndex = ((toRemove & CARD_SUIT) >> 4) * 13 + (toRemove & CARD_NUMBER);
	unsigned char *deckByte = deck + (cardIndex / 8);
	unsigned char pokeByte = 0x01 << (cardIndex % 8);
	if (*deckByte & pokeByte) return true;
	
	*deckByte |= pokeByte;
	deckCards--;
	return false;
}

void grabCard()
{
	orgStack = cursorStack;
	orgIndex = cursorIndex;

	selectedCard = tableau[cursorStack - NUM_FREECELLS][cursorIndex];
	tableau[cursorStack - NUM_FREECELLS][cursorIndex] = CARD_EMPTY;

	animateGrab();
}

void dropCard()
{
	animateDrop();

	unsigned char const prevProgress = progress;

	if (cursorStack < NUM_FREECELLS)
	{
		freeCells[cursorStack] = selectedCard;
		if ((selectedCard & CARD_NUMBER) == CARD_KING) progress++;
	}
	else
	{
		tableau[cursorStack - NUM_FREECELLS][cursorIndex] = selectedCard;
	}

	if (progress == PROGRESS_COMPLETE && prevProgress < PROGRESS_COMPLETE) numWins++;
}

void maxCursorIndex()
{
	if (cursorStack < NUM_FREECELLS) return;

	cursorIndex = 0;
	while (tableau[cursorStack - NUM_FREECELLS][cursorIndex + 1] & CARD_EXISTS) cursorIndex++;
}