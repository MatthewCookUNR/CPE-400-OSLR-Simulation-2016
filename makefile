# Check for OS Type
UNAME_S := $(shell uname -s)

# Linux
ifeq ($(UNAME_S), Linux)
	CC=g++
# Mac
else
	CC=clang++
	LIBS=-stdlib=libc++
endif

# Paths, use this to set Include directories, or library directories; uncomment in all: if used
#PATHI=-I/usr/local/include/ 
#PATHL=-L/usr/local/lib/

#Compiler Flags
CXXFLAGS=-g -Wall -std=c++0x

# .o Compilation
O_FILES=main.o node.o olsr.o route.o

# Point to includes of local directories
INDLUDES=-I../include


all: $(O_FILES)
	$(CC) $(CXXFLAGS) -o Tutorial $(O_FILES) $(LIBS) #$(PATHI) $(PATHL)

main.o: src/main.cpp
	$(CC) $(CXXFLAGS) -c src/main.cpp -o main.o $(INDLUDES)

node.o: src/node.cpp
	$(CC) $(CXXFLAGS) -c src/node.cpp -o node.o $(INDLUDES)

olsr.o: src/olsr.cpp
	$(CC) $(CXXFLAGS) -c src/olsr.cpp -o olsr.o $(INDLUDES)

route.o: src/route.cpp
	$(CC) $(CXXFLAGS) -c src/route.cpp -o route.o $(INDLUDES)

clean:
	-@if rm *.o Tutorial 2>/dev/null || true; then echo "Clean"; else echo "No Main"; fi
