#include "gfx.h"
#include <fileioc.h>

#define KBFDGFX_HEADER_SIZE 0

unsigned char *KBFDGFX_appvar[16] =
{
    (unsigned char*)0,
    (unsigned char*)16,
    (unsigned char*)1043,
    (unsigned char*)2070,
    (unsigned char*)3097,
    (unsigned char*)4124,
    (unsigned char*)5151,
    (unsigned char*)6178,
    (unsigned char*)7205,
    (unsigned char*)8232,
    (unsigned char*)9259,
    (unsigned char*)10286,
    (unsigned char*)11413,
    (unsigned char*)12440,
    (unsigned char*)12498,
    (unsigned char*)13805,
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

