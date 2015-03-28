CC=g++
RM=rm
# Libaray
LDCFLAGS=-fPIC -m64 -O2 -std=c++11 -Wall
LDFLAGS=-shared
LDINCLUDE=-lboost_serialization
LDSOURCES=world.cpp body.cpp linalg.cpp io.cpp
LDOBJECTS=$(LDSOURCES:.cpp=.o)
LIBRARY=libpanda.so
# Executable
EXCFLAGS=-m64 -02 -std=c++11 -Wall
EXSOURCES=test.cpp
EXINCLUDE=-lboost_serialization
EXOBJECTS=$(EXSOURCES:.cpp=.o)
EXECUTABLE=test


all: lib exe

exe: $(EXSOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(EXOBJECTS)
	$(CC) $(EXCFLAGS) $(EXINCLUDE) $(EXOBJECTS) -o $@

lib: $(LDSOURCES) $(LIBRARY)

$(LIBRARY): $(LDOBJECTS)
	$(CC) $(LDCFLAGS) $(LDFLAGS) $(LDINCLUDE) $(LDOBJECTS) -o $@

%.o : %.cpp
	$(CC) -c $<

clean:
	$(RM) $(LDOBJETS) $(EXOBJECTS) $(LIBRARY) $(EXECUTABLE)
	
