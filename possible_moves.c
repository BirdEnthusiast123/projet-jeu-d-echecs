#include "chess.h"

typedef struct
{
	int len;
	int* arr;
} Test;

Test* wrap_possible_moves(int x, int y)
{
	
	Test* res = malloc(sizeof(Test));
	res->arr = malloc(3 * sizeof(int));
	res->len = 3;
	res->arr[0] = 1;
	res->arr[1] = x;
	res->arr[2] = y;
	
	return res;
}

void free_test(Test* t)
{
	free(t->arr);
	free(t);
}













