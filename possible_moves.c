#include <stdio.h>
#include <stdlib.h>

#include <string.h>


typedef enum 
{
	EMPTY = 1,
	W_KNIGHT = 2,
	W_QUEEN = 4,
	W_KING = 8,
	W_ROOK = 16,
	W_BISHOP = 32,
	W_PAWN = 64,
	B_QUEEN = 128,
	B_KING = 256,
	B_ROOK = 512,
	B_BISHOP = 1024,
	B_PAWN = 2048,
	B_KNIGHT = 4096
} Piece;

typedef enum
{
	black = 'b',
	white = 'w'
} Player;

typedef struct
{
	Piece** board;
	char player;
	int castles;
	int half_moves_count;
	int full_moves_count;
} Game;

int is_digit(char c)
{
	return (c > 47) && (c < 58);
}

int parse_int(char c)
{
	return c - 48;
}

// post: free memory with free_game_board
void init_game_board(Game* g)
{
	g->board = (Piece**) malloc(8 * sizeof(Piece*));
	for(int i = 0; i < 8; i++)
	{
		g->board[i] = (Piece*) malloc(8 * sizeof(Piece));
	}
}

void free_game_board(Game* g)
{
	for(int i = 0; i < 8; i++)
	{
		free(g->board[i]);
	}
	free(g->board);
}

void parse_fen_char_pieces(Game* g, char c, int x, int y)
{
	if(is_digit(c))
	{
		int tmp = y + parse_int(c);
		for(int i = y; i < tmp; i++)
		{
			g->board[x][i] = EMPTY;
		}
	}
	else
	{
		switch(c)
		{
			case 'p':
				g->board[x][y] = EMPTY;
				break;
			case 'r':
				g->board[x][y] = EMPTY;
				break;
			case 'n':
				g->board[x][y] = EMPTY;
				break;
			case 'b':
				g->board[x][y] = EMPTY;
				break;
			case 'q':
				g->board[x][y] = EMPTY;
				break;
			case 'k':
				g->board[x][y] = EMPTY;
				break;
			case 'P':
				g->board[x][y] = EMPTY;
				break;
			case 'R':
				g->board[x][y] = EMPTY;
				break;
			case 'N':
				g->board[x][y] = EMPTY;
				break;
			case 'B':
				g->board[x][y] = EMPTY;
				break;
			case 'Q':
				g->board[x][y] = EMPTY;
				break;
			case 'K':
				g->board[x][y] = EMPTY;
				break;
			default:
				break;
		}
	}
}

void parse_fen_string_pieces(Game* g, char* fen_string)
{
	int i = 0, x = 0;
	while(fen_string[i] != ' ')
	{
		int y = 0;
		while(fen_string[i] != '/')
		{
			
		}
	}
}

void parse_fen_string(Game* g, char* fen_string)
{
	int i = 0;
	while(fen_string[i] != ' ')
	{
		// pieces
	}
	
	while(fen_string[i] != ' ')
	{
		// color to play
	}
	
	while(fen_string[i] != ' ')
	{
		// possible castles
	}
	
	while(fen_string[i] != ' ')
	{
		// en passant
	}
	
	while(fen_string[i] != ' ')
	{
		// half moves -> 50 moves draw rule
	}
	
	while(fen_string[i] != '\0')
	{
		// nb of moves
	}
}


void print_test(char* str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		printf("%d \n", parse_int(str[i]));
		i++;
	}
}



int main(int argc, char **argv)
{
    printf("hey");
    return 0;
}
