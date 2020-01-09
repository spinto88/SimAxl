import unittest
import ctypes as C
import os

libc = C.CDLL(os.getcwd() + '/libc.so')

class TestOverlap(unittest.TestCase):

    def test_overlap0(self):

        libc.overlap.argtypes = [C.c_int, C.c_double, C.c_double, C.c_double]
	libc.overlap.restype = C.c_int
        
	result0 = libc.overlap(3,1,1,1)
        self.assertEqual(result0, 0)

    def test_overlap1(self):

        libc.overlap.argtypes = [C.c_int, C.c_double, C.c_double, C.c_double]
	libc.overlap.restype = C.c_int
        
	result1 = libc.overlap(3,2,1,1)
        self.assertEqual(result1, 1)

    def test_down_rate(self):

        libc.down_rate.argtypes = [C.c_int, C.c_double, C.c_double, C.c_double]
	libc.down_rate.restype = C.c_double
        
	result = libc.down_rate(3,2,1,1)
        self.assertEqual(result, 0)

    def test_up_rate(self):

        libc.up_rate.argtypes = [C.c_int, C.c_double, C.c_double, C.c_double]
	libc.up_rate.restype = C.c_double
        
	result = libc.up_rate(3,2,1,1)
        self.assertEqual(result, 1./2)

if __name__ == '__main__':
    unittest.main()

