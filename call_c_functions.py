import ctypes as ct

lib_path = "./shared.so"
lib = ct.CDLL(lib_path)

lib.possible_moves.restype = ct.c_void_p


class TEST(ct.Structure):
	_fields_ = [('len', ct.c_int),
		('nb', ct.c_int),
                ('array', ct.POINTER(ct.c_int))]


fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR 0 KQkq - 0 0"
t1 = TEST.from_address(lib.possible_moves(fen_string.encode(), 0, 1))

for i in range(0, t1.nb):
	print(t1.array[i])

lib.free_test(ct.byref(t1))











