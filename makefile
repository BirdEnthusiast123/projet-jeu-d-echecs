EXEC_FILE = possible_moves
OBJECTS = possible_moves.o
SOURCES = possible_moves.c

CFLAGS = -c -g -Wall -Wextra -Werror# obligatoires
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
