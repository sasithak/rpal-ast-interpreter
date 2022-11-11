import subprocess
import unittest


def runTest(filename):
    p = subprocess.Popen(['./myrpal', 'tests/' + filename],
                         stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    ret_code = p.returncode
    return stdout.decode('utf-8').strip(), stderr.decode('utf-8').strip(), ret_code


class TestMyrpal(unittest.TestCase):
    def test_1(self):
        stdout, stderr, ret_code = runTest('test_01')
        self.assertEqual(stdout, '(21, 13, 8, 5, 3, 2, 1, 1)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_2(self):
        stdout, stderr, ret_code = runTest('test_02')
        self.assertEqual(stdout, '(0, 1, 1, 2, 3)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_3(self):
        stdout, stderr, ret_code = runTest('test_03')
        self.assertEqual(
            stdout, '(11, 22, 33, 44, 55, 66, 77, 88, 99, 101, 111, 121, 131, 141, 151, 161, 171, 181, 191)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_4(self):
        stdout, stderr, ret_code = runTest('test_04')
        self.assertEqual(stdout, '(Odd, Even)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_5(self):
        stdout, stderr, ret_code = runTest('test_05')
        self.assertEqual(stdout, '((ad, be, cf), (xp, yq, zr))')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_6(self):
        stdout, stderr, ret_code = runTest('test_06')
        self.assertEqual(stdout, '(true, false, true)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_7(self):
        stdout, stderr, ret_code = runTest('test_07')
        self.assertEqual(stdout, '(false, true)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_8(self):
        stdout, stderr, ret_code = runTest('test_08')
        self.assertEqual(stdout, '(true, true, false)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_9(self):
        stdout, stderr, ret_code = runTest('test_09')
        self.assertEqual(stdout, '(Negative, Zero, Positive)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)

    def test_10(self):
        stdout, stderr, ret_code = runTest('test_10')
        self.assertEqual(stdout, '(5, 7, 9)')
        self.assertEqual(stderr, '')
        self.assertEqual(ret_code, 0)


if __name__ == '__main__':
    unittest.main()
