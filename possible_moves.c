#include "chess.h"

// move list for a singular piece
// arr is x and y coordinates one after another
// ew : [x1, y1, x2, y2]
typedef struct
{
	int len;
	int nb;
	int* arr;
} Move_list;

Move_list* init_move_list()
{
	Move_list* res = malloc(sizeof(Move_list));
	res->len = MOVE_LIST_BUFFER_SIZE;
	res->arr = malloc(MOVE_LIST_BUFFER_SIZE * sizeof(int));
	res->nb = 0;
}

void add_move(Move_list* ml, int x, int y)
{
	ml->arr[ml->nb] = x;
	ml->arr[(ml->nb) + 1] = y;
	ml->nb += 2;
	
	if( ((ml->nb) % MOVE_LIST_BUFFER_SIZE) == 0 )
	{
		ml->len += MOVE_LIST_BUFFER_SIZE;
		ml->arr = realloc(ml->arr, ml->len * sizeof(int));
	}
}

void fill_move_list_black_pawn(Game* g, int x, int y, Move_list* ml)
{
	printf("black pawn in x = %d y = %d\n", x, y);
	if(g->board[y+1][x] == EMPTY)
	{
		add_move(ml, x, y+1);
		
		if((y == 1) && (g->board[3][x] == EMPTY))
		{
			add_move(ml, x, y+2);
		}
	}
}

void fill_move_list_black_rook(Game* g, int x, int y, Move_list* ml)
{
	printf("black rook in x = %d y = %d\n", x, y);
}

void fill_move_list_black_knight(Game* g, int x, int y, Move_list* ml)
{
	printf("black knight in x = %d y = %d\n", x, y);
}

void fill_move_list_black_bishop(Game* g, int x, int y, Move_list* ml)
{
	printf("black bishop in x = %d y = %d\n", x, y);
}

void fill_move_list_black_queen(Game* g, int x, int y, Move_list* ml)
{
	printf("black queen in x = %d y = %d\n", x, y);
}

void fill_move_list_black_king(Game* g, int x, int y, Move_list* ml)
{
	printf("black king in x = %d y = %d\n", x, y);
}

void fill_move_list_white_pawn(Game* g, int x, int y, Move_list* ml)
{
	printf("white pawn in x = %d y = %d\n", x, y);
}

void fill_move_list_white_rook(Game* g, int x, int y, Move_list* ml)
{
	printf("white pawn in x = %d y = %d\n", x, y);
}

void fill_move_list_white_knight(Game* g, int x, int y, Move_list* ml)
{
	printf("white pawn in x = %d y = %d\n", x, y);
}

void fill_move_list_white_bishop(Game* g, int x, int y, Move_list* ml)
{
	printf("white pawn in x = %d y = %d\n", x, y);
}

void fill_move_list_white_queen(Game* g, int x, int y, Move_list* ml)
{
	printf("white pawn in x = %d y = %d\n", x, y);
}

void fill_move_list_white_king(Game* g, int x, int y, Move_list* ml)
{
	printf("white pawn in x = %d y = %d\n", x, y);
}

void fill_move_list(Game* g, int x, int y, Move_list* ml)
{
	switch(g->board[y][x])
	{
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
}

Move_list* possible_moves(char* fen, int x, int y)
{
	Game g;
	init_game_board(&g);
	parse_fen_string(&g, fen);
	
	Move_list* res = init_move_list();
	
	fill_move_list(&g, x, y, res);
	
	free_game_board(&g);
	return res;
}

void free_test(Move_list* t)
{
	free(t->arr);
	free(t);
}













