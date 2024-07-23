#include <sys/rtc.h>
#include <fileioc.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <keypadc.h>

#include "variables.h"
#include "input.h"
#include "drawing.h"

unsigned char tableau[NUM_TABLSLOTS][TABL_STACK_SIZE];
unsigned char freeCells[NUM_FREECELLS];
unsigned char fcUnlocked;
unsigned char progress;
unsigned char numWins;

void start()
{
	srand(rtc_Time());

	// get number of wins
	numWins = 0;
	unsigned char saveHandle = ti_Open(SAVE_VAR_NAME, "r");
	if (saveHandle != 0) ti_Read(&numWins, 1, 1, saveHandle);
	ti_Close(saveHandle);

	// fill tableau and free cells with empty space
	for (unsigned char i = 0; i < NUM_FREECELLS; i++) 
		freeCells[i] = 11;
	for (unsigned char i = 0; i < NUM_TABLSLOTS; i++)
		for (unsigned char j = 0; j < TABL_STACK_SIZE; j++)
			tableau[i][j] = 11;

	// deal the deck
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

	cursorMode = SELECT;
	cursorStack = NUM_FREECELLS;
	cursorIndex = 40 / NUM_TABLSLOTS - 1;

	selectedCard = 1;
}

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
		unsigned char const saveHandle = ti_Open(SAVE_VAR_NAME, "w");
		ti_Write(&numWins, 1, 1, saveHandle);
		ti_Close(saveHandle);

		while (kb_AnyKey());

		while (true)
		{
			kb_Scan();
			if (kb_On) return false;
			if (kb_IsDown(kb_Key2nd)) return true;
		}
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

	while (run());

	gfx_End();

	kb_ClearOnLatch();

	return 0;
}
