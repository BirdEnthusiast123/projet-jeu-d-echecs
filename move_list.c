/**
 * \file move_list.c
 * \brief Source code relating to the data structure Move_list
 * where array contains coordinates contiguously, example : [x1, y1, x2, y2]
 * -> simplifies the 'communication' between python and c
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