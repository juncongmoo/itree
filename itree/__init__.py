from pickle import PicklingError
from .version import __version__

try:
    from . import _itree
except ImportError:
    import _itree


from .node import Node
from .tree import Tree

from .node import serialize_node, deserialize_node

# ForestStats is a collection of trees
ForestStats = _itree.ForestStats

nemo_transform = _itree.nemo_transform
uuid = _itree.uuid
mod = _itree.mod

exe = _itree._exe
time_s = _itree.time_s
time_ms = _itree.time_ms
time_us = _itree.time_us
is_virtual_node = _itree.is_virtual_node_
create_virtual_node = _itree.create_virtual_node_
create_tmp_tree = _itree.create_tmp_tree
create_tmp_node = _itree.create_tmp_node


def is_picklable(o):
    import pickle

    try:
        pickle.dumps(o)
    except (pickle.PicklingError, TypeError):
        return False
    return True
