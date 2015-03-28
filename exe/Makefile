CC=g++
RM=rm
CFLAGS=-m64 -O2 -std=c++11 -Wall
LDFLAGS=-lpanda
SOURCES=test.cpp
INCLUDE=-L../lib/ -Wl,-rpath=../lib/ -I../lib/
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	
