import unittest
import itree
import pickle


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


def demo_forest():
    f = itree.ForestStats()
    return f


class TestPicklable(unittest.TestCase):
    def test_picklable_node(self):
        fruit = itree.Node('fruit')

        self.assertTrue(itree.is_picklable(fruit))

        apple = itree.Node('apple', 0, 1, {"x": 1})
        self.assertTrue(itree.is_picklable(apple))

        fruit.append(apple)
        self.assertTrue(itree.is_picklable(fruit))
        fruit.end = 100
        self.assertTrue(itree.is_picklable(fruit))

        pear = itree.Node('pear', 4, 5, {"x": 4})
        fruit.append(pear)
        self.assertTrue(itree.is_picklable(pear))
        self.assertTrue(itree.is_picklable(fruit))

        d1 = pickle.dumps(fruit)
        f2 = pickle.loads(d1)
        d2 = pickle.dumps(f2)
        self.assertEqual(str(fruit), str(f2))
        self.assertEqual(d1, d2)
        # print("*"*40)
        # print(d1, "\n", d2)
        # print("*"*40)

    def test_picklable_tree(self):
        t1 = demo_tree()
        d1 = pickle.dumps(t1)
        t2 = pickle.loads(d1)
        d2 = pickle.dumps(t2)
        self.assertEqual(d1, d2)
        self.assertEqual(str(t1), str(t2))
        # print(d1, "\n", d2)

    def test_picklable_tree_v2(self):
        t1 = itree.Tree(tid="", extra={})
        d1 = pickle.dumps(t1)
        t2 = pickle.loads(d1)
        d2 = pickle.dumps(t2)
        self.assertEqual(d1, d2)
        self.assertEqual(str(t1), str(t2))
        # print(d1, "\n", d2)

    def test_forest(self):
        f1 = demo_forest()
        d1 = pickle.dumps(f1)
        f2 = pickle.loads(d1)
        d2 = pickle.dumps(f2)
        self.assertEqual(d1, d2)
        self.assertEqual(f1.init_time_us, f2.init_time_us)
        # print(d1, "\n", d2)


if __name__ == '__main__':
    unittest.main()
