#ifndef ops_include_file
#define ops_include_file

#include <stdbool.h>

bool canGrabCard();
bool canDropCard();
bool checkTableauCollapse(unsigned char stackToCheck);

void dropCard();
void maxCursorIndex();

bool cursorOnCollapsed();
bool cursorOnLocked();

#endif
