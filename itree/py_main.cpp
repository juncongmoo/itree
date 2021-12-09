#include "forest_stats.h"
#include "nemo.h"
#include "node.h"
#include "node_utils.h"
#include "pickle.h"
#include "shared.h"
#include "tree.h"
#include <iomanip> //put_time
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;
using namespace pybind11::literals;

string get_time() {
    stringstream ss;
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    ss << put_time(&tm, "%Y-%m-%d %H-%M-%S");
    return ss.str();
}

PYBIND11_MODULE(_itree, m) {
    m.doc() = R"--(
    iTree (A Interval Tree Library for structural time series data)

    iTree is a high performance nary Interval Tree library designed for structural time series data.

    )--";
    m.ver() = "0.0.4";
    m.author() = "Juncong Moo";
    m.date() = "2021-06-01";
    m.credits() = "Wendy Lee";
    py::class_<ForestStats>(m, "ForestStats", R"--(
    ForestStats

    ForestStats contains many trees for system statistics collection.
    It is a map like data structure to store all the structural time series data and cache data.
    The mapping relation is defined as:
        id => key => interval tree

    )--")

        .def(py::init<>())
        .def("load_tpl", &ForestStats::load_tpl)
        .def("get_dio_bytes_r", &ForestStats::get_dio_bytes_r)
        .def("get_dio_bytes_w", &ForestStats::get_dio_bytes_w)
        .def("get_sio_bytes_r", &ForestStats::get_sio_bytes_r)
        .def("get_sio_bytes_w", &ForestStats::get_sio_bytes_w)
        .def("get_nio_bytes_r", &ForestStats::get_nio_bytes_r)
        .def("get_nio_bytes_w", &ForestStats::get_nio_bytes_w)
        .def_readwrite("dio_bytes_r", &ForestStats::dio_bytes_r)
        .def_readwrite("dio_bytes_w", &ForestStats::dio_bytes_w)
        .def_readwrite("sio_bytes_r", &ForestStats::sio_bytes_r)
        .def_readwrite("sio_bytes_w", &ForestStats::sio_bytes_w)
        .def_readwrite("nio_bytes_r", &ForestStats::nio_bytes_r)
        .def_readwrite("nio_bytes_w", &ForestStats::nio_bytes_w)
        .def_readwrite("overhead_us", &ForestStats::overhead_us)
        .def_readwrite("enabled", &ForestStats::enabled)
        .def_readwrite("attach_timestamp", &ForestStats::attach_timestamp)
        .def_readwrite("itree_tpl", &ForestStats::itree_tpl)
        .def_readwrite("fast_fail", &ForestStats::fast_fail)
        //.def_readwrite("forest", &ForestStats::forest)
        .def_readwrite("init_time_us", &ForestStats::init_time_us)
        .def(py::pickle(
            [](const ForestStats &fr) { // __getstate__
                auto res = serialize_forest_(fr);
                // py::print("\nr:", res);
                return res;
            },
            [](py::str s) { // __setstate__
                // py::print("\ns:", s);
                auto fr = deserialize_forest_(s);
                return fr;
            }));
    py::class_<Tree, shared_ptr<Tree>>(m, "Tree", R"--(
    Tree

    In iTree, tree is a nary interval tree, plus a stack and map. Different from the traditional BS, AVL, RB Tree or interval tree, tree in iTree is a strictly insertion-time-ordered tree from top to bottom and from left to right. The values could be monotonic or not monotonic, which is marked by a `monotonic` member variable.

    )--")
        .def(py::init<const string &, const py::dict &, bool, int, double>(),
             py::arg("tid") = py::str(),
             py::arg("extra") = py::dict(),
             py::arg("monotonic") = true,
             py::arg("capacity") = 1024,
             py::arg("zin_threshold") = 1e-7) // in most cases, a tree has less than 1k nodes
        .def("is_valid", &Tree::is_valid, "check if it is a valid itree")
        .def("get_root", &Tree::get_root)
        .def("add_node",
             &Tree::add_node,
             py::arg("node") = py::none(),
             "add a new complete node into the tree, and return true if added or "
             "false is not")
        .def("discover", &Tree::discover)
        .def("finish", &Tree::finish)
        .def("__repr__", &Tree::repr)
        .def("__str__", &Tree::repr)
        .def("__eq__", &Tree::__eq__)
        .def("to_dot_string", &Tree::to_dot_string, py::arg("node_shape") = py::str("circle"))
        .def("non_mono", &Tree::non_mono, "mark tree as non-monotonic")
        .def("deserialize", &Tree::deserialize, "deserialize string to a Tree object itself")
        .def_readwrite("extra", &Tree::extra)
        .def_readwrite("tid", &Tree::tid)
        .def_readwrite("root", &Tree::root)
        .def_readwrite("stk", &Tree::stk)
        .def_readwrite("mode",
                       &Tree::mode,
                       R"--(
        0 - concise mode. do not record zero-span node [default]
        1 - verbose mode. record zero-span node when extra is not empty
        2 - debug mode. always record zero span node
        )--")
        .def_readwrite("count", &Tree::count, "the total number of the childen not including the virtual root node")
        .def_readwrite("depth", &Tree::depth, "the max depth of the nary tree, where virtual root has depth 1")
        //.def("deserialize", &Tree::deserialize)
        .def(py::pickle(
            [](const shared_ptr<Tree> &tr) { // __getstate__
                auto res = serialize_tree_(tr);
                // py::print("\nr:", res);
                return res;
            },
            [](py::str s) { // __setstate__
                // py::print("\ns:", s);
                auto _t = deserialize_tree_(s);
                return _t;
            }));
    py::class_<Node, shared_ptr<Node>>(m, "Node", R"--(
    Node

    Node is a nary interval tree node with the following member variables:
        nid - node id, unique in a tree
        name - node name
        start - start value
        end - end value
        extra - a dictionary of extra information
        nodes - childen of the current node

    When value is time itself, the start and end are timestamps. When value is not time, for instance network traffic, the time stamp can be stored in the extra dictionary.
    )--")
        .def(py::init<const string &, double, double, const py::dict &>(),
             py::arg("name") = py::str(),
             py::arg("start") = 0.0,
             py::arg("end") = 0.0,
             py::arg("extra") = py::dict())
        .def("__str__", &Node::__str__)
        .def("__repr__", &Node::__repr__)
        .def_readwrite("nid", &Node::nid)
        .def_readwrite("extra", &Node::extra)
        .def_readwrite("start", &Node::start)
        .def_readwrite("end", &Node::end)
        .def_readwrite("nodes", &Node::nodes)
        .def_readwrite("name", &Node::name)
        .def("span", &Node::span)
        .def("append", &Node::append, "append as current node's children by referencing")
        .def("add_child",
             &Node::add_child,
             "add as current node's children by shallow-copying")
        .def(py::pickle(
            [](const shared_ptr<Node> &n) { // __getstate__
                /* Return a tuple that fully encodes the state of the object */
                // py::print("xxxxxxxxxxxxxxxxxxxx");
                auto res = serialize_node_(n);
                // py::print(res);
                return res;
            },
            [](py::str s) { // __setstate__
                // py::print(s);
                // shared_ptr<Node> r = create_tmp_node();
                // py::print("yyyyyyyyyyyyyyyyyyyy");
                auto res = deserialize_node_(s);
                return res;
            }));

    m.def("nemo_transform", &encode);
    m.def("nemo_recover", &decode);
    m.def("create_virtual_node_", &create_virtual_node_);
    m.def("create_tmp_node", &create_tmp_node);
    m.def("create_tmp_tree", &create_tmp_tree);
    m.def("is_virtual_node_", &is_virtual_node_, "check if the node is virtual or not");
    m.def("consolidate", &consolidate, R"--(
merge virtual nodes into one
)--");
    m.def("time_s", &time_s);
    m.def("time_ms", &time_ms);
    m.def("time_us", &time_us);
    
    m.def("serialize_node_", &serialize_node_);
    m.def("serialize_tree_", &serialize_tree_);
    m.def("serialize_forest_", &serialize_forest_);

    m.def("deserialize_node_", &deserialize_node_);
    m.def("deserialize_tree_", &deserialize_tree_);
    m.def("deserialize_forest_", &deserialize_forest_);

    m.def("mod",
          &mod,
          py::arg("module_name") = py::str(),
          "load python module with a string-type module_name without "
          "exception(None will be returned in case of exception)");
    m.def("_exe",
          &_exe,
          py::arg("s") = py::str(),
          py::arg("l") = py::dict(),
          R"--(
execute python statement s with locals dictionary l, and return l
)--");
    m.def("uuid", &get_uuid, R"--(
a fast uuid generator by caching.

call uuid() for the first time will pre-generate 10240 uuids in memory

call it before the business logic starts to run
)--");
}

// https://stackoverflow.com/questions/15452828/pydoc-supported-python-metadata-such-as-version-0-1
