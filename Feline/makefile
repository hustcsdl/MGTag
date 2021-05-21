CC	= g++
CPPFLAGS= -Wno-deprecated -O3 -c -lm
LDFLAGS	= -O3 
SOURCES	= main.cpp Graph.cpp GraphUtil.cpp 
OBJECTS	= $(SOURCES:.cpp=.o) 
EXECUTABLE=feline

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o : 
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	-rm -f *.o feline
