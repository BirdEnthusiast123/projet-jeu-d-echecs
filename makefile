EXEC_FILE = possible_moves chessboard
OBJECTS = possible_moves.o chessboard.o
SOURCES = possible_moves.c chessboard.c

CFLAGS = -c -g -Wall -Wextra -Werror
MEM_DEBUG = -fsanitize=address

.PHONY: all clean


all: $(EXEC_FILE)

$(OBJECTS): %.o: %.c
	$(CC) $< $(CFLAGS) $(MEM_DEBUG)

$(EXEC_FILE): $(OBJECTS)
	$(CC) $^ -o $@ $(MEM_DEBUG)

clean:
	rm -f $(EXEC_FILE) *.o *.so

lib:
	gcc -fPIC -shared -o shared.so possible_moves.c

deploy:
	$(CC) -O3 -o $(EXEC_FILE) $(SOURCES)
