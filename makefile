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