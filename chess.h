#include <stdio.h>
#include <stdlib.h>

#define W_KINGSIDE_CASTLE 1
#define W_QUEENSIDE_CASTLE 2
#define B_KINGSIDE_CASTLE 4
#define B_QUEENSIDE_CASTLE 8

#define MOVE_LIST_BUFFER_SIZE 8

typedef enum
{
	W_KNIGHT = 1,
	W_QUEEN = 2,
	W_KING = 3,
	W_ROOK = 4,
	W_BISHOP = 5,
	W_PAWN = 6,
	B_QUEEN = 8,
	B_KING = 9,
	B_ROOK = 10,
	B_BISHOP = 11,
	B_PAWN = 12,
	B_KNIGHT = 13,
	EMPTY = 16
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


// chessboard.c functions
int is_digit(char c);
int parse_int_char(char c);
int parse_int_string(char *str);
int skip_spaces_string(char *str, int i);

void init_game_board(Game *g);
void free_game_board(Game *g);

void parse_fen_char_pieces(Game *g, char c, int x, int* y);
void parse_fen_string_pieces(Game *g, char *fen_string);
void parse_fen_string(Game *g, char *fen_string);

void print_board(Game *g);
void print_game(Game *g);

Piece get_piece(Game* g, int x, int y);

int is_black(Piece p);
int is_white(Piece p);
int is_empty(Piece p);



// move list for a singular piece
// arr is x and y coordinates one after another
// ew : [x1, y1, x2, y2]
typedef struct
{
	int len;
	int nb;
	int *arr;
} Move_list;

// possible_moves.c functions
Move_list *init_move_list();
void add_move(Move_list *ml, int x, int y);
void fill_move_list(Game *g, int x, int y, Move_list *ml);
Move_list *possible_moves(char *fen, int x, int y);
void free_move_list(Move_list *ml);
void print_move_list(Move_list *ml);