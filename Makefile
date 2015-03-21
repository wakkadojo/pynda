CC=g++
RM=rm
CFLAGS=-m64 -O2 -std=c++11 -Wall
SOURCES=test.cpp world.cpp body.cpp linalg.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	
