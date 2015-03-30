CC=g++
RM=rm
CFLAGS=-m64 -O2 -std=c++11 -Wall
SOURCES=test.cpp
ORIGIN=$(shell cd ../ && pwd)
INCLUDE=-L$(ORIGIN)/lib/ -Wl,-rpath=$(ORIGIN)/lib -I$(ORIGIN)/lib -lpanda
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(INCLUDE) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	
