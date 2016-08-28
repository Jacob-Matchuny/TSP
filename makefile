SOURCES=$(wildcard src/*.cpp)
OBJS=$(SOURCES:.cpp=.o)

SYS := $(shell gcc -dumpmachine)

LIBFLAGS=-Llib -Bdynamic -Wl,-rpath=lib -lcairo 

project1: $(OBJS)
	g++ $(OBJS) -std=c++11 -o $@ -I/usr/include/cairo/ `pkg-config --cflags --libs gtk+-3.0` -Wall -O3 $(LIBFLAGS) -g
	rm -f $(OBJS) *~
src/%.o : src/%.cpp
	g++ $< -c -std=c++11 -o $@ -I/usr/include/cairo/  `pkg-config --cflags --libs gtk+-3.0` -Wall -O3 -Iinclude -g -lcairo

# cleans stuff
clean:
	rm -f $(OBJS) $(TARG) *~
