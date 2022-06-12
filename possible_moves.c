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

void add_move_if_not_threatening_king
(
	Game* g, 
	Move_list *ml, 
	int x, int y, 
	int new_x, int new_y, 
	Piece p
)
{
	Piece tmp_p = g->board[new_y][new_x];
	int tmp_x, tmp_y, i = -1;

	g->board[new_y][new_x] = p;
	g->board[y][x] = EMPTY;

	// temporary removal of the captured piece in the enemy_pieces list
	if
	(
		(g->bool_is_black && is_white(tmp_p)) ||
		(!(g->bool_is_black) && is_black(tmp_p))
	)
	{
		i = 0;
		while(i < g->enemy_pieces_count)
		{
			if((g->enemy_pieces[i].y == new_x) && (g->enemy_pieces[i].x == new_y))
			{
				tmp_x = g->enemy_pieces[i].x;
				tmp_y = g->enemy_pieces[i].y;

				g->enemy_pieces[i].x = -1;
				g->enemy_pieces[i].y = -1;

				break;
			}
			i++;
		}
	}

	if((g->bool_is_black) && (is_black_king_threatened(g) == 0))
		add_move(ml, new_x, new_y);
	else if ((!(g->bool_is_black)) && (is_white_king_threatened(g) == 0))
		add_move(ml, new_x, new_y);

	if((i != -1) && (i != 16))
	{
		g->enemy_pieces[i].x = tmp_x;
		g->enemy_pieces[i].y = tmp_y;
	}
	
	g->board[new_y][new_x] = tmp_p;
	g->board[y][x] = p;
}

// post: check for promotion
void fill_move_list_black_pawn(Game *g, int x, int y, Move_list *ml)
{
	// usual moves
	if (get_piece(g, x, y + 1) == EMPTY)
	{
		add_move_if_not_threatening_king(g, ml, x, y, x, y+1, B_PAWN);

		if ((y == 1) && (get_piece(g, x, 3) == EMPTY))
		{
			add_move_if_not_threatening_king(g, ml, x, y, x, y+2, B_PAWN);
		}
	}

	// captures
	if ((x > 0) && is_white(get_piece(g, x - 1, y + 1)))
	{
		add_move_if_not_threatening_king(g, ml, x, y, x-1, y+1, B_PAWN);
	}

	if ((x < 7) && is_white(get_piece(g, x + 1, y + 1)))
	{
		add_move_if_not_threatening_king(g, ml, x, y, x+1, y+1, B_PAWN);
	}

	// en passant
	if (((x + 1) == g->en_pass.x) && ((y + 1) == g->en_pass.y))
	{
		g->board[y][x+1] = EMPTY;
		add_move_if_not_threatening_king(g, ml, x, y, x+1, y+1, B_PAWN);
		g->board[y][x+1] = W_PAWN;
	}

	if (((x - 1) == g->en_pass.x) && ((y + 1) == g->en_pass.y))
	{
		g->board[y][x-1] = EMPTY;
		add_move_if_not_threatening_king(g, ml, x, y, x-1, y+1, B_PAWN);
		g->board[y][x-1] = W_PAWN;
	}
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, B_ROOK);
			tmp--;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, B_ROOK);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, B_ROOK);
			tmp++;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, B_ROOK);
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
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, B_ROOK);
			tmp--;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, B_ROOK);
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
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, B_ROOK);
			tmp++;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, B_ROOK);
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
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
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
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
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
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
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
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_black(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KNIGHT);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
			tmp_x++;
			tmp_y++;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
			tmp_x--;
			tmp_y++;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
			tmp_x++;
			tmp_y--;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
			tmp_x--;
			tmp_y--;
		}
		else if (is_white(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_BISHOP);
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
	g->board[y][x] = B_QUEEN;
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
					(!(is_black(get_piece(g, tmp_x, tmp_y))))
				)
				{
					int x_mem = g->black_king_pos.x, y_mem = g->black_king_pos.y;
					g->black_king_pos.x = tmp_y;
					g->black_king_pos.y = tmp_x;
					add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KING);
					g->black_king_pos.x = x_mem;
					g->black_king_pos.y = y_mem;
				}
			}
		}
	}

	fill_threatmap(g);

	//kingside castle
	if
	(
		(g->castles & B_KINGSIDE_CASTLE)&&
		(is_empty(g->board[0][6]))&&
		(is_empty(g->board[0][5]))&&
		(g->threatmap[0][6] == 0)&&
		(g->threatmap[0][5] == 0)&&
		(g->threatmap[0][4] == 0)
	)
	{
		add_move(ml, 6, 0);
	}

	//queenside castle
	if
	(
		(g->castles & B_QUEENSIDE_CASTLE)&&
		(is_empty(g->board[0][1]))&&
		(is_empty(g->board[0][2]))&&
		(is_empty(g->board[0][3]))&&
		(g->threatmap[0][2] == 0)&&
		(g->threatmap[0][3] == 0)&&
		(g->threatmap[0][4] == 0)
	)
	{
		add_move(ml, 2, 0);
	}
}

// post: check for promotion
void fill_move_list_white_pawn(Game *g, int x, int y, Move_list *ml)
{
	// usual moves
	if (get_piece(g, x, y - 1) == EMPTY)
	{
		add_move_if_not_threatening_king(g, ml, x, y, x, y - 1, W_PAWN);

		if ((y == 6) && (get_piece(g, x, 4) == EMPTY))
		{
			add_move_if_not_threatening_king(g, ml, x, y, x, y - 2, W_PAWN);
		}
	}

	// captures
	if ((x > 0) && is_black(get_piece(g, x - 1, y - 1)))
		add_move_if_not_threatening_king(g, ml, x, y, x - 1, y - 1, W_PAWN);

	if ((x < 7) && is_black(get_piece(g, x + 1, y - 1)))
		add_move_if_not_threatening_king(g, ml, x, y, x + 1, y - 1, W_PAWN);

	// en passant
	if (((x + 1) == g->en_pass.x) && ((y - 1) == g->en_pass.y))
	{
		g->board[y][x+1] = EMPTY;
		add_move_if_not_threatening_king(g, ml, x, y, x + 1, y - 1, W_PAWN);
		g->board[y][x+1] = B_PAWN;
	}

	if (((x - 1) == g->en_pass.x) && ((y - 1) == g->en_pass.y))
	{
		g->board[y][x-1] = EMPTY;
		add_move_if_not_threatening_king(g, ml, x, y, x - 1, y - 1, W_PAWN);
		g->board[y][x-1] = B_PAWN;
	}
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, W_ROOK);
			tmp--;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, W_ROOK);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, W_ROOK);
			tmp++;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp, y, W_ROOK);
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
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, W_ROOK);
			tmp--;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, W_ROOK);
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
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, W_ROOK);
			tmp++;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, x, tmp, W_ROOK);
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
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
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
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
		}

		tmp_y = y + 1;
		if (tmp_y <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
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
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
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
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
		}

		tmp_x = x + 1;
		if (tmp_x <= 7)
		{
			tmp_p = get_piece(g, tmp_x, tmp_y);
			if (!(is_white(tmp_p)))
				add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KNIGHT);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
			tmp_x++;
			tmp_y++;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
			tmp_x--;
			tmp_y++;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
			tmp_x++;
			tmp_y--;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
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
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
			tmp_x--;
			tmp_y--;
		}
		else if (is_black(p))
		{
			add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_BISHOP);
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
	g->board[y][x] = W_QUEEN;
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
				if (
					(tmp_y >= 0) &&
					(tmp_y <= 7) &&
					((!is_white(get_piece(g, tmp_x, tmp_y)))))
				{
					int x_mem = g->white_king_pos.x, y_mem = g->white_king_pos.y;
					g->white_king_pos.x = tmp_y;
					g->white_king_pos.y = tmp_x;
					add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KING);
					g->white_king_pos.x = x_mem;
					g->white_king_pos.y = y_mem;
				}
			}
		}
	}

	fill_threatmap(g);

	//kingside castle
	if
	(
		(g->castles & W_KINGSIDE_CASTLE)&&
		(is_empty(g->board[7][6]))&&
		(is_empty(g->board[7][5]))&&
		(g->threatmap[7][6] == 0)&&
		(g->threatmap[7][5] == 0)&&
		(g->threatmap[7][4] == 0)
	)
	{
		add_move(ml, 6, 7);
	}

	//queenside castle
	if
	(
		(g->castles & W_QUEENSIDE_CASTLE)&&
		(is_empty(g->board[7][1]))&&
		(is_empty(g->board[7][2]))&&
		(is_empty(g->board[7][3]))&&
		(g->threatmap[7][2] == 0)&&
		(g->threatmap[7][3] == 0)&&
		(g->threatmap[7][4] == 0)
	)
	{
		add_move(ml, 2, 7);
	}
}

/** \brief Fills the array of a Move_list struct
 * with the possible moves achievable by the piece at coordinates \c x, \c y
 * in the game described by the fen string
 */
void fill_move_list(Game *g, int x, int y, Move_list *ml)
{
	if(x == -1)
		return;
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
}

/** \brief Initializes and fills the array of a Move_list struct
 * with the possible moves achievable by the piece at coordinates \c x, \c y
 * in the game described by the fen string
 */
Move_list *possible_moves(char *fen, int x, int y)
{
	Game g;
	parse_fen_string(&g, fen);

	Move_list *res = init_move_list();

	fill_move_list(&g, x, y, res);

	return res;
}

int ally_player_can_move(Game* g)
{
	Move_list *ml = init_move_list();

    for (int i = 0; i < g->ally_pieces_count; i++)
    {
        fill_move_list(g, g->ally_pieces[i].y, g->ally_pieces[i].x, ml);
        if(ml->nb != 0) 
		{
			free_move_list(ml);
			return 1;
		}
        ml->nb = 0;
    }

	free_move_list(ml);
	return 0;
}

int enemy_player_can_move(Game* g)
{
	Move_list *ml = init_move_list();

	for (int i = 0; i < g->enemy_pieces_count; i++)
	{
		fill_move_list(g, g->enemy_pieces[i].y, g->enemy_pieces[i].x, ml);
		if(ml->nb != 0) 
		{
			free_move_list(ml);
			return 1;
		}
		ml->nb = 0;
	}

	free_move_list(ml);
	return 0;
}

int player_can_move(Game* g)
{
	return ally_player_can_move(g) && enemy_player_can_move(g);
}


