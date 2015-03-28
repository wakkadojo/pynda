CC=g++
RM=rm
CFLAGS=-m64 -02 -std=c++11 -Wall
SOURCES=test.cpp
INCLUDE=-lboost_serialization
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	
