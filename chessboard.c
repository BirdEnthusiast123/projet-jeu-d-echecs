#include "chess.h"


int is_digit(char c)
{
	return (c > 47) && (c < 58);
}

int parse_int_char(char c)
{
	return c - 48;
}

// space or null terminated string : ' ' | '\0'
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

int skip_spaces_string(char *str, int i)
{
	while (str[i] == ' ')
		i++;
	return i;
}

// post: free memory with free_game_board
void init_game_board(Game *g)
{
	g->board = (Piece **)malloc(8 * sizeof(Piece *));
	for (int i = 0; i < 8; i++)
	{
		g->board[i] = (Piece *)malloc(8 * sizeof(Piece));
	}
}

void free_game_board(Game *g)
{
	for (int i = 0; i < 8; i++)
	{
		free(g->board[i]);
	}
	free(g->board);
}

void parse_fen_char_pieces(Game *g, char c, int x, int y)
{
	if (is_digit(c))
	{
		int tmp = y + parse_int_char(c);
		for (int i = y; i < tmp; i++)
		{
			g->board[x][i] = EMPTY;
		}
	}
	else
	{
		switch (c)
		{
		case 'p':
			g->board[x][y] = B_PAWN;
			break;
		case 'r':
			g->board[x][y] = B_ROOK;
			break;
		case 'n':
			g->board[x][y] = B_KNIGHT;
			break;
		case 'b':
			g->board[x][y] = B_BISHOP;
			break;
		case 'q':
			g->board[x][y] = B_QUEEN;
			break;
		case 'k':
			g->board[x][y] = B_KING;
			break;
		case 'P':
			g->board[x][y] = W_PAWN;
			break;
		case 'R':
			g->board[x][y] = W_ROOK;
			break;
		case 'N':
			g->board[x][y] = W_KNIGHT;
			break;
		case 'B':
			g->board[x][y] = W_BISHOP;
			break;
		case 'Q':
			g->board[x][y] = W_QUEEN;
			break;
		case 'K':
			g->board[x][y] = W_KING;
			break;
		default:
			break;
		}
	}
}

// probably optimizable
void parse_fen_string_pieces(Game *g, char *fen_string)
{
	int i = 0, x = 0;
	while (fen_string[i] != ' ')
	{
		int y = 0;
		while ((fen_string[i] != '/') && (fen_string[i] != ' '))
		{
			parse_fen_char_pieces(g, fen_string[i], x, y);
			y++;
			i++;
		}
		if (fen_string[i] == ' ')
			return;
		x++;
		i++;
	}
}

void parse_fen_string(Game *g, char *fen_string)
{
	int i = 0;

	// pieces
	parse_fen_string_pieces(g, fen_string);
	while (fen_string[i] != ' ')
		i++;

	i = skip_spaces_string(fen_string, i);

	// color to play
	g->player = (fen_string[i] == 'w') ? white : black;
	i++;

	i = skip_spaces_string(fen_string, i);

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
		g->en_pass.y = parse_int_char(fen_string[i]) - 1;
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
			printf("%d, \t", g->board[i][j]);
		}
		printf("\n");
	}
}

void print_game(Game *g)
{
	print_board(g);
	printf("player: %c\n", g->player);
	printf("castle: %d\n", g->castles);
	printf("en passant: x = %d y = %d\n", g->en_pass.x, g->en_pass.y);
	printf("half moves: %d\n", g->half_moves_count);
	printf("full moves: %d\n", g->full_moves_count);
}

int main()
{
	printf("hey\n");

	Game g;

	init_game_board(&g);

	char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR 0 KQkq - 0 0";
	parse_fen_string(&g, fen);
	print_game(&g);

	free_game_board(&g);
	return 0;
}

