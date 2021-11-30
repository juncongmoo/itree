import unittest
import itree


class TestUtils(unittest.TestCase):
    def test_utils_node(self):
        n = itree.create_virtual_node()
        self.assertTrue(itree.is_virtual_node(n))

    def test_exe(self):
        x = itree.exe("n=itree.create_virtual_node()")['n']
        self.assertTrue(itree.is_virtual_node(x))

    def test_mod(self):
        x = itree.mod('os')
        self.assertTrue(x.path)

    def test_uuid(self):
        x = set()
        L = 1024000
        for i in range(L):
            x.add(itree.uuid())
        self.assertEqual(len(x), L)

    def test_time(self):
        self.assertTrue(itree.time_s())
        self.assertTrue(itree.time_ms())
        self.assertTrue(itree.time_us())


if __name__ == '__main__':
    unittest.main()
