# ----------------------------
# Makefile Options
# ----------------------------

NAME = KBFDSLTR
ICON = icon.png
DESCRIPTION = "Zachtronics's Kabufuda Solitaire"
COMPRESSED = YES
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)

group::
	convbin -j 8x -i src/gfx/KBFDGFX.8xv -i bin/KBFDSLTR.8xp -k 8xg-auto-extract -o bin/KBFDSLTR.8xg -n KBFDSLTR
