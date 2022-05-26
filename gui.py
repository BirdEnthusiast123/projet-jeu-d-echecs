import sys
import ctypes as ct
if sys.version_info[0] == 3:
	import tkinter as tk
else:
	import Tkinter as tk

SQUARE_SIZE = 80
TOTAL_SIZE = 8 * SQUARE_SIZE

WHITE_THEME = "#C0F7AD"
BLACK_THEME = "#42752F"
HIGHLIGHT_THEME = "#F5AC5D"

EMPTY = 1

W_KNIGHT = 2
W_QUEEN = 4
W_KING = 8
W_ROOK = 16
W_BISHOP = 32
W_PAWN = 64

B_QUEEN = 128
B_KING = 256
B_ROOK = 512
B_BISHOP = 1024
B_PAWN = 2048
B_KNIGHT = 4096

BASE_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

# Some function calls throw an error if Tk environment is not initialized
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
				for i in range(int(char)):
					res.append(EMPTY)
			elif char == 'p':
				res.append(B_PAWN)
			elif char == 'r':
				res.append(B_ROOK)
			elif char == 'n':
				res.append(B_KNIGHT)
			elif char == 'b':
				res.append(B_BISHOP)
			elif char == 'q':
				res.append(B_QUEEN)
			elif char == 'k':
				res.append(B_KING)
			elif char == 'P':
				res.append(W_PAWN)
			elif char == 'R':
				res.append(W_ROOK)
			elif char == 'N':
				res.append(W_KNIGHT)
			elif char == 'B':
				res.append(W_BISHOP)
			elif char == 'Q':
				res.append(W_QUEEN)
			elif char == 'K':
				res.append(W_KING)
		return res

	def print_grid(self):
		for i in self.grid:
			print(i)

	def update_fen(self):
		res = ""
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

		res = res[:-1]
		tmp = self.fen.split(' ', 1)
		res += ' ' + tmp[1]
		self.fen = res
		print(self.fen)
		#also update rest of fen (castles etc)

# Canvas, graphic interface
class Chess_board(tk.Canvas):
	def __init__(self, root, wdth, hght, game):
		super().__init__(root, width=wdth, height=hght)
		self.setup_grid()
		self.game = game
		self.mouse_input_x = 0
		self.mouse_input_y = 0
		self.ml = Move_list()

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
								  outline="#000000",
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
						   fill="white",
						   outline="white")

	def mouse_input_handler(self, eventorigin):
		# move if in move_list
		new_x = eventorigin.x // SQUARE_SIZE
		new_y = eventorigin.y // SQUARE_SIZE

		if(self.ml.nb != 0):
			for i in range(0, self.ml.nb, 2):
				if(new_x == self.ml.array[i] and new_y == self.ml.array[i+1]):
					print("here1")
					self.game.grid[new_y][new_x] = self.game.grid[self.mouse_input_y][self.mouse_input_x]
					self.game.grid[self.mouse_input_y][self.mouse_input_x] = EMPTY
					self.game.update_fen()
					self.setup_grid()
					self.draw_pieces()
					lib.free_move_list(ct.byref(self.ml))
					self.ml.nb = 0
					return
					#update fen too


		# clean previous highlight
		self.draw_square(self.mouse_input_x, self.mouse_input_y)
		self.draw_piece(self.mouse_input_x, self.mouse_input_y)
		if(self.ml.nb != 0):
			for i in range(0, self.ml.nb, 2):
				self.draw_square(self.ml.array[i], self.ml.array[i+1])
				self.draw_piece(self.ml.array[i], self.ml.array[i+1])
			lib.free_move_list(ct.byref(self.ml))
			self.ml.nb = 0

		# highlight current and possible squares
		self.mouse_input_x = new_x
		self.mouse_input_y = new_y
		self.highlight_square(self.mouse_input_x, self.mouse_input_y)
		self.draw_piece(self.mouse_input_x, self.mouse_input_y)
		
		self.ml = Move_list.from_address(lib.possible_moves(self.game.fen.encode(), self.mouse_input_x, self.mouse_input_y))
		for i in range(0, self.ml.nb, 2):
			self.highlight_possible_move(self.ml.array[i], self.ml.array[i+1])
		


def main():
	game = Game(BASE_FEN)

	ex = Chess_board(root, str(TOTAL_SIZE), str(TOTAL_SIZE), game)
	ex.pack()

	root.bind("<Button 1>", ex.mouse_input_handler)

	ex.draw_pieces()
	root.mainloop()


if __name__ == '__main__':
	main()
