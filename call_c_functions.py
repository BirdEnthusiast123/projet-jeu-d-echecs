import ctypes as ct

ct.cdll.LoadLibrary("./test_shared.so")  

shared_file = "./test_shared.so"
shared = ct.CDLL(shared_file)

# lib = ct.cdll.LoadLibrary("./test_shared.so")
print(hex(ct.windll.kernel32.GetModuleHandleA(None)))  

#print(lib.square(10))

