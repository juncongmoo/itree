import unittest
import itree
import os

def demo_tree():
    t = itree.Tree(tid="123", extra={"img": "1241241313.png"})
    t.start("root", 1, {"name": "itree"})
    t.start("math", 2, {"age": 10})
    t.start("music", 3, {"location": [1, 2, 3]})
    t.end("music", 4, {"price": 12.3})
    t.end("math", 16284000, {"memory": (1, 2, 3)})
    t.start("music", 122840057.8713503)
    t.end("music", 1228400500)
    t.start("music", 32840057.8713503)
    t.start("egg", 3284.8713503)
    t.start("icecream", 32843.8713503)
    t.start("pizza", 32843.8713503)
    t.end("pizza", 62845003)
    t.end("icecream", 62845003)
    t.end("egg", 6284500)
    t.end("music", 628400500)
    t.start("piggy", 3284.8713503)
    t.start("unicorn", 32843.8713503)
    t.start("monkey", 32843.8713503)
    t.end("monkey", 62845003)
    t.end("unicorn", 62845003)
    t.end("piggy", 6284500)

    t.end("root", 1628400570.8713503)
    return t

class TestTree(unittest.TestCase):
    def setUp(self):
        self.tree = demo_tree()
        self.img_path = None

    def test_empty_tree(self):
        tree = itree.Tree()
        tmp = str(tree)
        self.assertEqual(tmp[:2], "(🌳")
        self.assertEqual(len(tree.tid), len('eef7cc7d'))
        self.assertEqual(tree.extra, {})
        tree.consolidate()

    def test_to_img(self):
        self.img_path = self.tree.to_img()
        self.assertTrue(os.path.exists(self.img_path))
        
    def tearDown(self):
        if self.img_path:
            os.unlink(self.img_path)
        

if __name__ == '__main__':
    unittest.main()
