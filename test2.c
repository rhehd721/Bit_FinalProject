import ctypes


libc = ctypes.CDLL("./test.so")
counter = libc.main(1)
