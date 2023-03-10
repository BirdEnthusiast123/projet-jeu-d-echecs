CFLAGS = -c -g -Wall -Wextra

ifeq ($(OS),Windows_NT)
	RM = del
else
	RM = rm
endif

EXEC_FILE = chessboard move_list possible_moves filter_possible_moves ai
OBJECTS = chessboard.o move_list.o possible_moves.o filter_possible_moves.o ai.o
SOURCES = chessboard.c move_list.c possible_moves.c filter_possible_moves.c ai.c

CC = gcc

.PHONY: all clean


all: $(EXEC_FILE)

$(OBJECTS): %.o: %.c
	$(CC) -O3 -fPIC $< $(CFLAGS)

$(EXEC_FILE): $(OBJECTS)
	$(CC) $^ -o $@ 
	
clean:
	$(RM) -f $(EXEC_FILE) *.o *.so *.exe

lib: $(OBJECTS)
	$(CC) -fPIC -shared -o shared.so $(OBJECTS)

deploy: $(OBJECTS)
	$(CC) -O3 -o $(EXEC_FILE) $(SOURCES)
