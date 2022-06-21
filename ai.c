#include <time.h> 
#include "chess.h"

// evaluation is relative to blacks perspective
int evaluate_board(Game* g)
{
    if(!(white_can_move(g)) && is_white_king_threatened(g))
        return 10000;
    else if(!(black_can_move(g)) && is_black_king_threatened(g))
        return -10000;
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
                evaluation += (j != 1)? 5: 0;
                break;
            case W_ROOK:
                evaluation -= 500;
                break;
            case B_ROOK:
                evaluation += 500;
                evaluation += (j != 0)? 5: 0;
                break;
            case W_BISHOP:
            case W_KNIGHT:
                evaluation -= 300;
                break;
            case B_BISHOP:
            case B_KNIGHT:
                evaluation += 300;
                evaluation += (j != 0)? 5: 0;
                break;
            case W_QUEEN:
                evaluation -= 1000;
                break;
            case B_QUEEN:
                evaluation += 1000;
                evaluation += (j != 0)? 5: 0;
                break;
            default:
                break;
            }
        }
    }
    return evaluation;
}

int minimax(Game* g, int depth, Ai_move* ai_m)
{
    if ((depth == 0) || !(player_can_move(g)))
    {
        return evaluate_board(g);
    }

    Piece_and_moves pm;
    pm.ml = init_move_list();

    if (g->bool_is_black)
    {
        int max_value = -100000;
        for (int i = 0; i < g->black_pieces_count; i++)
        {
            pm.pos.x = g->black_pieces[i].x;
            pm.pos.y = g->black_pieces[i].y;
            if(pm.pos.x == -1)
                continue;
            pm.p = get_piece(g, pm.pos.x, pm.pos.y);
            fill_move_list(g, pm.pos.x, pm.pos.y, pm.ml);

            for(int j = 0; j < pm.ml->nb; j += 2)
            {
                int new_x = pm.ml->arr[j];
                int new_y = pm.ml->arr[j+1];
                Piece tmp_captured = get_piece(g, new_x, new_y);
                set_piece(g, new_x, new_y, pm.p);
                set_piece(g, pm.pos.x, pm.pos.y, EMPTY);

                // temporary change of the piece list
                g->black_pieces[i].x = new_x;
                g->black_pieces[i].y = new_y;

                // temporary removal of the captured piece in the pieces list
                int tmp_x = -1, tmp_y = -1, iter = -1;
                if (is_white(tmp_captured))
                {
                    iter = 0;
                    while(iter < g->white_pieces_count)
                    {
                        if((g->white_pieces[iter].x == new_x) && (g->white_pieces[iter].y == new_y))
                        {
                            tmp_x = g->white_pieces[iter].x;
                            tmp_y = g->white_pieces[iter].y;

                            g->white_pieces[iter].x = -1;
                            g->white_pieces[iter].y = -1;

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
                        set_piece(g, 5, 0, B_ROOK);
                        set_piece(g, 7, 0, EMPTY);
                        has_castled = B_KINGSIDE_CASTLE;
                        break;
                    case -2:
                        set_piece(g, 3, 0, B_ROOK);
                        set_piece(g, 0, 0, EMPTY);
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
                        set_piece(g, new_x, new_y - 1, EMPTY);
                        // temporary removal of the captured piece in the pieces list
                        iter = 0;
                        while(iter < g->white_pieces_count)
                        {
                            if((g->white_pieces[iter].x == new_x) && (g->white_pieces[iter].y == (new_y - 1)))
                            {
                                tmp_x = g->white_pieces[iter].x;
                                tmp_y = g->white_pieces[iter].y;

                                g->white_pieces[iter].x = -1;
                                g->white_pieces[iter].y = -1;

                                break;
                            }
                            iter++;
                        }
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

                    if(new_y == 7)
                    {
                        // promotion, I (incorrectly) asssume promotion will only end up in a queen
                        // sorry knight-promotion-enjoyers :/
                        set_piece(g, new_x, new_y, B_QUEEN);
                    }
                }
                else
                {
                    g->en_pass.x = -1;
                    g->en_pass.y = -1;
                }

                g->bool_is_black = 0;

                int candidate = minimax(g, depth - 1, ai_m);
                if(candidate > max_value)
                {
                    max_value = candidate;
                    if(depth == ARBITRARY_DEPTH)
                    {
                        ai_m->x1 = pm.pos.x;
                        ai_m->y1 = pm.pos.y;
                        ai_m->x2 = new_x;
                        ai_m->y2 = new_y;
                    }
                }

                g->bool_is_black = 1;

                // return to original pos
                set_piece(g, new_x, new_y, tmp_captured);
                set_piece(g, pm.pos.x, pm.pos.y, pm.p);

                // former castles state
                g->castles = tmp_castles;
                switch (has_castled)
                {
                case B_KINGSIDE_CASTLE:
                    set_piece(g, 7, 0, B_ROOK);
                    set_piece(g, 5, 0, EMPTY);
                    break;
                case B_QUEENSIDE_CASTLE:
                    set_piece(g, 0, 0, B_ROOK);
                    set_piece(g, 3, 0, EMPTY);
                    break;
                default:
                    break;
                }

                // former en passant state
                g->en_pass.x = tmp_en_pass.x;
                g->en_pass.y = tmp_en_pass.y;

                if(has_captured_en_pass)
                    set_piece(g, new_x, new_y - 1, W_PAWN);

                // if piece was captured, restore piece list
                if(iter != -1)
                {
                    g->white_pieces[iter].x = tmp_x;
                    g->white_pieces[iter].y = tmp_y;
                }

                // former piece list
                g->black_pieces[i].x = pm.pos.x;
                g->black_pieces[i].y = pm.pos.y;
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

        for (int i = 0; i < g->white_pieces_count; i++)
        {
            pm.pos.x = g->white_pieces[i].x;
            pm.pos.y = g->white_pieces[i].y;
            if(pm.pos.x == -1)
                continue;
            pm.p = get_piece(g, pm.pos.x, pm.pos.y);
            fill_move_list(g, pm.pos.x, pm.pos.y, pm.ml);

            for(int j = 0; j < pm.ml->nb; j += 2)
            {
                int new_x = pm.ml->arr[j];
                int new_y = pm.ml->arr[j+1];
                Piece tmp_captured = get_piece(g, new_x, new_y);
                set_piece(g, new_x, new_y, pm.p);
                set_piece(g, pm.pos.x, pm.pos.y, EMPTY);

                // temporary change of the piece list
                g->white_pieces[i].x = new_x;
                g->white_pieces[i].y = new_y;

                // temporary removal of the captured piece in the pieces list
                int tmp_x = -1, tmp_y = -1, iter = -1;
                if (is_black(tmp_captured))
                {
                    iter = 0;
                    while(iter < g->black_pieces_count)
                    {
                        if((g->black_pieces[iter].x == new_x) && (g->black_pieces[iter].y == new_y))
                        {
                            tmp_x = g->black_pieces[iter].x;
                            tmp_y = g->black_pieces[iter].y;

                            g->black_pieces[iter].x = -1;
                            g->black_pieces[iter].y = -1;

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
                        set_piece(g, 5, 7, W_ROOK);
                        set_piece(g, 7, 7, EMPTY);
                        has_castled = W_KINGSIDE_CASTLE;
                        break;
                    case -2:
                        set_piece(g, 3, 7, W_ROOK);
                        set_piece(g, 0, 7, EMPTY);
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
                        set_piece(g, new_x, new_y + 1, EMPTY);
                        iter = 0;
                        while(iter < g->black_pieces_count)
                        {
                            if((g->black_pieces[iter].x == new_x) && (g->black_pieces[iter].y == (new_y + 1)))
                            {
                                tmp_x = g->black_pieces[iter].x;
                                tmp_y = g->black_pieces[iter].y;

                                g->black_pieces[iter].x = -1;
                                g->black_pieces[iter].y = -1;

                                break;
                            }
                            iter++;
                        }
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
                        set_piece(g, new_x, new_y, W_QUEEN);
                    }
                }
                else
                {
                    g->en_pass.x = -1;
                    g->en_pass.y = -1;
                }

                g->bool_is_black = 1;

                int candidate = minimax(g, depth - 1, ai_m);
                min_value = (candidate < min_value)? candidate: min_value;

                g->bool_is_black = 0;

                // return to original pos
                set_piece(g, new_x, new_y, tmp_captured);
                set_piece(g, pm.pos.x, pm.pos.y, pm.p);

                // former castles state
                g->castles = tmp_castles;
                switch (has_castled)
                {
                case W_KINGSIDE_CASTLE:
                    set_piece(g, 7, 7, W_ROOK);
                    set_piece(g, 5, 7, EMPTY);
                    break;
                case W_QUEENSIDE_CASTLE:
                    set_piece(g, 0, 7, W_ROOK);
                    set_piece(g, 3, 7, EMPTY);
                    break;
                default:
                    break;
                }

                // former en passant state
                g->en_pass.x = tmp_en_pass.x;
                g->en_pass.y = tmp_en_pass.y;

                if(has_captured_en_pass)
                    set_piece(g, new_x, new_y + 1, B_PAWN);

                // if piece was captured, restore piece list
                if(iter != -1)
                {
                    g->black_pieces[iter].x = tmp_x;
                    g->black_pieces[iter].y = tmp_y;
                }

                // former piece list
                g->white_pieces[i].x = pm.pos.x;
                g->white_pieces[i].y = pm.pos.y;

            }
            // empty move list
            pm.ml->nb = 0;
        }
        free_move_list(pm.ml);
        return min_value;
    }
}


Ai_move* ai_move(char* fen_string)
{
    Game g;
	parse_fen_string(&g, fen_string);

    Ai_move* res = malloc(sizeof(Ai_move));

    minimax(&g, ARBITRARY_DEPTH, res);

    return res;
}

void free_ai_move(Ai_move* ai_m)
{
    free(ai_m);
}