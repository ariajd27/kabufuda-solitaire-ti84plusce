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

#include "save.h"

#include <sys/rtc.h>
#include <fileioc.h>
#include <stdlib.h>

#include "variables.h"
#include "drawing.h"

void deal()
{
	srand(rtc_Time());

	for (unsigned char i = 0; i < 10; i++)
	{
		for (unsigned char j = 0; j < 4; j++)
		{
			while (true)
			{
				unsigned char dStack = rand() % 8;
				unsigned char dIndex = rand() % 5;

				if (tableau[dStack][dIndex] == 11)
				{
					tableau[dStack][dIndex] = i;
					break;
				}
			}
		}
	}

	fcUnlocked = 1;
	progress = 0;

	animateDeal();
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
		// load save
		ti_Read(freeCells, 1, NUM_FREECELLS, saveHandle);
		ti_Read(tableau, 1, NUM_TABLSLOTS * TABL_STACK_SIZE, saveHandle);
		
		// how far along does that make us?
		progress = 0;
		fcUnlocked = 1;
		for (unsigned char i = 0; i < NUM_FREECELLS; i++)
		{
			if (freeCells[i] > 11)
			{
				progress++;
			}
		}
		for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
		{
			if (tableau[i][0] > 11)
			{
				progress++;
				fcUnlocked++;
			}
		}
	}

	ti_Close(saveHandle);
}

void save()
{
	unsigned char saveHandle = ti_Open(SAVE_VAR_NAME, "w");

	ti_Write(freeCells, 1, NUM_FREECELLS, saveHandle);
	ti_Write(tableau, 1, NUM_TABLSLOTS * TABL_STACK_SIZE, saveHandle);

	ti_Close(saveHandle);
}

void deleteSave()
{
	ti_Delete(SAVE_VAR_NAME);
}

void loadWins()
{
	numWins = 0;
	unsigned char winsHandle = ti_Open(WINS_VAR_NAME, "r");
	if (winsHandle != 0) ti_Read(&numWins, 1, 1, winsHandle);
	ti_Close(winsHandle);
}

void saveWins()
{
	unsigned char const winsHandle = ti_Open(WINS_VAR_NAME, "w");
	ti_Write(&numWins, 1, 1, winsHandle);
	ti_Close(winsHandle);
}
