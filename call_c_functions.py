import ctypes as ct

ct.cdll.LoadLibrary("./shared.so")  

shared_file = "./shared.so"
shared = ct.CDLL(shared_file)

shared.print_test(b"10358")
testvar = "test"
shared.print_test(testvar.encode())

