#include "chess.h"

void fill_capture_list_black_pawn(Game *g, int x, int y, Move_list *ml)
{
	// captures
	if (x > 0)
		add_move(ml, x - 1, y + 1);

	if (x < 7)
		add_move(ml, x + 1, y + 1);

	// en passant
	if (((x + 1) == g->en_pass.x) && ((y + 1) == g->en_pass.y))
		add_move(ml, x + 1, y + 1);

	if (((x - 1) == g->en_pass.x) && ((y + 1) == g->en_pass.y))
		add_move(ml, x - 1, y + 1);
}

void fill_capture_list_white_pawn(Game *g, int x, int y, Move_list *ml)
{
	// captures
	if (x > 0)
		add_move(ml, x - 1, y - 1);

	if (x < 7)
		add_move(ml, x + 1, y - 1);

	// en passant
	if (((x + 1) == g->en_pass.x) && ((y - 1) == g->en_pass.y))
		add_move(ml, x + 1, y - 1);

	if (((x - 1) == g->en_pass.x) && ((y - 1) == g->en_pass.y))
		add_move(ml, x - 1, y - 1);
}

void fill_capture_list_rook(Game *g, int x, int y, Move_list *ml)
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
		else
		{
			add_move(ml, tmp, y);
			break;
		}
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
		else
		{
			add_move(ml, tmp, y);
			break;
		}
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
		else
		{
			add_move(ml, x, tmp);
			break;
		}
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
		else
		{
			add_move(ml, x, tmp);
			break;
		}
	}
}


void fill_capture_list_knight(int x, int y, Move_list *ml)
{
	int tmp_x = x - 2;
	int tmp_y;

	// left
	if (tmp_x >= 0)
	{
		tmp_y = y - 1;
		if (tmp_y >= 0)
	    	add_move(ml, tmp_x, tmp_y);

		tmp_y = y + 1;
		if (tmp_y <= 7)
            add_move(ml, tmp_x, tmp_y);
	}

	// right
	tmp_x = x + 2;
	if (tmp_x <= 7)
	{
		tmp_y = y - 1;
		if (tmp_y >= 0)
			add_move(ml, tmp_x, tmp_y);

		tmp_y = y + 1;
		if (tmp_y <= 7)
			add_move(ml, tmp_x, tmp_y);
	}

	// up
	tmp_y = y - 2;
	if (tmp_y >= 0)
	{
		tmp_x = x - 1;
		if (tmp_x >= 0)
			add_move(ml, tmp_x, tmp_y);

		tmp_x = x + 1;
		if (tmp_x <= 7)
			add_move(ml, tmp_x, tmp_y);
	}

	// down
	tmp_y = y + 2;
	if (tmp_y <= 7)
	{
		tmp_x = x - 1;
		if (tmp_x >= 0)
			add_move(ml, tmp_x, tmp_y);

		tmp_x = x + 1;
		if (tmp_x <= 7)
			add_move(ml, tmp_x, tmp_y);
	}
}


void fill_capture_list_bishop(Game *g, int x, int y, Move_list *ml)
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
		else
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
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
		else
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
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
		else
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
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
		else
		{
			add_move(ml, tmp_x, tmp_y);
			break;
		}
	}
}

void fill_capture_list_queen(Game *g, int x, int y, Move_list *ml)
{
	fill_capture_list_rook(g, x, y, ml);
	fill_capture_list_bishop(g, x, y, ml);
}

void fill_capture_list_king(int x, int y, Move_list *ml)
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
				if((tmp_y >= 0) && (tmp_y <= 7) && (i != 0 || j != 0))
				{
					add_move(ml, tmp_x, tmp_y);
				}
			}
		}
	}
}

 /** \brief Fills the array of a Move_list struct
  * with the possible captures achievable by the piece at coordinates \c x, \c y
  * in the game described by the fen string
 */
void fill_capture_list(Game *g, int x, int y, Move_list *ml)
{
	if(x == -1)
		return;
	switch (get_piece(g, x, y))
	{
    case W_PAWN:
        fill_capture_list_white_pawn(g, x, y, ml);
		break;
	case B_PAWN:
		fill_capture_list_black_pawn(g, x, y, ml);
		break;
    case W_ROOK:
	case B_ROOK:
		fill_capture_list_rook(g, x, y, ml);
		break;
    case W_KNIGHT:
	case B_KNIGHT:
		fill_capture_list_knight(x, y, ml);
		break;
    case W_BISHOP:
	case B_BISHOP:
		fill_capture_list_bishop(g, x, y, ml);
		break;
    case W_QUEEN:
	case B_QUEEN:
		fill_capture_list_queen(g, x, y, ml);
		break;
    case W_KING:
	case B_KING:
		fill_capture_list_king(x, y, ml);
		break;
	default:
		break;
	}
}

void fill_black_threatmap(Game* g)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            g->black_threatmap[i][j] = 0;

    Move_list *ml = init_move_list();

    // foreach white piece, compute their possible moves (unrestricted by pins and checks)
    for (int i = 0; i < g->white_pieces_count; i++)
    {
        fill_capture_list(g, g->white_pieces[i].x, g->white_pieces[i].y, ml);
        for (int j = 0; j < ml->nb; j += 2)
        {
            // add each of these moves as an increment on the threatmap
            g->black_threatmap[ml->arr[j]][ml->arr[j+1]] += 1;
        }
        ml->nb = 0;
    }

	free_move_list(ml);
}

void fill_white_threatmap(Game* g)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            g->white_threatmap[i][j] = 0;

    Move_list *ml = init_move_list();

    // foreach white piece, compute their possible moves (unrestricted by pins and checks)
    for (int i = 0; i < g->black_pieces_count; i++)
    {
        fill_capture_list(g, g->black_pieces[i].x, g->black_pieces[i].y, ml);
        for (int j = 0; j < ml->nb; j += 2)
        {
            // add each of these moves as an increment on the threatmap
            g->white_threatmap[ml->arr[j]][ml->arr[j+1]] += 1;
        }
        ml->nb = 0;
    }

	free_move_list(ml);
}

void print_black_threatmap(Game* g)
{
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            printf("%d, ", g->black_threatmap[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_white_threatmap(Game* g)
{
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            printf("%d, ", g->white_threatmap[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

// returns n = 0 if not threatened, n > 0 if threatened
int is_black_king_threatened(Game* g)
{
    fill_black_threatmap(g);
    return g->black_threatmap[g->black_king_pos.x][g->black_king_pos.y];
}

int is_white_king_threatened(Game* g)
{
    fill_white_threatmap(g);
    return g->white_threatmap[g->white_king_pos.x][g->white_king_pos.y];
}


// int main()
// {
// 	Game g;

// 	char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
// 	parse_fen_string(&g, fen);
// 	print_game(&g);

//     fill_black_threatmap(&g);
//     fill_white_threatmap(&g);
//     print_black_threatmap(&g);
//     print_white_threatmap(&g);
//     printf("%d, %d\n", is_black_king_threatened(&g), is_white_king_threatened(&g));
	

// 	return 0;
// }