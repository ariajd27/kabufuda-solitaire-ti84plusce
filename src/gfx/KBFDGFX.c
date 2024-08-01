#include "gfx.h"
#include <fileioc.h>

#define KBFDGFX_HEADER_SIZE 0

unsigned char *KBFDGFX_appvar[16] =
{
    (unsigned char*)0,
    (unsigned char*)22,
    (unsigned char*)1049,
    (unsigned char*)2076,
    (unsigned char*)3103,
    (unsigned char*)4130,
    (unsigned char*)5157,
    (unsigned char*)6184,
    (unsigned char*)7211,
    (unsigned char*)8238,
    (unsigned char*)9265,
    (unsigned char*)10292,
    (unsigned char*)11419,
    (unsigned char*)12446,
    (unsigned char*)12504,
    (unsigned char*)13811,
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

