from .version import __version__

try:
    from . import _itree
except ImportError:
    import _itree

import time
from subprocess import check_call
from tempfile import NamedTemporaryFile
import random
from .node import _consolidate


class Tree(_itree.Tree):
    """An interval tree"""

    def __init__(
        self, tid=None, extra=None, monotonic=True, capacity=1024, zin_threshold=1e-7
    ):
        """Constructor of Tree

        Args:
            tid (str, optional): a unique id of the tree. Defaults to None.
            extra (dict, optional): extra information of the tree. Defaults to {}.
        """
        if tid is None:
            tid = "{:08x}".format(random.getrandbits(32))

        super().__init__(
            tid=tid,
            extra=extra if extra is not None else {},
            monotonic=monotonic,
            capacity=capacity,
            zin_threshold=zin_threshold,
        )

    def start(self, a, b, extra=None):
        """discover a node with name `a` and value `b`, and extra"""
        self.discover(a, b, extra if extra is not None else {})

    def end(self, a, b, extra=None):
        """finish a node with name `a` and value `b`, and extra"""
        self.finish(a, b, extra if extra is not None else {})

    def consolidate(self):
        """consolidate virtual nodes if any"""
        if len(self.stk) > 1:
            return
        self.root = _consolidate(self.root)

    def to_img(sf, filename=None, format="png", node_shape="record"):
        """Render tree to an image with `dot`"""
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
