/**
 * \file chessboard.c
 * \brief Source code relating to representation of a chess game
 */

#include "chess.h"

/* General purpose functions */
int is_digit(char c)
{
	return (c > 47) && (c < 58);
}

int parse_int_char(char c)
{
	return c - 48;
}

/**
 * \brief Parses space or null terminated strings
 * and returns the parsed int
 */
int parse_int_string(char *str)
{
	int i = 0, res = 0;
	while ((str[i] != ' ') && (str[i] != '\0'))
	{
		res *= 10;
		res += parse_int_char(str[i]);
		i++;
	}

	return res;
}

/**
 * \brief Return the index of the first character that isn't a space
 */
int skip_spaces_string(char *str, int i)
{
	while (str[i] == ' ')
		i++;
	return i;
}

/* struct Game related functions */

/**
 * \brief Allocates the necessary memory for a Game struct
 *
 * \post Free the allocated memory with the \c free_game_board function
 */
void init_game_board(Game *g)
{
	g->board = malloc(8 * sizeof *g->board);
	for (int i = 0; i < 8; i++)
	{
		g->board[i] = malloc(8 * sizeof *g->board[i]);
	}

	g->enemy_pieces_count = 0;
}

/**
 * \brief Frees the memory allocated by a Game struct
 */
void free_game_board(Game *g)
{
	for (int i = 0; i < 8; i++)
	{
		free(g->board[i]);
	}
	free(g->board);
}

/**
 * \brief Fills the Game's struct board at coordinates \c x, \c y
 * in regards to the FEN chess notation associated with the
 * parsed character
 */
void parse_fen_char_pieces(Game *g, char c, int x, int *y)
{
	if (is_digit(c))
	{
		int tmp = *y + parse_int_char(c);
		for (int i = *y; i < tmp; i++)
		{
			g->board[x][i] = EMPTY;
		}
		*y += parse_int_char(c);
	}
	else
	{
		switch (c)
		{
			case 'p':
				g->board[x][*y] = B_PAWN;
				break;
			case 'r':
				g->board[x][*y] = B_ROOK;
				break;
			case 'n':
				g->board[x][*y] = B_KNIGHT;
				break;
			case 'b':
				g->board[x][*y] = B_BISHOP;
				break;
			case 'q':
				g->board[x][*y] = B_QUEEN;
				break;
			case 'k':
				g->board[x][*y] = B_KING;
				if(g->bool_is_black)
				{
					g->king_pos.x = x;
					g->king_pos.y = *y;
				}
				break;
			case 'P':
				g->board[x][*y] = W_PAWN;
				break;
			case 'R':
				g->board[x][*y] = W_ROOK;
				break;
			case 'N':
				g->board[x][*y] = W_KNIGHT;
				break;
			case 'B':
				g->board[x][*y] = W_BISHOP;
				break;
			case 'Q':
				g->board[x][*y] = W_QUEEN;
				break;
			case 'K':
				g->board[x][*y] = W_KING;
				if(!(g->bool_is_black))
				{
					g->king_pos.x = x;
					g->king_pos.y = *y;
				}
				break;
			default:
				break;
		}

		if 
		(
			(g->bool_is_black && is_white(g->board[x][*y])) ||
			(!(g->bool_is_black) && is_black(g->board[x][*y]))
		)
		{
			g->enemy_pieces[g->enemy_pieces_count].x = x;
			g->enemy_pieces[g->enemy_pieces_count].y = *y;
			g->enemy_pieces_count++;
		}

		*y += 1;
	}
}

// probably optimizable
/**
 * \brief Fills the Game's struct board
 * in regards to the FEN chess notation associated with the
 * parsed string
 */
void parse_fen_string_pieces(Game *g, char *fen_string)
{
	int i = 0, x = 0;
	while (fen_string[i] != ' ')
	{
		int y = 0;
		while ((fen_string[i] != '/') && (fen_string[i] != ' '))
		{
			parse_fen_char_pieces(g, fen_string[i], x, &y);
			i++;
		}
		if (fen_string[i] == ' ')
			return;
		x++;
		i++;
	}
}

/** \brief Fills the Game struct
 * in regards to the FEN chess notation associated with the
 * parsed string
 */
void parse_fen_string(Game *g, char *fen_string)
{
	int i = 0;

	// pieces
	while (fen_string[i] != ' ')
		i++;

	i = skip_spaces_string(fen_string, i);

	// color to play
	g->bool_is_black = (fen_string[i] == 'b');
	i++;

	i = skip_spaces_string(fen_string, i);

	parse_fen_string_pieces(g, fen_string);

	// possible castles KQkq
	g->castles = 0;
	while (fen_string[i] != ' ')
	{
		switch (fen_string[i])
		{
		case 'K':
			g->castles = g->castles | W_KINGSIDE_CASTLE;
			break;
		case 'Q':
			g->castles = g->castles | W_QUEENSIDE_CASTLE;
			break;
		case 'k':
			g->castles = g->castles | B_KINGSIDE_CASTLE;
			break;
		case 'q':
			g->castles = g->castles | B_QUEENSIDE_CASTLE;
			break;
		default:
			break;
		}
		i++;
	}
	i++;

	i = skip_spaces_string(fen_string, i);

	// en passant
	if (fen_string[i] != '-')
	{
		g->en_pass.x = fen_string[i] - 'a';
		i++;
		g->en_pass.y = 8 - parse_int_char(fen_string[i]);
		i++;
	}
	else
	{
		g->en_pass.x = -1;
		g->en_pass.y = -1;
		i += 2;
	}

	i = skip_spaces_string(fen_string, i);

	// half moves -> 50 moves draw rule
	g->half_moves_count = parse_int_string(&(fen_string[i]));
	while (fen_string[i] != ' ')
		i++;

	i = skip_spaces_string(fen_string, i);

	// full moves
	g->full_moves_count = parse_int_string(&(fen_string[i]));
}

void print_board(Game *g)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			printf("%c, \t", PRINT_STR[g->board[i][j]]);
		}
		printf("\n");
	}
}

void print_game(Game *g)
{
	print_board(g);
	printf("player boolean (is_black): %d\n", g->bool_is_black);
	printf("castle: %d\n", g->castles);
	printf("en passant: x = %d y = %d\n", g->en_pass.x, g->en_pass.y);
	printf("half moves: %d\n", g->half_moves_count);
	printf("full moves: %d\n", g->full_moves_count);
	printf("enemy pieces count : %d\n", g->enemy_pieces_count);
	for (int i = 0; i < g->enemy_pieces_count; i++)
	{
		printf("x = %d, y = %d\t", g->enemy_pieces[i].x, g->enemy_pieces[i].y);
	}
	printf("\n");
	printf("king : x = %d, y = %d \n", g->king_pos.x, g->king_pos.y);
}

Piece get_piece(Game *g, int x, int y)
{
	return g->board[y][x];
}

int is_black(Piece p)
{
	return (p & 8);
}

int is_white(Piece p)
{
	return (p < 8);
}

int is_empty(Piece p)
{
	return (p == EMPTY);
}
