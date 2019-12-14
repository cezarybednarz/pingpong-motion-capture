#
# Platform Independent
# Bitmap Image Reader Writer Library
# By Arash Partow - 2002
#
# URL: http://partow.net/programming/bitmap/index.html
#
# Copyright Notice:
# Free use of this library is permitted under the
# guidelines and in accordance with the most
# current version of the MIT License.
# http://www.opensource.org/licenses/MIT
#


COMPILER      = -g++ 
OPTIONS       = -ansi -pedantic-errors -Wall -Wall -Wextra -o
LINKER_OPT    = -L/usr/lib -lstdc++ -lm -std=c++11

all: filter

filter: filter.cpp bitmap_image.hpp
	$(COMPILER) $(OPTIONS) filter filter.cpp $(LINKER_OPT)

valgrind_check:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes -v ./bitmap_test

clean:
	rm -f core *.o *.bak *stackdump *~

#
# The End !
#
