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

#include <fileioc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <keypadc.h>

#include "variables.h"
#include "input.h"
#include "drawing.h"
#include "save.h"
#include "ops.h"

unsigned char tableau[NUM_TABLSLOTS][TABL_STACK_SIZE];
unsigned char freeCells[NUM_FREECELLS];
unsigned char fcUnlocked;
unsigned char progress;
unsigned char numWins;

bool run()
{
	start();

	while (doInput())
	{
		clock_t frameTimer = clock();

		drawFrame();

		while (clock() - frameTimer < FRAME_TIME);
	}

	if (progress == 10)
	{
		// this way we know not to try to resume
		deleteSave();

		drawFrame();

		while (kb_AnyKey());

		while (true)
		{
			kb_Scan();
			if (kb_On) return false;
			if (kb_IsDown(kb_Key2nd) || kb_IsDown(kb_KeyEnter)) return true;
		}
	}
	else
	{
		// user exited mid-game
		save();
		return false;
	}
}

int main(void)
{
	KBFDGFX_init();

	cardSprite[0] = card1;
	cardSprite[1] = card2;
	cardSprite[2] = card3;
	cardSprite[3] = card4;
	cardSprite[4] = card5;
	cardSprite[5] = card6;
	cardSprite[6] = card7;
	cardSprite[7] = card8;
	cardSprite[8] = card9;
	cardSprite[9] = card10;
	cardSprite[10] = cardstack;

	gfx_Begin();
	gfx_SetDrawBuffer();
	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
	gfx_SetTransparentColor(3);

	kb_EnableOnLatch();
	kb_ClearOnLatch();

	while (run());

	gfx_End();

	kb_ClearOnLatch();

	return 0;
}
