__version__ = "0.0.4"

try:
    from . import _itree
except ImportError:
    import _itree

import time
from abc import ABC
from subprocess import check_call
from tempfile import NamedTemporaryFile
import random


class Node(_itree.Node):
    """Tree Node"""


class ITree(_itree.Tree):
    """An interval tree"""

    def __init__(self, tree_id=None, extra={}):
        """Constructor of Tree

        Args:
            tree_id (str, optional): a unique id of the tree. Defaults to None.
            extra (dict, optional): extra information of the tree. Defaults to {}.
            update_min_max_required (bool, optional): used for some tree where min/max values appear in the leaf node. Defaults to False.
        """
        if tree_id is None:
            tree_id = "{:08x}".format(random.getrandbits(32))

        super(ITree, self).__init__(tree_id=tree_id, extra=extra)

    def start(self, a, b, extra={}):
        self.discover(a, b, extra)

    def end(self, a, b, extra={}):
        self.finish(a, b, extra)

    def consolidate(self):
        if len(self.stk) > 1:
            return
        self.root = _itree.consolidate(self.root)

    def to_img(sf, filename=None, format="png", node_shape="record"):
        try:
            if not filename:
                filename = f"/tmp/{time.time()}.{format}"
            s = sf.to_dot_string(node_shape)
            with NamedTemporaryFile("wb", delete=True) as dotfile:
                dotfilename = dotfile.name
                dotfile.write(s.encode("utf-8"))
                dotfile.flush()
                cmd = ["dot", dotfilename, "-T", format, "-o", filename]
                check_call(cmd)
        except FileNotFoundError as e:
            print("dot is missing! please install graphviz.")
            return None
        return filename


class ForestBase(_itree.Forest):
    """Forest is a collection of trees"""

    __metaclass__ = ABC
