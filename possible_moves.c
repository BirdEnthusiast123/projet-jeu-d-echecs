#include "chess.h"





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













