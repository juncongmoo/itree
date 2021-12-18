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

    def test_serialization(self):
        tree = demo_tree()
        d1 = itree._itree.serialize_tree_(tree)
        expected = "t1^123,,0,11,6,1,1e-07,25#{'img': '1241241313.png'}%n1*[None,inf,inf,0$0#[root,1.0,1628400570.8713503,0$17#{'name': 'itree'}[math,2.0,16284000.0,0$32#{'age': 10, 'memory': (1, 2, 3)}[music,3.0,4.0,0$38#{'location': [1, 2, 3], 'price': 12.3}]][music,122840057.8713503,1228400500.0,0$0#][music,32840057.8713503,628400500.0,0$0#[egg,3284.8713503,6284500.0,0$0#[icecream,32843.8713503,62845003.0,0$0#[pizza,32843.8713503,62845003.0,0$0#]]]][piggy,3284.8713503,6284500.0,0$0#[unicorn,32843.8713503,62845003.0,0$0#[monkey,32843.8713503,62845003.0,0$0#]]]]]"
        self.assertEqual(d1, expected)

    def test_deserialization(self):
        x = demo_tree()

        tree = itree.create_tmp_tree()
        d1 = "t1^123,,0,11,6,1,1e-07,25#{'img': '1241241313.png'}%n1*[None,inf,inf,0$0#[root,1.0,1628400570.8713503,0$17#{'name': 'itree'}[math,2.0,16284000.0,0$32#{'age': 10, 'memory': (1, 2, 3)}[music,3.0,4.0,0$38#{'location': [1, 2, 3], 'price': 12.3}]][music,122840057.8713503,1228400500.0,0$0#][music,32840057.8713503,628400500.0,0$0#[egg,3284.8713503,6284500.0,0$0#[icecream,32843.8713503,62845003.0,0$0#[pizza,32843.8713503,62845003.0,0$0#]]]][piggy,3284.8713503,6284500.0,0$0#[unicorn,32843.8713503,62845003.0,0$0#[monkey,32843.8713503,62845003.0,0$0#]]]]]"
        tree.deserialize(d1)
        self.assertEqual(x, tree)

    def tearDown(self):
        if self.img_path:
            os.unlink(self.img_path)


if __name__ == '__main__':
    unittest.main()
