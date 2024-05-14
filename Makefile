##
# Anim
#
# @file
# @version 0.1

CXX=g++
CPPFLAGS=-Wall -Wextra -std=c++26 -lraylib

SRC = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJ = $(SRC:.cpp=.o)

%.o: %.cpp $(HEADERS) Makefile
	$(CXX) $< $(CPPFLAGS) -g -fsanitize=address -c

debug: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(CPPFLAGS) -g -fsanitize=address

clean:
	rm -rf $(OBJ) debug

# end
