#include "gfx.h"
#include <fileioc.h>

#define KBFDGFX_HEADER_SIZE 0

unsigned char *KBFDGFX_appvar[16] =
{
    (unsigned char*)0,
    (unsigned char*)14,
    (unsigned char*)1041,
    (unsigned char*)2068,
    (unsigned char*)3095,
    (unsigned char*)4122,
    (unsigned char*)5149,
    (unsigned char*)6176,
    (unsigned char*)7203,
    (unsigned char*)8230,
    (unsigned char*)9257,
    (unsigned char*)10284,
    (unsigned char*)11411,
    (unsigned char*)12438,
    (unsigned char*)12496,
    (unsigned char*)13803,
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

