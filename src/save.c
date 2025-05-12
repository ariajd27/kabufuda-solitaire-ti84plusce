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

#include "save.h"

#include <sys/rtc.h>
#include <fileioc.h>
#include <stdlib.h>

#include "variables.h"
#include "drawing.h"
#include "ops.h"

#define MAGIC_NUMBER 0x02

unsigned char deck[7];
unsigned char deckCards;

unsigned char *numWins;
unsigned char *saveData;

void deal()
{
	progress = 0;
	deckCards = 52;

	for (unsigned char i = 0; i < 7; i++)
	{
		deck[i] = 0x00;
	}

	for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
	{
		for (unsigned char j = 0; j < TABL_STACK_SIZE; j++)
		{
			tableau[i][j] = CARD_EMPTY;
		}
	}

	for (unsigned char i = 0; i < NUM_FREECELLS; i++)
	{
		freeCells[i] = CARD_EXISTS | i | (unsigned char)(rand() & CARD_SUIT);
		removeFromDeck(freeCells[i]);
	}

	animateDeal();
}

void loadWins()
{
	const unsigned char winsHandle = ti_Open(WINS_VAR_NAME, "w+");
	
	while (true)
	{
		unsigned char thisId = 0;
	    const unsigned char success = ti_Read(&thisId, 1, 1, winsHandle);

        if (!success)
        {
            // reached end without finding our numWins
            ti_PutC(MAGIC_NUMBER, winsHandle);
            ti_PutC(0, winsHandle);
            numWins = ti_GetDataPtr(winsHandle) - 1;
            break;
        }

		if (thisId == MAGIC_NUMBER)
		{
            numWins = ti_GetDataPtr(winsHandle);
            break;
		}
        else ti_Seek(1, SEEK_CUR, winsHandle); // skip this numWins
	}

	ti_Close(winsHandle);
}

void load()
{
	unsigned char saveHandle = ti_Open(SAVE_VAR_NAME, "r");

	if (saveHandle == 0)
	{
		// no save present
		deal();
	}
	else
	{
        unsigned char magicNumber = ti_GetC(saveHandle);
        if (magicNumber != MAGIC_NUMBER)
        {
            // this is from a different solitaire game
            deal();
        }
        else
        {
            // load save
            ti_Read(freeCells, 1, NUM_FREECELLS, saveHandle);
            ti_Read(tableau, 1, NUM_TABLSLOTS * TABL_STACK_SIZE, saveHandle);
            
            // how far along does that make us?
            progress = 0;
            for (unsigned char i = 0; i < NUM_FREECELLS; i++)
            {
                if ((freeCells[i] & CARD_NUMBER) == CARD_KING) progress++;
            }

            ti_Read(&deckCards, 1, 1, saveHandle);
            ti_Read(deck, 1, 7, saveHandle);
            ti_Read(&selectedCard, 1, 1, saveHandle);
        }
	}

	ti_Close(saveHandle);
}

void save()
{
	unsigned char saveHandle = ti_Open(SAVE_VAR_NAME, "w");

    ti_PutC(MAGIC_NUMBER, saveHandle);
	ti_Write(freeCells, 1, NUM_FREECELLS, saveHandle);
	ti_Write(tableau, 1, NUM_TABLSLOTS * TABL_STACK_SIZE, saveHandle);
	ti_Write(&deckCards, 1, 1, saveHandle);
	ti_Write(deck, 1, 7, saveHandle);
	ti_Write(&selectedCard, 1, 1, saveHandle);

	ti_Close(saveHandle);
}

void deleteSave()
{
	ti_Delete(SAVE_VAR_NAME);
}
