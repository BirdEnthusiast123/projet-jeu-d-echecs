#include "chess.h"

long nb_of_evaluations = 0;

// evaluation is relative to blacks perspective
int evaluate_board(Game* g)
{
    nb_of_evaluations++;
    int evaluation = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (get_piece(g, i, j))
            {
            case W_PAWN:
                evaluation -= 100;
                break;
            case B_PAWN:
                evaluation += 100;
                if(j != 1)
                    evaluation += 50;
                break;
            case W_ROOK:
                evaluation -= 500;
                break;
            case B_ROOK:
                evaluation += 500;
                if(j != 0)
                    evaluation += 50;
                break;
            case W_BISHOP:
            case W_KNIGHT:
                evaluation -= 300;
                break;
            case B_BISHOP:
            case B_KNIGHT:
                evaluation += 300;
                if(j != 0)
                    evaluation += 50;
                break;
            case W_QUEEN:
                evaluation -= 1000;
                break;
            case B_QUEEN:
                evaluation += 1000;
                if(j != 0)
                    evaluation += 50;
                break;
            default:
                break;
            }
        }
    }
    return evaluation;
}

int minimax(Game* g, int depth)
{
    //printf("enemy : %d, ally : %d, depth : %d\n", ally_player_can_move(g), enemy_player_can_move(g), depth);
    if ((depth == 0) || !(player_can_move(g)))
    {
        return evaluate_board(g);
    }

    // compute possible moves
    Piece_and_moves pm;
    pm.ml = init_move_list();

    if (g->bool_is_black)
    {
        int max_value = -100000;
        for (int i = 0; i < g->ally_pieces_count; i++)
        {
            pm.pos.x = g->ally_pieces[i].y;
            pm.pos.y = g->ally_pieces[i].x;
            if(pm.pos.x == -1)
                continue;
            pm.p = get_piece(g, pm.pos.x, pm.pos.y);
            //printf("minimax debug : pm.pos.x = %d, pm.pos.y = %d, piece is %c\n", pm.pos.x, pm.pos.y, PRINT_STR[pm.p]);
            fill_move_list(g, pm.pos.x, pm.pos.y, pm.ml);
            //printf("piece & move list length, p = %c, len = %d\n", PRINT_STR[pm.p], pm.ml->nb);

            for(int j = 0; j < pm.ml->nb; j += 2)
            {
                int new_y = pm.ml->arr[j+1];
                int new_x = pm.ml->arr[j];
                Piece tmp_captured = g->board[new_y][new_x];
                g->board[new_y][new_x] = pm.p;
                g->board[pm.pos.y][pm.pos.x] = EMPTY;

                // temporary change of the piece list
                g->ally_pieces[i].x = new_y;
                g->ally_pieces[i].y = new_x;

                // temporary removal of the captured piece in the pieces list
                int tmp_x = -1, tmp_y = -1, iter = -1;
                if (is_white(tmp_captured)) // change to is_black in the other case
                {
                    iter = 0;
                    while(iter < g->enemy_pieces_count) // change to ally_piece_count in other case
                    {
                        if((g->enemy_pieces[iter].y == new_x) && (g->enemy_pieces[iter].x == new_y))
                        {
                            tmp_x = g->enemy_pieces[iter].x;
                            tmp_y = g->enemy_pieces[iter].y;

                            g->enemy_pieces[iter].x = -1;
                            g->enemy_pieces[iter].y = -1;

                            break;
                        }
                        iter++;
                    }
                }

                // update castles
                int tmp_castles = g->castles, has_castled = 0;
                if(pm.p == B_ROOK)
                {
                    if(pm.pos.x == 0)
                        g->castles = g->castles & (~B_QUEENSIDE_CASTLE);
                    if(pm.pos.x == 7)
                        g->castles = g->castles & (~B_KINGSIDE_CASTLE);
                }

                if(pm.p == B_KING)
                {
                    g->castles = g->castles & (~(B_QUEENSIDE_CASTLE | B_KINGSIDE_CASTLE));
                    switch (new_x - pm.pos.x)
                    {
                    case 2:
                        g->board[0][5] = g->board[0][7];
                        g->board[0][7] = EMPTY;
                        has_castled = B_KINGSIDE_CASTLE;
                        break;
                    case -2:
                        g->board[0][3] = g->board[0][0];
                        g->board[0][0] = EMPTY;
                        has_castled = B_QUEENSIDE_CASTLE;
                        break;
                    default:
                        break;
                    }
                }

                // update en passsant
                int has_captured_en_pass = 0;
                Position tmp_en_pass;
                tmp_en_pass.x = g->en_pass.x;
                tmp_en_pass.y = g->en_pass.y;
                if(pm.p == B_PAWN)
                {
                    if((new_x == g->en_pass.x) && (new_y == g->en_pass.y))
                    {
                        has_captured_en_pass = 1;
                        g->board[new_y - 1][new_x] = EMPTY;
                    }

                    if((new_y - pm.pos.y) == 2)
                    {
                        g->en_pass.x = pm.pos.x;
                        g->en_pass.y = pm.pos.y + 1;
                    }
                    else
                    {
                        g->en_pass.x = -1;
                        g->en_pass.y = -1;
                    }

                    if(new_y == 0)
                    {
                        // promotion, I (incorrectly) asssume promotion will only end up in a queen
                        // sorry knight-promotion-enjoyers :/
                        g->board[new_y][new_x] = B_QUEEN;
                    }
                }
                else
                {
                    g->en_pass.x = -1;
                    g->en_pass.y = -1;
                }

                g->bool_is_black = 0;

                int candidate = minimax(g, depth - 1);
                max_value = (candidate > max_value)? candidate: max_value;

                g->bool_is_black = 1;

                // return to original pos
                g->board[new_y][new_x] = tmp_captured;
                g->board[pm.pos.y][pm.pos.x] = pm.p;

                // former castles state
                g->castles = tmp_castles;
                switch (has_castled)
                {
                case B_KINGSIDE_CASTLE:
                    g->board[0][7] = g->board[0][5];
                    g->board[0][5] = EMPTY;
                    break;
                case B_QUEENSIDE_CASTLE:
                    g->board[0][0] = g->board[0][3];
                    g->board[0][3] = EMPTY;
                    break;
                default:
                    break;
                }

                // former en passant state
                g->en_pass.x = tmp_en_pass.x;
                g->en_pass.y = tmp_en_pass.y;

                if(has_captured_en_pass)
                    g->board[new_y - 1][new_x] = W_PAWN;

                // if piece was captured, restore piece list
                if(iter != -1)
                {
                    g->enemy_pieces[iter].x = tmp_x;
                    g->enemy_pieces[iter].y = tmp_y;
                }

                // former piece list
                g->ally_pieces[i].x = pm.pos.y;
                g->ally_pieces[i].y = pm.pos.x;
            }
            // empty move list
            pm.ml->nb = 0;
        }
        free_move_list(pm.ml);
        return max_value;
    }
    else
    {
        int min_value = 100000;

        for (int i = 0; i < g->enemy_pieces_count; i++)
        {
            pm.pos.x = g->enemy_pieces[i].y;
            pm.pos.y = g->enemy_pieces[i].x;
            if(pm.pos.y == -1)
                continue;
            pm.p = get_piece(g, pm.pos.x, pm.pos.y);
            //printf("minimax debug : pm.pos.x = %d, pm.pos.y = %d, piece is %c\n", pm.pos.x, pm.pos.y, PRINT_STR[pm.p]);
            fill_move_list(g, pm.pos.x, pm.pos.y, pm.ml);
            //printf("piece & move list length, p = %c, len = %d\n", PRINT_STR[pm.p], pm.ml->nb);

            for(int j = 0; j < pm.ml->nb; j += 2)
            {
                int new_y = pm.ml->arr[j+1];
                int new_x = pm.ml->arr[j];
                Piece tmp_captured = g->board[new_y][new_x];
                g->board[new_y][new_x] = pm.p;
                g->board[pm.pos.y][pm.pos.x] = EMPTY;

                // temporary change of the piece list
                g->enemy_pieces[i].x = new_y;
                g->enemy_pieces[i].y = new_x;

                // temporary removal of the captured piece in the pieces list
                int tmp_x = -1, tmp_y = -1, iter = -1;
                if (is_black(tmp_captured)) // change to is_black in the other case
                {
                    iter = 0;
                    while(iter < g->ally_pieces_count) // change to ally_piece_count in other case
                    {
                        if((g->ally_pieces[iter].y == new_x) && (g->ally_pieces[iter].x == new_y))
                        {
                            tmp_x = g->ally_pieces[iter].x;
                            tmp_y = g->ally_pieces[iter].y;

                            g->ally_pieces[iter].x = -1;
                            g->ally_pieces[iter].y = -1;

                            break;
                        }
                        iter++;
                    }
                }

                // update castles
                int tmp_castles = g->castles, has_castled = 0;
                if(pm.p == W_ROOK)
                {
                    if(pm.pos.x == 0)
                        g->castles = g->castles & (~W_QUEENSIDE_CASTLE);
                    if(pm.pos.x == 7)
                        g->castles = g->castles & (~W_KINGSIDE_CASTLE);
                }

                if(pm.p == W_KING)
                {
                    g->castles = g->castles & (~(W_QUEENSIDE_CASTLE | W_KINGSIDE_CASTLE));
                    switch (new_x - pm.pos.x)
                    {
                    case 2:
                        g->board[7][5] = g->board[7][7];
                        g->board[7][7] = EMPTY;
                        has_castled = W_KINGSIDE_CASTLE;
                        break;
                    case -2:
                        g->board[7][3] = g->board[7][0];
                        g->board[7][0] = EMPTY;
                        has_castled = W_QUEENSIDE_CASTLE;
                        break;
                    default:
                        break;
                    }
                }

                // update en passsant
                int has_captured_en_pass = 0;
                Position tmp_en_pass;
                tmp_en_pass.x = g->en_pass.x;
                tmp_en_pass.y = g->en_pass.y;
                if(pm.p == W_PAWN)
                {
                    if((new_x == g->en_pass.x) && (new_y == g->en_pass.y))
                    {
                        has_captured_en_pass = 1;
                        g->board[new_y + 1][new_x] = EMPTY;
                    }

                    if((new_y - pm.pos.y) == -2)
                    {
                        g->en_pass.x = pm.pos.x;
                        g->en_pass.y = pm.pos.y - 1;
                    }
                    else
                    {
                        g->en_pass.x = -1;
                        g->en_pass.y = -1;
                    }

                    if(new_y == 0)
                    {
                        // promotion, I (incorrectly) asssume promotion will only end up in a queen
                        // sorry knight-promotion-enjoyers :/
                        g->board[new_y][new_x] = W_QUEEN;
                    }
                }
                else
                {
                    g->en_pass.x = -1;
                    g->en_pass.y = -1;
                }

                g->bool_is_black = 1;

                int candidate = minimax(g, depth - 1);
                min_value = (candidate < min_value)? candidate: min_value;

                g->bool_is_black = 0;

                // return to original pos
                g->board[new_y][new_x] = tmp_captured;
                g->board[pm.pos.y][pm.pos.x] = pm.p;

                // former castles state
                g->castles = tmp_castles;
                switch (has_castled)
                {
                case W_KINGSIDE_CASTLE:
                    g->board[7][7] = g->board[7][5];
                    g->board[7][5] = EMPTY;
                    break;
                case W_QUEENSIDE_CASTLE:
                    g->board[7][0] = g->board[7][3];
                    g->board[7][3] = EMPTY;
                    break;
                default:
                    break;
                }

                // former en passant state
                g->en_pass.x = tmp_en_pass.x;
                g->en_pass.y = tmp_en_pass.y;

                if(has_captured_en_pass)
                    g->board[new_y - 1][new_x] = B_PAWN;

                // if piece was captured, restore piece list
                if(iter != -1)
                {
                    g->ally_pieces[iter].x = tmp_x;
                    g->ally_pieces[iter].y = tmp_y;
                }

                // former piece list
                g->enemy_pieces[i].x = pm.pos.y;
                g->enemy_pieces[i].y = pm.pos.x;

            }
            // empty move list
            pm.ml->nb = 0;
        }
        free_move_list(pm.ml);
        return min_value;
    }
}

// int main()
// {
// 	Game g;

// 	char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 0";
//     //char* fen2 = "rnb1kbnr/pppp1ppp/8/4p3/4q3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1";
// 	parse_fen_string(&g, fen);
// 	print_game(&g);
	
//     for (int i = 0; i < 7; i++)
//     {
//         printf("minimax = %d\n", minimax(&g, i));
//         printf("nb_eval = %ld\n", nb_of_evaluations);
//     }
    
    
//     printf("evaluate = %d\n", evaluate_board(&g));
    


// 	return 0;
// }
