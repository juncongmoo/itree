from itree.version import __version__
from itree import _itree
import itree
import ast
from typing import Union


def serialize_node(n):
    s = "n1*"

    def __f(n, s: str = ""):
        """serialize_node a Node to string"""
        if n:
            extra_ = "0#"
            if n.extra:
                extra_ = str(n.extra)
                extra_ = f"{len(extra_)}#{extra_}"
            s += "[" + f"{n.name},{n.start},{n.end},{n.nid}${extra_}"
            for i in n.nodes:
                s = __f(i, s)
            s += "]"
        return s

    return __f(n, s)


# Node = _itree.Node
# create_virtual_node = _itree.create_virtual_node


class Node(_itree.Node):
    """
    Node is a nary interval tree node with the following member variables:
    .. highlight:: python
    .. code-block:: python
        nid - node id, unique in a tree
        name - node name
        start - start value
        end - end value
        extra - a dictionary of extra information
        nodes - children of the current node
    When value is time itself, the start and end are timestamps. When value is not time, for instance network traffic, the timestamp can be stored in the extra dictionary.
    """

    def __init__(self, name="", start=0.0, end=0.0, extra=None, nid=0, node=None):
        if extra is None:
            extra = {}
        if node is None:
            super().__init__(name, start, end, extra)
            if nid:
                self.nid = nid
        else:
            # copy constructor
            super().__init__(node.name, node.start, node.end, node.extra)
            self.nid = node.nid
            self.nodes = node.nodes

    """
    def __getstate__(self):
        return serialize_node(self)


    def __setstate__(self, d):
        super().__setstate__(d)
        print(d)
        t = deserialize_node(d)
        self = t
        return self
        self.name = t.name
        self.start = t.start
        self.end = t.end
        self.extra = t.extra
        self.nid = t.nid
        self.nodes = t.nodes
    """


def create_virtual_node() -> Node:
    return Node(node=_itree.create_virtual_node_())


def _consolidate(node: Node):
    if isinstance(node, (Node, _itree.Node)):
        n = _itree.consolidate(node)
        if len(n.nodes) == 1:
            return n.nodes[0]
        return n
    return None


def __create_tmp_node():
    return Node(name="", start=0, end=0)


def str_to_number(x) -> Union[int, float]:
    x = float(x)
    if x.is_integer():
        return int(x)
    else:
        return x


def deserialize_node(bs: str):
    if not bs:
        return None
    if isinstance(bs, bytes):
        bs = bs.decode("utf-8")
    assert bs[0] == 'n', "data format error"
    i = bs.index('*')
    version = int(bs[1:i])
    bs = bs[i + 1 :]

    stk_ = [create_virtual_node()]
    s = ""
    i = 0
    while i < len(bs):
        ch = bs[i]
        if ch == "[":
            if len(stk_) == 1:
                stk_.append(__create_tmp_node())
                stk_[0].append(stk_[1])
            else:
                t = __create_tmp_node()
                stk_[-1].append(t)
                stk_.append(t)
        elif ch == "$":
            if s:
                kv = s.split(",")
                stk_[-1].name = kv[0]
                stk_[-1].start = str_to_number(kv[1])
                stk_[-1].end = str_to_number(kv[2])
                stk_[-1].nid = int(kv[3])
                s = ""
                extra_len_str, remaining = bs[i + 1 :].split("#", 1)
                extra_len = int(extra_len_str)
                if extra_len == 0:
                    stk_[-1].extra = {}
                else:
                    stk_[-1].extra = ast.literal_eval(remaining[:extra_len])
                i += extra_len + 1 + len(extra_len_str) + 1
                continue
        elif ch == "]":
            assert not s, "s should be empty"
            if len(stk_) > 1:
                stk_.pop()
        else:
            s += ch
        i += 1

    assert len(stk_) == 1, "bad tree data"
    r = _consolidate(stk_[0])
    if isinstance(r, _itree.Node):
        r = Node(node=r)
    # print(r)
    return r


if __name__ == '__main__':
    import pickle

    fruit = itree.Node('fruit')
    apple = itree.Node('apple', 0, 1, {"x": 1})
    fruit.append(apple)
    fruit.end = 100
    pear = itree.Node('pear', 4, 5, {"x": 4})
    fruit.append(pear)

    data = pickle.dumps(fruit)
    print(data)
    print(itree.serialize_node(fruit))
    fruit_v2 = pickle.loads(data)
    print(str(fruit), str(fruit_v2))
