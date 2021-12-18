import unittest
import itree


def demo_forest():
    f = itree.ForestStats()
    f.init_time_us = 1638259166350616
    return f


class TestForest(unittest.TestCase):
    def setUp(self):
        self.fr = demo_forest()

    def test_serialization(self):
        d1 = itree._itree.serialize_forest_(self.fr)
        expected = 'f1^1638259166350616🆃0🆃0🆃0🆃0🆃0🆃0🆃0🆃0🆃0🆃🆃0'
        self.assertEqual(d1, expected)


if __name__ == '__main__':
    unittest.main()
