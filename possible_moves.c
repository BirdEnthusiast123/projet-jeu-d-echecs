/** 
 * \file possible_moves.c
 * \brief Source code relating to the computation of the moves
 * a piece can make (does not account for checks and pins)
 */

#include "chess.h"

 /** \brief Initializes and allocates the memory necessary
  * \post Free the allocated memory with the \c free_move_list function
 */
Move_list *init_move_list()
{
	Move_list *res = malloc(sizeof(Move_list));
	res->len = MOVE_LIST_BUFFER_SIZE;
	res->arr = malloc(MOVE_LIST_BUFFER_SIZE * sizeof(int));
	res->nb = 0;
	return res;
}

 /** \brief Frees the memory allocated by a Move_list struct
 */
void free_move_list(Move_list *ml)
{
	free(ml->arr);
	free(ml);
}

void print_move_list(Move_list *ml)
{
	for (int i = 0; i < ml->nb; i += 2)
	{
		printf("x = %d, y = %d\n", ml->arr[i], ml->arr[i + 1]);
	}
}

 /** \brief Adds the move \c x, \c y to the Move-list struct
 */
void add_move(Move_list *ml, int x, int y)
{
	ml->arr[ml->nb] = x;
	ml->arr[(ml->nb) + 1] = y;
	ml->nb += 2;

	if (((ml->nb) % MOVE_LIST_BUFFER_SIZE) == 0)
	{
		ml->len += MOVE_LIST_BUFFER_SIZE;
		ml->arr = realloc(ml->arr, ml->len * sizeof(int));
	}
}

// post: check for promotion
void fill_move_list_black_pawn(Game *g, int x, int y, Move_list *ml)
{
	// usual moves
	if (get_piece(g, x, y + 1) == EMPTY)
	{
		add_move(ml, x, y + 1);

		if ((y == 1) && (get_piece(g, x, 3) == EMPTY))
		{
			add_move(ml, x, y + 2);
		}
	}

	// captures
	if ((x > 0) && is_white(get_piece(g, x - 1, y + 1)))
		add_move(ml, x - 1, y + 1);

	if ((x < 7) && is_white(get_piece(g, x + 1, y + 1)))
		add_move(ml, x + 1, y + 1);

	// en passant
	if (((x + 1) == g->en_pass.x) && ((y + 1) == g->en_pass.y))
		add_move(ml, x + 1, y + 1);

	if (((x - 1) == g->en_pass.x) && ((y + 1) == g->en_pass.y))
		add_move(ml, x - 1, y + 1);
}

void fill_move_list_black_rook(Game *g, int x, int y, Move_list *ml)
{
	// up
	int tmp = x - 1;
	while (tmp >= 0)
	{
		Piece p = get_piece(g, tmp, y);
		if (is_empty(p))
		{
			add_move(ml, tmp, y);
			tmp--;
		}
		else if (is_white(p))
		{
			add_move(ml, tmp, y);
			break;
		}
		else
			break; // is_black == true
	}

	// down
	tmp = x + 1;
	while (tmp <= 7)
	{
		Piece p = get_piece(g, tmp, y);
		if (is_empty(p))
		{
			add_move(ml, tmp, y);
			tmp++;
		}
		else if (is_white(p))
		{
			add_move(ml, tmp, y);
			break;
		}
		else
			break; // is_black == true
	}

	// left
	tmp = y - 1;
	while (tmp >= 0)
	{
		Piece p = get_piece(g, x, tmp);
		if (is_empty(p))
		{
			add_move(ml, x, tmp);
			tmp--;
		}
		else if (is_white(p))
		{
			add_move(ml, x, tmp);
			break;
		}
		else
			break; // is_black == true
	}

	// right
	tmp = y + 1;
	while (tmp <= 7)
	{
		Piece p = get_piece(g, x, tmp);
		if (is_empty(p))
		{
			add_move(ml, x, tmp);
			tmp++;
		}
		else if (is_white(p))
		{
			add_move(ml, x, tmp);
			break;
		}
		else
			break; // is_black == true
	}
}

void fill_move_list_black_knight(Game *g, int x, int y, Move_list *ml)
{
	int tmp_x = x - 2;
	int tmp_y;
	Piece tmp_p;

	// left
	if (tmp_x >= 0)
	{
		tmp_y = y - 1;
		if (tmp_y >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}

	// right
	tmp_x = x + 2;
	if (tmp_x <= 7)
	{
		tmp_y = y - 1;
		if (tmp_y >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}

	// up
	tmp_y = y - 2;
	if (tmp_y >= 0)
	{
		tmp_x = x - 1;
		if (tmp_x >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}

	// down
	tmp_y = y + 2;
	if (tmp_y <= 7)
	{
		tmp_x = x - 1;
		if (tmp_x >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_white(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}
}

void fill_move_list_black_bishop(Game *g, int x, int y, Move_list *ml)
{
	int tmp_x = x + 1, tmp_y = y + 1;
	Piece p;
	// Sout east
	while ((tmp_x <= 7) && (tmp_y <= 7))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x++;
			tmp_y++;
		}
		else if (is_white(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}

	// South west
	tmp_x = x - 1;
	tmp_y = y + 1;
	while ((tmp_x >= 0) && (tmp_y <= 7))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x--;
			tmp_y++;
		}
		else if (is_white(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}

	// North east
	tmp_x = x + 1;
	tmp_y = y - 1;
	while ((tmp_x <= 7) && (tmp_y >= 0))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x++;
			tmp_y--;
		}
		else if (is_white(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}

	// North west
	tmp_x = x - 1;
	tmp_y = y - 1;
	while ((tmp_x >= 0) && (tmp_y >= 0))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x--;
			tmp_y--;
		}
		else if (is_white(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}
}

void fill_move_list_black_queen(Game *g, int x, int y, Move_list *ml)
{
	fill_move_list_black_rook(g, x, y, ml);
	fill_move_list_black_bishop(g, x, y, ml);
}

void fill_move_list_black_king(Game *g, int x, int y, Move_list *ml)
{
	int tmp_x, tmp_y;
	for (int i = -1; i <= 1; i++)
	{
		tmp_x = x + i;
		if ((tmp_x >= 0) && (tmp_x <= 7))
		{
			for (int j = -1; j <= 1; j++)
			{
				tmp_y = y + j;
				if
				(
					(tmp_y >= 0) &&
					(tmp_y <= 7) &&
					(
						is_empty(get_piece(g, tmp_x, tmp_y)) ||
						is_white(get_piece(g, tmp_x, tmp_y))
					)
				)
				{
					add_move(ml, tmp_x, tmp_y);
				}
			}
		}
	}
}

// post: check for promotion
void fill_move_list_white_pawn(Game *g, int x, int y, Move_list *ml)
{
	// usual moves
	if (get_piece(g, x, y - 1) == EMPTY)
	{
		add_move(ml, x, y - 1);

		if ((y == 6) && (get_piece(g, x, 4) == EMPTY))
		{
			add_move(ml, x, y - 2);
		}
	}

	// captures
	if ((x > 0) && is_black(get_piece(g, x - 1, y - 1)))
		add_move(ml, x - 1, y - 1);

	if ((x < 7) && is_black(get_piece(g, x + 1, y - 1)))
		add_move(ml, x + 1, y - 1);

	// en passant
	if (((x + 1) == g->en_pass.x) && ((y - 1) == g->en_pass.y))
		add_move(ml, x + 1, y - 1);

	if (((x - 1) == g->en_pass.x) && ((y - 1) == g->en_pass.y))
		add_move(ml, x - 1, y - 1);
}

void fill_move_list_white_rook(Game *g, int x, int y, Move_list *ml)
{
	// up
	int tmp = x - 1;
	while (tmp >= 0)
	{
		Piece p = get_piece(g, tmp, y);
		if (is_empty(p))
		{
			add_move(ml, tmp, y);
			tmp--;
		}
		else if (is_black(p))
		{
			add_move(ml, tmp, y);
			break;
		}
		else
			break; // is_white == true
	}

	// down
	tmp = x + 1;
	while (tmp <= 7)
	{
		Piece p = get_piece(g, tmp, y);
		if (is_empty(p))
		{
			add_move(ml, tmp, y);
			tmp++;
		}
		else if (is_black(p))
		{
			add_move(ml, tmp, y);
			break;
		}
		else
			break; // is_white == true
	}

	// left
	tmp = y - 1;
	while (tmp >= 0)
	{
		Piece p = get_piece(g, x, tmp);
		if (is_empty(p))
		{
			add_move(ml, x, tmp);
			tmp--;
		}
		else if (is_black(p))
		{
			add_move(ml, x, tmp);
			break;
		}
		else
			break; // is_white == true
	}

	// right
	tmp = y + 1;
	while (tmp <= 7)
	{
		Piece p = get_piece(g, x, tmp);
		if (is_empty(p))
		{
			add_move(ml, x, tmp);
			tmp++;
		}
		else if (is_black(p))
		{
			add_move(ml, x, tmp);
			break;
		}
		else
			break; // is_white == true
	}
}

void fill_move_list_white_knight(Game *g, int x, int y, Move_list *ml)
{
	int tmp_x = x - 2;
	int tmp_y;
	Piece tmp_p;

	// left
	if (tmp_x >= 0)
	{
		tmp_y = y - 1;
		if (tmp_y >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}

	// right
	tmp_x = x + 2;
	if (tmp_x <= 7)
	{
		tmp_y = y - 1;
		if (tmp_y >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}

	// up
	tmp_y = y - 2;
	if (tmp_y >= 0)
	{
		tmp_x = x - 1;
		if (tmp_x >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}

	// down
	tmp_y = y + 2;
	if (tmp_y <= 7)
	{
		tmp_x = x - 1;
		if (tmp_x >= 0)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (is_empty(tmp_p) || is_black(tmp_p))
				add_move(ml, tmp_x, tmp_y);
		}
	}
}

void fill_move_list_white_bishop(Game *g, int x, int y, Move_list *ml)
{
	int tmp_x = x + 1, tmp_y = y + 1;
	Piece p;
	// Sout east
	while ((tmp_x <= 7) && (tmp_y <= 7))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x++;
			tmp_y++;
		}
		else if (is_black(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}

	// South west
	tmp_x = x - 1;
	tmp_y = y + 1;
	while ((tmp_x >= 0) && (tmp_y <= 7))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x--;
			tmp_y++;
		}
		else if (is_black(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}

	// North east
	tmp_x = x + 1;
	tmp_y = y - 1;
	while ((tmp_x <= 7) && (tmp_y >= 0))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x++;
			tmp_y--;
		}
		else if (is_black(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}

	// North west
	tmp_x = x - 1;
	tmp_y = y - 1;
	while ((tmp_x >= 0) && (tmp_y >= 0))
	{
		p = get_piece(g, tmp_x, tmp_y);

		if (is_empty(p))
		{
			add_move(ml, tmp_x, tmp_y);
			tmp_x--;
			tmp_y--;
		}
		else if (is_black(p))
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
		else
			break;
	}
}

void fill_move_list_white_queen(Game *g, int x, int y, Move_list *ml)
{
	fill_move_list_white_rook(g, x, y, ml);

	fill_move_list_white_bishop(g, x, y, ml);
}

void fill_move_list_white_king(Game *g, int x, int y, Move_list *ml)
{
	int tmp_x, tmp_y;
	for (int i = -1; i <= 1; i++)
	{
		tmp_x = x + i;
		if ((tmp_x >= 0) && (tmp_x <= 7))
		{
			for (int j = -1; j <= 1; j++)
			{
				tmp_y = y + j;
				if
				(
					(tmp_y >= 0) &&
					(tmp_y <= 7) &&
					(
						is_empty(get_piece(g, tmp_x, tmp_y)) ||
						is_black(get_piece(g, tmp_x, tmp_y))
					)
				)
				{
					add_move(ml, tmp_x, tmp_y);
				}
			}
		}
	}
}

 /** \brief Fills the array of a Move_list struct
  * with the possible moves achievable by the piece at coordinates \c x, \c y
  * in the game described by the fen string
 */
void fill_move_list(Game *g, int x, int y, Move_list *ml)
{
	switch (g->board[y][x])
	{
	case B_PAWN:
		fill_move_list_black_pawn(g, x, y, ml);
		break;
	case B_ROOK:
		fill_move_list_black_rook(g, x, y, ml);
		break;
	case B_KNIGHT:
		fill_move_list_black_knight(g, x, y, ml);
		break;
	case B_BISHOP:
		fill_move_list_black_bishop(g, x, y, ml);
		break;
	case B_QUEEN:
		fill_move_list_black_queen(g, x, y, ml);
		break;
	case B_KING:
		fill_move_list_black_king(g, x, y, ml);
		break;
	case W_PAWN:
		fill_move_list_white_pawn(g, x, y, ml);
		break;
	case W_ROOK:
		fill_move_list_white_rook(g, x, y, ml);
		break;
	case W_KNIGHT:
		fill_move_list_white_knight(g, x, y, ml);
		break;
	case W_BISHOP:
		fill_move_list_white_bishop(g, x, y, ml);
		break;
	case W_QUEEN:
		fill_move_list_white_queen(g, x, y, ml);
		break;
	case W_KING:
		fill_move_list_white_king(g, x, y, ml);
		break;
	default:
		break;
	}
	print_move_list(ml);
}

 /** \brief Initializes and fills the array of a Move_list struct
  * with the possible moves achievable by the piece at coordinates \c x, \c y
  * in the game described by the fen string
 */
Move_list *possible_moves(char *fen, int x, int y)
{
	Game g;
	init_game_board(&g);
	parse_fen_string(&g, fen);

	Move_list *res = init_move_list();

	fill_move_list(&g, x, y, res);

	free_game_board(&g);
	return res;
}


