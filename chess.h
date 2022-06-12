#include <stdio.h>
#include <stdlib.h>

/**
 * \def W_KINGSIDE_CASTLE
 * \brief Bit representation of different possible castles
 */
#define W_KINGSIDE_CASTLE 1
#define W_QUEENSIDE_CASTLE 2
#define B_KINGSIDE_CASTLE 4
#define B_QUEENSIDE_CASTLE 8

/**
 * \def MOVE_LIST_BUFFER_SIZE
 * \brief Base size for Move_list struct array
 */
#define MOVE_LIST_BUFFER_SIZE 8

#define PRINT_STR " NQKRBP qkrbpn  _"

/**
 * \enum Piece
 * \brief Representation of chess pieces and emptiness
 */
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

/**
 * \struct Position
 * \brief Representation of a chess position
 */
typedef struct
{
	int x;
	int y;
} Position;

/**
 * \struct Game
 * \brief Representation of a chess game
 */
typedef struct
{
	Piece board[8][8];
	int bool_is_black;
	int castles;
	Position en_pass;
	int half_moves_count;
	int full_moves_count;

	int enemy_pieces_count;
	Position enemy_pieces[16];

	int ally_pieces_count;
	Position ally_pieces[16];

	int threatmap[8][8];

	Position black_king_pos;
	Position white_king_pos;
} Game;

// chessboard.c 
int is_digit(char c);
int parse_int_char(char c);
int parse_int_string(char *str);
int skip_spaces_string(char *str, int i);

void init_game_board(Game *g);
void free_game_board(Game *g);

void parse_fen_char_pieces(Game *g, char c, int x, int *y);
void parse_fen_string_pieces(Game *g, char *fen_string);
void parse_fen_string(Game *g, char *fen_string);

void print_board(Game *g);
void print_game(Game *g);

Piece get_piece(Game *g, int x, int y);

int is_black(Piece p);
int is_white(Piece p);
int is_empty(Piece p);

// possible_moves.c

/**
 * \struct Position
 * \brief Representation of a list of chess moves possible for
 * one signular piece \n
 * arr contains coordinates contiguously, example : [x1, y1, x2, y2]
 */
typedef struct
{
	int len;
	int nb;
	int *arr;
} Move_list;

Move_list *init_move_list();
void add_move(Move_list *ml, int x, int y);
void fill_move_list(Game *g, int x, int y, Move_list *ml);
Move_list *possible_moves(char *fen, int x, int y);
void free_move_list(Move_list *ml);
void print_move_list(Move_list *ml);
int player_can_move(Game* g);

// filter_possible_moves.c 
void fill_threatmap(Game* g);
int is_black_king_threatened(Game* g);
int is_white_king_threatened(Game* g);

// ai.c
typedef struct 
{
	Piece p;
	Position pos;
	Move_list* ml;
} Piece_and_moves;


int enemy_player_can_move(Game* g);
int ally_player_can_move(Game* g);