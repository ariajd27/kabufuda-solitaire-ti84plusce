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

#ifndef ops_include_file
#define ops_include_file

#include <stdbool.h>
#include "variables.h"

void start();

bool canGrabCard();
bool canDropCard();

void getNewCard();
bool removeFromDeck(card_t toRemove); // returns true iff card not in deck
void grabCard();
void dropCard();
void clearCard();
void maxCursorIndex();

#endif
