#include "chess.h"


Position* get_position()
{
	Position* res = (Position*) malloc(sizeof(Position));
	res->x = rand();
	res->y = rand();
	return res;
}

void free_position(Position* p)
{
	free(p);
}

Move* get_move()
{
	Move* res = (Move*) malloc(sizeof(Move));
	res->piece = rand();
	return res;
}

void free_move(Move* m)
{
	free_position(m->from);
	free_position(m->to);
	free(m);
}

Move_arr* get_move_arr()
{
	Move_arr* res = (Move_arr*) malloc(sizeof(Move_arr));
	res->move_arr = malloc(2 * sizeof(Move));
	res->move_arr[0] = *(get_move());
	res->move_arr[1] = *(get_move());
}












