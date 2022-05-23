#include <stdio.h>
#include <stdlib.h>

#define W_KINGSIDE_CASTLE 1
#define W_QUEENSIDE_CASTLE 2
#define B_KINGSIDE_CASTLE 4
#define B_QUEENSIDE_CASTLE 8

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
	int x;
	int y;
} Position;

typedef struct
{
    Position* from;
    Position* to;
    Piece piece;
} Move;

typedef struct
{
	int len;
	Move* move_arr;
} Move_arr;

typedef struct
{
	Piece **board;
	Player player;
	int castles;
	Position en_pass;
	int half_moves_count;
	int full_moves_count;
} Game;

int is_digit(char c);
int parse_int_char(char c);
int parse_int_string(char *str);
int skip_spaces_string(char *str, int i);

void init_game_board(Game *g);
void free_game_board(Game *g);

void parse_fen_char_pieces(Game *g, char c, int x, int y);
void parse_fen_string_pieces(Game *g, char *fen_string);
void parse_fen_string(Game *g, char *fen_string);

void print_board(Game *g);
void print_game(Game *g);
