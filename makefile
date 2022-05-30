CFLAGS = -c -g -Wall -Wextra

ifeq ($(OS),Windows_NT)
	RM = del
else
	RM = rm
endif

EXEC_FILE = chessboard possible_moves filter_possible_moves
OBJECTS = chessboard.o possible_moves.o filter_possible_moves.o
SOURCES = chessboard.c possible_moves.c filter_possible_moves.c

CC = gcc

.PHONY: all clean


all: $(EXEC_FILE)

$(OBJECTS): %.o: %.c
	$(CC) -fPIC $< $(CFLAGS)

$(EXEC_FILE): $(OBJECTS)
	$(CC) $^ -o $@ 

clean:
	$(RM) -f $(EXEC_FILE) *.o *.so *.exe

lib: $(OBJECTS)
	$(CC) -fPIC -shared -o shared.so $(OBJECTS)

deploy:
	$(CC) -O3 -o $(EXEC_FILE) $(SOURCES)
