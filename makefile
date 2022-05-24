EXEC_FILE = possible_moves chessboard
OBJECTS = possible_moves.o chessboard.o
SOURCES = possible_moves.c chessboard.c

CFLAGS = -c -g -Wall -Wextra -Werror

.PHONY: all clean


all: $(EXEC_FILE)

$(OBJECTS): %.o: %.c
	$(CC) -fPIC $< $(CFLAGS)

$(EXEC_FILE): $(OBJECTS)
	$(CC) $^ -o $@ 

clean:
	rm -f $(EXEC_FILE) *.o *.so

lib: chessboard.o possible_moves.o 
	$(CC) -fPIC -shared -o shared.so $(OBJECTS)

deploy:
	$(CC) -O3 -o $(EXEC_FILE) $(SOURCES)
