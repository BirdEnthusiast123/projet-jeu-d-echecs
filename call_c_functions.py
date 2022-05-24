import ctypes as ct

lib_path = "./shared.so"
lib = ct.CDLL(lib_path)

lib.wrap_possible_moves.restype = ct.c_void_p


class TEST(ct.Structure):
	_fields_ = [('len', ct.c_int),
                ('array', ct.POINTER(ct.c_int))]

t1 = TEST.from_address(lib.wrap_possible_moves(4, 2))

print(t1.array[0], t1.array[1])

lib.free_test(ct.byref(t1))











