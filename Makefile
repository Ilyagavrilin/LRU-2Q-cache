#files with source code
SOURCE= main.c queues.c hashmap.c cache2q.c input.c test.c
OBJECTS=$(SOURCE:.c=.o)
#resulting file
EXECUTABLE=LRU_2Q
#compilation options
COMPILER=gcc
CFLAGS= -Wall -Wfloat-equal -Wundef -std=c11



all: $(SOURCE) $(EXECUTABLE) clean

build_obj: $(SOURCE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $@

.c.o:
	$(COMPILER) -c $(CFLAGS) $< -o $@

clean_all:
	rm -rf *.o $(EXECUTABLE)

clean:
	rm -rf *.o 

