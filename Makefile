CC=g++
RM=rm
CFLAGS=-m64 -O2 -std=c++11 -Wall
INCLUDE=-lboost_serialization
SOURCES=test.cpp world.cpp body.cpp linalg.cpp io.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	
