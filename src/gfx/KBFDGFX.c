#include "gfx.h"
#include <fileioc.h>

#define KBFDGFX_HEADER_SIZE 0

unsigned char *KBFDGFX_appvar[16] =
{
    (unsigned char*)0,
    (unsigned char*)28,
    (unsigned char*)1055,
    (unsigned char*)2082,
    (unsigned char*)3109,
    (unsigned char*)4136,
    (unsigned char*)5163,
    (unsigned char*)6190,
    (unsigned char*)7217,
    (unsigned char*)8244,
    (unsigned char*)9271,
    (unsigned char*)10298,
    (unsigned char*)11425,
    (unsigned char*)12452,
    (unsigned char*)12510,
    (unsigned char*)13817,
};

unsigned char KBFDGFX_init(void)
{
    unsigned int data, i;
    uint8_t appvar;

    appvar = ti_Open("KBFDGFX", "r");
    if (appvar == 0)
    {
        return 0;
    }

    data = (unsigned int)ti_GetDataPtr(appvar) - (unsigned int)KBFDGFX_appvar[0] + KBFDGFX_HEADER_SIZE;
    for (i = 0; i < 16; i++)
    {
        KBFDGFX_appvar[i] += data;
    }

    ti_Close(appvar);

    return 1;
}

