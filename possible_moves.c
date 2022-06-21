/**
 * \file possible_moves.c
 * \brief Source code relating to the computation of the moves
 * a piece can make
 */

#include "chess.h"

void add_move_if_not_threatening_king
(
	Game* g, 
	Move_list *ml, 
	int x, int y, 
	int new_x, int new_y, 
	Piece p
)
{
	Piece tmp_p = get_piece(g, new_x, new_y);
    set_piece(g, new_x, new_y, p);
    set_piece(g, x, y, EMPTY);

	// temporary removal of the captured piece in the enemy_pieces list
	int tmp_x, tmp_y, i = -1;
	if(g->bool_is_black && is_white(tmp_p))
	{
		i = 0;
		while(i < g->white_pieces_count)
		{
			if((g->white_pieces[i].x == new_x) && (g->white_pieces[i].y == new_y))
			{
				tmp_x = g->white_pieces[i].x;
				tmp_y = g->white_pieces[i].y;

				g->white_pieces[i].x = -1;
				g->white_pieces[i].y = -1;

				break;
			}
			i++;
		}
	}
    else if (!(g->bool_is_black) && is_black(tmp_p))
    {
        i = 0;
		while(i < g->black_pieces_count)
		{
			if((g->black_pieces[i].x == new_x) && (g->black_pieces[i].y == new_y))
			{
				tmp_x = g->black_pieces[i].x;
				tmp_y = g->black_pieces[i].y;

				g->black_pieces[i].x = -1;
				g->black_pieces[i].y = -1;

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
        if(g->bool_is_black)
        {
            g->white_pieces[i].x = tmp_x;
            g->white_pieces[i].y = tmp_y;
        }
        else
        {
            g->black_pieces[i].x = tmp_x;
            g->black_pieces[i].y = tmp_y;
        }
	}
	
    set_piece(g, new_x, new_y, tmp_p);
    set_piece(g, x, y, p);
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
        set_piece(g, x + 1, y, EMPTY);
		add_move_if_not_threatening_king(g, ml, x, y, x+1, y+1, B_PAWN);
        set_piece(g, x + 1, y, W_PAWN);
	}

	if (((x - 1) == g->en_pass.x) && ((y + 1) == g->en_pass.y))
	{
		set_piece(g, x - 1, y, EMPTY);
		add_move_if_not_threatening_king(g, ml, x, y, x-1, y+1, B_PAWN);
		set_piece(g, x - 1, y, W_PAWN);
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
    // function above changes the piece quality to a bishop
    // reseting piece quality to queen
    set_piece(g, x, y, B_QUEEN);
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
					g->black_king_pos.x = tmp_x;
					g->black_king_pos.y = tmp_y;
					add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, B_KING);
					g->black_king_pos.x = x_mem;
					g->black_king_pos.y = y_mem;
				}
			}
		}
	}

	fill_black_threatmap(g);

	//kingside castle
	if
	(
		(g->castles & B_KINGSIDE_CASTLE)&&
        (is_empty(get_piece(g, 6, 0)))&&
        (is_empty(get_piece(g, 5, 0)))&&
		(g->black_threatmap[6][0] == 0)&&
		(g->black_threatmap[5][0] == 0)&&
		(g->black_threatmap[4][0] == 0)
	)
	{
		add_move(ml, 6, 0);
	}

	//queenside castle
	if
	(
		(g->castles & B_QUEENSIDE_CASTLE)&&
        (is_empty(get_piece(g, 1, 0)))&&
        (is_empty(get_piece(g, 2, 0)))&&
        (is_empty(get_piece(g, 3, 0)))&&
		(g->black_threatmap[2][0] == 0)&&
		(g->black_threatmap[3][0] == 0)&&
		(g->black_threatmap[4][0] == 0)
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
        set_piece(g, x + 1, y, EMPTY);
		add_move_if_not_threatening_king(g, ml, x, y, x + 1, y - 1, W_PAWN);
		set_piece(g, x + 1, y, B_PAWN);
	}

	if (((x - 1) == g->en_pass.x) && ((y - 1) == g->en_pass.y))
	{
		set_piece(g, x - 1, y, EMPTY);
		add_move_if_not_threatening_king(g, ml, x, y, x - 1, y - 1, W_PAWN);
		set_piece(g, x - 1, y, B_PAWN);
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
    // function above changes the piece quality to a bishop
    // reseting piece quality to queen
    set_piece(g, x, y, W_QUEEN);
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
					g->white_king_pos.x = tmp_x;
					g->white_king_pos.y = tmp_y;
					add_move_if_not_threatening_king(g, ml, x, y, tmp_x, tmp_y, W_KING);
					g->white_king_pos.x = x_mem;
					g->white_king_pos.y = y_mem;
				}
			}
		}
	}

	fill_white_threatmap(g);

	//kingside castle
	if
	(
		(g->castles & W_KINGSIDE_CASTLE)&&
        (is_empty(get_piece(g, 6, 7)))&&
        (is_empty(get_piece(g, 5, 7)))&&
		(g->white_threatmap[6][7] == 0)&&
		(g->white_threatmap[5][7] == 0)&&
		(g->white_threatmap[4][7] == 0)
	)
	{
		add_move(ml, 6, 7);
	}

	//queenside castle
	if
	(
		(g->castles & W_QUEENSIDE_CASTLE)&&
        (is_empty(get_piece(g, 1, 7)))&&
        (is_empty(get_piece(g, 2, 7)))&&
        (is_empty(get_piece(g, 3, 7)))&&
		(g->white_threatmap[2][7] == 0)&&
		(g->white_threatmap[3][7] == 0)&&
		(g->white_threatmap[4][7] == 0)
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
	switch (get_piece(g, x, y))
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
 * \post free the allocated memory with free_move_list
 */
Move_list *possible_moves(char *fen, int x, int y)
{
	Game g;
	parse_fen_string(&g, fen);

	Move_list *res = init_move_list();

	fill_move_list(&g, x, y, res);

	return res;
}

int black_can_move(Game* g)
{
    Move_list *ml = init_move_list();

    for (int i = 0; i < g->black_pieces_count; i++)
    {
        fill_move_list(g, g->black_pieces[i].x, g->black_pieces[i].y, ml);
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

int white_can_move(Game* g)
{
    Move_list *ml = init_move_list();

    for (int i = 0; i < g->white_pieces_count; i++)
    {
        fill_move_list(g, g->white_pieces[i].x, g->white_pieces[i].y, ml);
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
    return (g->bool_is_black)? black_can_move(g): white_can_move(g);
}
