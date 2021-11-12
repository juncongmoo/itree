__version__ = "1.0.1"

from . import _itree

import time
from abc import ABC
from subprocess import check_call
from tempfile import NamedTemporaryFile
import random


class ITree(_itree.Tree):
    """An interval tree"""

    def __init__(self, s="", tree_id=None, extra={}):
        """Constructor of Tree

        Args:
            s (str, optional): a string representation which a tree could be recovered from. Defaults to "".
            tree_id (str, optional): a unique id of the tree. Defaults to None.
            extra (dict, optional): extra information of the tree. Defaults to {}.
            update_min_max_required (bool, optional): used for some tree where min/max values appear in the leaf node. Defaults to False.
        """
        if tree_id is None:
            tree_id = "{:08x}".format(random.getrandbits(32))

        super(ITree, self).__init__(s=s, tree_id=tree_id, extra=extra)

    def add(self, node: _itree.Node):
        pass

    def start(self, a, b, extra={}):
        self.discover(a, b, extra)

    def end(self, a, b, extra={}):
        self.finish(a, b, extra)

    def consolidate(self):
        if len(self.stk) > 1:
            return
        self.root = _itree.consolidate(self.root)

    def to_img(sf, filename=None):
        if not filename:
            filename = f"/tmp/{time.time()}.png"
        s = sf.to_dot_string("circle")
        with NamedTemporaryFile("wb", delete=True) as dotfile:
            dotfilename = dotfile.name
            dotfile.write(s.encode("utf-8"))
            dotfile.flush()
            cmd = ["dot", dotfilename, "-T", "png", "-o", filename]
            check_call(cmd)
        return filename


class ForestBase(_itree.Forest):
    __metaclass__ = ABC


if __name__ == "__main__":

    def hand_crafted_tree():
        start = time.monotonic()
        t = ITree(tree_id="123", extra={"img_path": "1241241313.png"})
        t.start("root", 1, {"name": "fuheng"})
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
        end = time.monotonic()
        print(f"{t.count},{t.depth}")
        t.consolidate()
        # print(get_graph(t))
        # print(t.to_dot_string("triangle"))
        img_name = t.to_img()
        return img_name, (end - start) * 1e6

    print(hand_crafted_tree())
