# ----------------------------
# Makefile Options
# ----------------------------

NAME = CALCSLTR
DESCRIPTION = "Calculation Solitaire"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

group::
	convbin -j 8x -i src/gfx/CALCGFX.8xv -i bin/CALCSLTR.8xp -k 8xg-auto-extract -o bin/CALCSLTR.8xg -n CALCSLTR