#ifndef ITREE_SRC_PICKLE
#define ITREE_SRC_PICKLE

#include "forest_stats.h"
#include "nemo.h"
#include "node.h"
#include "node_utils.h"
#include "shared.h"
#include "tree.h"
#include <iomanip> //put_time
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

py::str serialize_node_(const shared_ptr<Node> &n);
py::str serialize_node_impl(const shared_ptr<Node> &n, py::str &s);
shared_ptr<Node> deserialize_node_(py::str bs);
shared_ptr<Node> deserialize_node_impl(const string &d);

py::str serialize_tree_(const shared_ptr<Tree> &n);
shared_ptr<Tree> deserialize_tree_(py::str bs);
void _deserialize_tree(Tree *tree, py::str bs);

py::str serialize_forest_(const ForestStats &);
ForestStats deserialize_forest_(const py::str &bs);
#endif
