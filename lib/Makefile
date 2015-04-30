CC=g++
RM=rm
# Libaray
CFLAGS=-fPIC -m64 -O0 -std=c++11 -Wall -g
LDFLAGS=-shared
INCLUDE=-lboost_serialization
SOURCES=world.cpp body.cpp linalg.cpp
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=libpanda.so

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(OBJECTS) $(LIBRARY)
	
