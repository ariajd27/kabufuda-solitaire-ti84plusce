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
