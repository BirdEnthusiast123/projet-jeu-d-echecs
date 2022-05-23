import ctypes as ct

#ct.cdll.LoadLibrary("./shared.so")  

#shared_file = "./shared.so"
#shared = ct.CDLL(shared_file)

#shared.print_test(b"10358")
#testvar = "test"
#shared.print_test(testvar.encode())



#lib_path = "./shared.so"
#lib = ct.CDLL(lib_path)

#class Position(ct.Structure):
#	_fields_ = [('x', ct.c_int), ('y', ct.c_int)]
	
#lib.get_position.restype = ct.c_void_p

#p1 = Position.from_address(lib.get_position())
#print(p1.x, p1.y)

#lib.free_position(ct.byref(p1))
#del p1




lib_path = "./shared.so"
lib = ct.CDLL(lib_path)

lib.get_position.restype = ct.c_void_p
lib.get_move.restype = ct.c_void_p
lib.get_move_arr.restype = ct.c_void_p

class Position(ct.Structure):
	_fields_ = [('x', ct.c_int), ('y', ct.c_int)]

class Move(ct.Structure):
	_fields_ = [('from_', Position),
				('to', Position),
				('piece', ct.c_int)]


m1 = Move.from_address(lib.get_move())
m1.from_ = Position.from_address(lib.get_position())
m1.to = Position.from_address(lib.get_position())
print(m1.from_.x, m1.from_.y, m1.to.x, m1.to.y, m1.piece)


class Move_array(ct.Structure):
    _fields_ = [('len', ct.c_int),
                ('STRUCT_ARRAY', ct.POINTER(Move))]

    def __init__(self,num_of_structs):
        elems = (STRUCT_2 * num_of_structs)()
        self.STRUCT_ARRAY = ctypes.cast(elems,ct.POINTER(STRUCT_2))
        self.elements = num_of_structs

        for num in range(0,num_of_structs):
            self.STRUCT_ARRAY[num].field_1 = 1
            self.STRUCT_ARRAY[num].field_2 = 2
            self.STRUCT_ARRAY[num].field_3 = 3
            self.STRUCT_ARRAY[num].field_4 = 4


move_array = Move_array.from_address(lib.get_move_arr())
print(move_array.STRUCT_ARRAY[0].from_.x, move_array.STRUCT_ARRAY[0].from_.y, move_array.STRUCT_ARRAY[0].to.x, move_array.STRUCT_ARRAY[0].to.y, move_array.STRUCT_ARRAY[0].piece)
print(move_array.STRUCT_ARRAY[1].from_.x, move_array.STRUCT_ARRAY[1].from_.y, move_array.STRUCT_ARRAY[1].to.x, move_array.STRUCT_ARRAY[1].to.y, move_array.STRUCT_ARRAY[1].piece)





