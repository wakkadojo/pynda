CC=g++
RM=rm
CFLAGS=-fPIC -m64 -O0 -std=c++11 -Wall -g
LDFLAGS=-shared
SOURCES=panda.cpp
ORIGIN=$(shell cd ../ && pwd)
INCLUDE=-L$(ORIGIN)/lib/ -Wl,-rpath=$(ORIGIN)/lib -I$(ORIGIN)/lib -lpanda -lboost_python -I/usr/include/python2.7/ -lpython2.7
OBJECTS=$(SOURCES:.cpp=.o)
LIBRARY=panda.so

all: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(INCLUDE) -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	$(RM) $(OBJECTS) $(LIBRARY)
