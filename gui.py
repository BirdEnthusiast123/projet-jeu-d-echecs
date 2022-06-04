import sys
import ctypes as ct
if sys.version_info[0] == 3:
    import tkinter as tk
else:
    import Tkinter as tk
import copy

SQUARE_SIZE = 80
TOTAL_SIZE = 8 * SQUARE_SIZE

WHITE_THEME = "#C0F7AD"
BLACK_THEME = "#42752F"
HIGHLIGHT_THEME = "#F5AC5D"

EMPTY = 16

W_KNIGHT = 1
W_QUEEN = 2
W_KING = 3
W_ROOK = 4
W_BISHOP = 5
W_PAWN = 6

B_QUEEN = 8
B_KING = 9
B_ROOK = 10
B_BISHOP = 11
B_PAWN = 12
B_KNIGHT = 13

BASE_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
TEST_FEN = "rnbqkbnr/ppPp3p/8/4ppp1/8/8/PPP1PPPP/RNBQKBNR b KQkq - 7 8"

# Some function calls throw an error if the Tkinter environment is not initialized
# such as the following images being loaded
root = tk.Tk()

W_KNIGHT_SPRITE = tk.PhotoImage(file='pieces/knight_w.png').subsample(7)
W_QUEEN_SPRITE = tk.PhotoImage(file='pieces/queen_w.png').subsample(7)
W_KING_SPRITE = tk.PhotoImage(file='pieces/king_w.png').subsample(7)
W_ROOK_SPRITE = tk.PhotoImage(file='pieces/rook_w.png').subsample(7)
W_BISHOP_SPRITE = tk.PhotoImage(file='pieces/bishop_w.png').subsample(7)
W_PAWN_SPRITE = tk.PhotoImage(file='pieces/pawn_w.png').subsample(7)

B_QUEEN_SPRITE = tk.PhotoImage(file='pieces/queen_b.png').subsample(7)
B_KING_SPRITE = tk.PhotoImage(file='pieces/king_b.png').subsample(7)
B_ROOK_SPRITE = tk.PhotoImage(file='pieces/rook_b.png').subsample(7)
B_BISHOP_SPRITE = tk.PhotoImage(file='pieces/bishop_b.png').subsample(7)
B_PAWN_SPRITE = tk.PhotoImage(file='pieces/pawn_b.png').subsample(7)
B_KNIGHT_SPRITE = tk.PhotoImage(file='pieces/knight_b.png').subsample(7)

dict_piece_img = {
    W_KNIGHT: W_KNIGHT_SPRITE,
    W_QUEEN: W_QUEEN_SPRITE,
    W_KING: W_KING_SPRITE,
    W_ROOK: W_ROOK_SPRITE,
    W_BISHOP: W_BISHOP_SPRITE,
    W_PAWN: W_PAWN_SPRITE,

    B_QUEEN: B_QUEEN_SPRITE,
    B_KING: B_KING_SPRITE,
    B_ROOK: B_ROOK_SPRITE,
    B_BISHOP: B_BISHOP_SPRITE,
    B_PAWN: B_PAWN_SPRITE,
    B_KNIGHT: B_KNIGHT_SPRITE,
}

dict_piece_fen = {
    W_KNIGHT: 'N',
    W_QUEEN: 'Q',
    W_KING: 'K',
    W_ROOK: 'R',
    W_BISHOP: 'B',
    W_PAWN: 'P',

    B_QUEEN: 'q',
    B_KING: 'k',
    B_ROOK: 'r',
    B_BISHOP: 'b',
    B_PAWN: 'p',
    B_KNIGHT: 'n',
}

# inverse dictonnary char => int
inv_dict_piece_fen = {val: key for key, val in dict_piece_fen.items()}

# move list
# calling c function
lib_path = "./shared.so"
lib = ct.CDLL(lib_path)

lib.possible_moves.restype = ct.c_void_p


class Move_list(ct.Structure):
    _fields_ = [('len', ct.c_int),
                ('nb', ct.c_int),
                ('array', ct.POINTER(ct.c_int))]

    def __init__(self):
        self.nb = 0

# chess game representation


class Game:

    def __init__(self, fen_string):
        self.parse_fen_string(fen_string)
        self.fen = fen_string
        self.castles = "KQkq"
        self.en_passant = "-"

    def parse_fen_string(self, fen_string):
        board_info = fen_string.split(' ')
        lines = board_info[0].split('/')

        self.grid = []
        for line in lines:
            self.grid.append(self.parse_fen_line(line))

    def parse_fen_line(self, line):
        res = []
        for char in line:
            if char.isdigit():
                res += [EMPTY] * int(char)
            else:
                res += [inv_dict_piece_fen[char]]
        return res

    def print_grid(self):
        for i in self.grid:
            print(i)

    def is_black(self, x, y):
        return (self.grid[y][x] > 7) and (self.grid[y][x] < 16)

    def is_white(self, x, y):
        return self.grid[y][x] < 7

    def is_empty(self, x, y):
        return self.grid[y][x] == 16

    def update_fen(self):
        res = ""
        tmp = self.fen.split(' ')

        # pieces
        count = 0
        for line in self.grid:
            for cell in line:
                if(cell == EMPTY):
                    count += 1
                else:
                    if(count != 0):
                        res += str(count)
                        count = 0
                    res += dict_piece_fen[cell]
            if(count != 0):
                res += str(count)
                count = 0
            res += '/'
        res = res[:-1] + ' '

        # player
        res += 'b ' if (tmp[1][0] == 'w') else 'w '

        # castles
        res += (self.castles + ' ')

        # en passant
        res += self.en_passant + ' '

        # half moves
        res += str(int(tmp[4]) + 1) + ' '

        # full moves
        res += str(int(tmp[5]) + 1) + ' '
        self.fen = copy.copy(res)
        print(self.fen)

# Canvas, graphic interface

class Promotion_window(tk.Toplevel):
    def __init__(self, rt, bool_is_black=True, wdth=str(4 * SQUARE_SIZE), hght=str(SQUARE_SIZE)):
        super().__init__(rt, width=wdth, height=hght)
        self.piece = EMPTY

        self.bind("<Button 1>", self.mouse_input_handler_promotion, add='+')

        promo_canvas = tk.Canvas(self, width=wdth, height=hght)
        promo_canvas.pack()

        self.promo = [B_QUEEN, B_KNIGHT, B_BISHOP, B_ROOK] if bool_is_black\
                    else [W_QUEEN, W_KNIGHT, W_BISHOP, W_ROOK]
        promo_canvas.create_image(40, 45, image=dict_piece_img[self.promo[0]])
        promo_canvas.create_image(40 + 1 * SQUARE_SIZE, 45, image=dict_piece_img[self.promo[1]])
        promo_canvas.create_image(40 + 2 * SQUARE_SIZE, 45, image=dict_piece_img[self.promo[2]])
        promo_canvas.create_image(40 + 3 * SQUARE_SIZE, 45, image=dict_piece_img[self.promo[3]])

    def mouse_input_handler_promotion(self, eventorigin):
        x = eventorigin.x // SQUARE_SIZE
        self.piece = self.promo[x]
        self.destroy()


class Chess_board(tk.Canvas):
    def __init__(self, rt, wdth, hght, game):
        super().__init__(rt, width=wdth, height=hght)
        self.setup_grid()
        self.game = game
        self.mouse_input_x = 0
        self.mouse_input_y = 0
        self.ml = Move_list()
        self.promotion = EMPTY

    def setup_grid(self):
        self.create_rectangle(0, 0,
                              TOTAL_SIZE, TOTAL_SIZE,
                              outline="#000000",
                              fill=BLACK_THEME)
        for i in range(0, 8):
            for j in range(0, 8):
                if(((i + j) % 2) == 0):
                    self.create_rectangle(i*SQUARE_SIZE, j*SQUARE_SIZE,
                                          i*SQUARE_SIZE + SQUARE_SIZE, j*SQUARE_SIZE + SQUARE_SIZE,
                                          outline="#000000",
                                          fill=WHITE_THEME)

    def draw_piece(self, x, y):
        if (self.game.grid[y][x] != EMPTY):
            self.create_image(40 + 80 * x, 45 + 80 * y,
                              image=dict_piece_img[self.game.grid[y][x]])

    def draw_pieces(self):
        for i, line in enumerate(self.game.grid):
            for j, piece in enumerate(line):
                if piece != EMPTY:
                    self.create_image(40 + 80 * j, 45 + 80 * i,
                                      image=dict_piece_img[piece])

    def highlight_square(self, x, y):
        if(self.game.grid[y][x] != EMPTY):
            self.create_rectangle(x*SQUARE_SIZE, y*SQUARE_SIZE,
                                  x*SQUARE_SIZE + SQUARE_SIZE, y*SQUARE_SIZE + SQUARE_SIZE,
                                  outline="#FFFFFF",
                                  fill=HIGHLIGHT_THEME)

    def draw_square(self, x, y):
        if((x + y) % 2) == 0:
            self.create_rectangle(x*SQUARE_SIZE, y*SQUARE_SIZE,
                                  x*SQUARE_SIZE + SQUARE_SIZE, y*SQUARE_SIZE + SQUARE_SIZE,
                                  outline="#000000",
                                  fill=WHITE_THEME)
        else:
            self.create_rectangle(x*SQUARE_SIZE, y*SQUARE_SIZE,
                                  x*SQUARE_SIZE + SQUARE_SIZE, y*SQUARE_SIZE + SQUARE_SIZE,
                                  outline="#000000",
                                  fill=BLACK_THEME)

    def create_circle(self, x, y, r, **kwargs):
        self.create_oval(x-r, y-r, x+r, y+r, **kwargs)

    def highlight_possible_move(self, x, y):
        self.create_circle(x*SQUARE_SIZE + (SQUARE_SIZE/2),
                           y*SQUARE_SIZE + (SQUARE_SIZE/2),
                           SQUARE_SIZE/8,
                           fill=HIGHLIGHT_THEME,
                           outline="black")

    def mouse_input_handler(self, eventorigin):
        new_x = eventorigin.x // SQUARE_SIZE
        new_y = eventorigin.y // SQUARE_SIZE

        # move if clicked square is in move_list
        if(self.ml.nb != 0):
            for i in range(0, self.ml.nb, 2):
                if(new_x == self.ml.array[i] and new_y == self.ml.array[i+1]):
                    # test for castles
                    # blackside castles
                    piece = self.game.grid[self.mouse_input_y][self.mouse_input_x]
                    if(piece == B_KING):
                        if((self.mouse_input_x - new_x) == 2):
                            self.game.grid[0][3] = self.game.grid[0][0]
                            self.game.grid[0][0] = EMPTY
                        if((self.mouse_input_x - new_x) == -2):
                            self.game.grid[0][5] = self.game.grid[0][7]
                            self.game.grid[0][7] = EMPTY
                        self.game.castles = self.game.castles.replace('k', '')
                        self.game.castles = self.game.castles.replace('q', '')
                        self.game.castles = "-" if (self.game.castles == "") \
                                                else self.game.castles

                    # whiteside castles
                    if(piece == W_KING):
                        if((self.mouse_input_x - new_x) == 2):
                            self.game.grid[7][3] = self.game.grid[7][0]
                            self.game.grid[7][0] = EMPTY
                        if((self.mouse_input_x - new_x) == -2):
                            self.game.grid[7][5] = self.game.grid[7][7]
                            self.game.grid[7][7] = EMPTY
                        self.game.castles = self.game.castles.replace('K', '')
                        self.game.castles = self.game.castles.replace('Q', '')
                        self.game.castles = "-" if (self.game.castles == "") \
                                                else self.game.castles

                    # black rook moves => no castles on that side
                    if(piece == B_ROOK):
                        if(self.mouse_input_x == 0):
                            self.game.castles = self.game.castles.replace(
                                'q', '')
                            self.game.castles = "-" if (self.game.castles == "") \
                                                    else self.game.castles
                        if(self.mouse_input_x == 7):
                            self.game.castles = self.game.castles.replace('k', '')
                            self.game.castles = "-" if (self.game.castles == "") \
                                                    else self.game.castles

                    # white rook moves => no castles on that side
                    if(piece == W_ROOK):
                        if(self.mouse_input_x == 0):
                            self.game.castles = self.game.castles.replace(
                                'Q', '')
                            self.game.castles = "-" if (self.game.castles == "") \
                                                    else self.game.castles
                        if(self.mouse_input_x == 7):
                            self.game.castles = self.game.castles.replace('K', '')
                            self.game.castles = "-" if (self.game.castles == "") \
                                                    else self.game.castles

                    # en passant fen string update and capture board update
                    if(piece == B_PAWN):
                        # update en passant coordinates in fen string
                        if((new_y - self.mouse_input_y) == 2):
                            self.game.en_passant = chr(ord('a') + new_x) + str(8 - (new_y - 1))
                        # en passant : remove captured piece
                        if(self.game.fen.split(' ')[3][0] != '-'):
                            en_pass_str = self.game.fen.split(' ')[3]
                            en_pass_x = ord(en_pass_str[0]) - ord('a')
                            en_pass_y = 8 - int(en_pass_str[1])
                            if((new_x == en_pass_x) and (new_y == en_pass_y)):
                                self.game.grid[new_y - 1][new_x] = EMPTY
                        # promotion
                        if(new_y == 7):
                            promotion = Promotion_window(root, bool_is_black=True)
                            root.wait_window(promotion)
                            # dependency below, thankfully promotion is the only move
                            # changing the quality of a piece
                            piece = promotion.piece

                    if(piece == W_PAWN):
                        # update en passant coordinates in fen string
                        if((self.mouse_input_y - new_y) == 2):
                            self.game.en_passant = chr(ord('a') + new_x) + str(8 - (new_y + 1))
                        # en passant : remove captured piece
                        if(self.game.fen.split(' ')[3][0] != '-'):
                            en_pass_str = self.game.fen.split(' ')[3]
                            en_pass_x = ord(en_pass_str[0]) - ord('a')
                            en_pass_y = 8 - int(en_pass_str[1])
                            if((new_x == en_pass_x) and (new_y == en_pass_y)):
                                self.game.grid[new_y + 1][new_x] = EMPTY
                        # promotion
                        if(new_y == 0):
                            promotion = Promotion_window(root, bool_is_black=False)
                            root.wait_window(promotion)
                            # dependency below, thankfully promotion is the only move
                            # changing the quality of a piece
                            piece = promotion.piece

                    self.game.grid[new_y][new_x] = piece
                    self.game.grid[self.mouse_input_y][self.mouse_input_x] = EMPTY

                    self.game.update_fen()
                    # reset en_passant
                    self.game.en_passant = '-'

                    self.delete("all")

                    self.setup_grid()
                    self.draw_pieces()

                    lib.free_move_list(ct.byref(self.ml))
                    self.ml.nb = 0
                    return

        # clean previous highlight
        self.draw_square(self.mouse_input_x, self.mouse_input_y)
        self.draw_piece(self.mouse_input_x, self.mouse_input_y)
        if(self.ml.nb != 0):
            for i in range(0, self.ml.nb, 2):
            	if(self.ml.array[i] >= 0 and self.ml.array[i] <= 7 and \
            		self.ml.array[i+1] >= 0 and self.ml.array[i+1] <= 7):
	                self.draw_square(self.ml.array[i], self.ml.array[i+1])
	                self.draw_piece(self.ml.array[i], self.ml.array[i+1])
            self.ml.nb = 0

        # highlight current and possible squares
        self.mouse_input_x = new_x if ((new_x >= 0) and (new_x <= 7)) else 0
        self.mouse_input_y = new_y if ((new_y >= 0) and (new_y <= 7)) else 0
        self.highlight_square(self.mouse_input_x, self.mouse_input_y)
        self.draw_piece(self.mouse_input_x, self.mouse_input_y)

        player = self.game.fen.split(' ', 1)[1][0]
        if((player == 'b' and self.game.is_black(self.mouse_input_x, self.mouse_input_y)) or
                (player == 'w' and self.game.is_white(self.mouse_input_x, self.mouse_input_y))):
            self.ml = Move_list.from_address(lib.possible_moves(
                self.game.fen.encode(), self.mouse_input_x, self.mouse_input_y))
            for i in range(0, self.ml.nb, 2):
                self.highlight_possible_move(self.ml.array[i], self.ml.array[i+1])


def main():
    game = Game(TEST_FEN)

    ex = Chess_board(root, str(TOTAL_SIZE), str(TOTAL_SIZE), game)
    ex.pack()

    root.bind("<Button 1>", ex.mouse_input_handler, add='+')

    ex.draw_pieces()
    root.mainloop()


if __name__ == '__main__':
    main()
