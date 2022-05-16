#files with source code
SOURCE= main.c queues.c hashmap.c cache2q.c input.c test.c
OBJECTS=$(SOURCE:.c=.o)
#resulting file
EXECUTABLE=LRU_2Q
#compilation options
COMPILER=gcc
NTEST=10
CFLAGS= -Wall -Wfloat-equal -Wundef -std=c11 -D HW8_INPUT
TSTFLAGS = -Wall -Wfloat-equal -Wundef -std=c11 -D HW8_INPUT -D 'TST_RUN=$(NTEST)'


test:
	$(COMPILER) $(SOURCE) $(TSTFLAGS)  -o $(EXECUTABLE)

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

