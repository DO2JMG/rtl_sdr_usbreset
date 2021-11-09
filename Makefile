CC      = gcc
CXX     = g++
CFLAGS  = -O3 -Wall -Wno-unused-variable
LIBS    = -lm -lusb

OBJECTS = main.o 

all:		main

main:	$(OBJECTS)
		$(CXX) $(OBJECTS) $(CFLAGS) $(LIBS) -o usbreset

%.o: %.cpp
		$(CXX) $(CFLAGS) -c -o $@ $<
                 
                                  