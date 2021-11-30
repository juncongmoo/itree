import unittest
import itree
import pickle


class TestPicklable(unittest.TestCase):
    def test_picklable_tree_v2(self):
        t1 = itree.Tree(tid="", extra={})
        d1 = pickle.dumps(t1)
        t2 = pickle.loads(d1)
        d2 = pickle.dumps(t2)
        self.assertEqual(d1, d2)
        self.assertEqual(str(t1), str(t2))
        # print(d1, "\n", d2)


if __name__ == '__main__':
    unittest.main()
