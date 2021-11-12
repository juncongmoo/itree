#include "forest.h"
#include "nemo.h"
#include "node.h"
#include "shared.h"
#include "tree.h"
#include <iomanip> //put_time
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace std;

string get_time() {
  stringstream ss;
  auto t = time(nullptr);
  auto tm = *localtime(&t);
  ss << put_time(&tm, "%Y-%m-%d %H-%M-%S");
  return ss.str();
}

PYBIND11_MODULE(_itree, m) {
  m.doc() = R"--(
    iTree (Yet Another Tree Library exclusively for structural time series data)

    iTree is a high performance nary Interval Tree library designed exclusively for structural time series data.

    )--";
  m.ver() = "1.0.0";
  m.author() = "Juncong Moo";
  m.date() = "2021-06-01";
  m.credits() = "Wendy Lee";
  py::class_<Forest>(m, "Forest", R"--(
    Forest

    Forest contains many trees. It is a map like data structure to store all the structural time series data and cache data.
    The mapping relation is defined as:
        id => key => interval tree

    )--")

      .def(py::init<>())
      .def("load_tpl", &Forest::load_tpl)
      .def("get_dio_bytes_r", &Forest::get_dio_bytes_r)
      .def("get_dio_bytes_w", &Forest::get_dio_bytes_w)
      .def("get_sio_bytes_r", &Forest::get_sio_bytes_r)
      .def("get_sio_bytes_w", &Forest::get_sio_bytes_w)
      .def("get_nio_bytes_r", &Forest::get_nio_bytes_r)
      .def("get_nio_bytes_w", &Forest::get_nio_bytes_w)
      .def_readwrite("dio_bytes_r", &Forest::dio_bytes_r)
      .def_readwrite("dio_bytes_w", &Forest::dio_bytes_w)
      .def_readwrite("sio_bytes_r", &Forest::sio_bytes_r)
      .def_readwrite("sio_bytes_w", &Forest::sio_bytes_w)
      .def_readwrite("nio_bytes_r", &Forest::nio_bytes_r)
      .def_readwrite("nio_bytes_w", &Forest::nio_bytes_w)
      .def_readwrite("overhead_us", &Forest::overhead_us)
      .def_readwrite("enabled", &Forest::enabled)
      .def_readwrite("attach_timestamp", &Forest::attach_timestamp)
      .def_readwrite("itree_tpl", &Forest::itree_tpl)
      .def_readwrite("fast_tail", &Forest::fast_tail)
      .def_readwrite("init_time_us", &Forest::init_time_us)
      //.def("deserialize", &Tree::deserialize)
      ;
  py::class_<Tree, shared_ptr<Tree>>(m, "Tree", R"--(
    Tree

    In iTree, tree is a nary interval tree, plus a stack and map. Different from the traditional BS, AVL, RB Tree or interval tree, tree in iTree is a strictly insertion-time-ordered tree from top to bottom and from left to right. The values could be monotonic or not monotonic, which is marked by a `monotonic` member variable.

    )--")
      .def(py::init<const string &, const py::dict &, bool, int,
                    double>(),
           py::arg("tree_id") = py::str(),
           py::arg("extra") = py::dict(), py::arg("monotonic") = true,
           py::arg("capacity") = 1024,
           py::arg("zsn_threshold") =
               1e-7) // in most cases, a tree has less than 1k nodes
      .def("is_valid", &Tree::is_valid, "check if it is a valid itree")
      .def("get_root", &Tree::get_root)
      .def("add_node", &Tree::add_node, py::arg("node") = py::none(), "add a new complete node into the tree, and return true if added or false is not")
      .def("discover", &Tree::discover)
      .def("finish", &Tree::finish)
      .def("__repr__", &Tree::repr)
      .def("__str__", &Tree::repr)
      .def("to_dot_string", &Tree::to_dot_string,
           py::arg("node_shape") = py::str("circle"))
      .def("set_as_non_monotonic", &Tree::set_as_non_monotonic,
           "mark it as non-monotonic")
      .def_readwrite("extra", &Tree::extra)
      .def_readwrite("tree_id", &Tree::tree_id)
      .def_readwrite("root", &Tree::root)
      .def_readwrite("stk", &Tree::stk)
      .def_readwrite(
          "mode", &Tree::mode,
          R"-(0 - concise mode. do not record zero-span node [default]
        1 - verbose mode. record zero-span node when extra is not empty
        2 - debug mode. always record zero span node)-")
      .def_readwrite(
          "count", &Tree::count,
          "the total number of the childen not including the virtual root node")
      .def_readwrite(
          "depth", &Tree::depth,
          "the max depth of the nary tree, where virtual root has depth 1")
      //.def("deserialize", &Tree::deserialize)
      ;
  py::class_<Node, shared_ptr<Node>>(m, "Node", R"--(
    Node

    Node is a nary interval tree node with 5 member variables:
        name - node name
        start - start value
        end - end value
        extra - a dictionary of extra information
        nodes - childen of the current node

    When value is time itself, the start and end are time stamp. When value is not time, like network traffic, the time stamp can be stored in the extra dictionary.
    )--")
      .def(py::init<const string &, double, double, const py::dict &>(),
           py::arg("name") = py::str(), py::arg("start") = 0.0,
           py::arg("end") = 0.0, py::arg("extra") = py::dict())
      .def("__str__", &Node::__str__)
      .def("__repr__", &Node::__repr__)
      .def_readwrite("extra", &Node::extra)
      .def_readwrite("start", &Node::start)
      .def_readwrite("end", &Node::end)
      .def_readwrite("nodes", &Node::nodes)
      .def_readwrite("name", &Node::name)
      .def("span", &Node::span)
      .def("append", &Node::append)
      .def("add_child", &Node::add_child);

  m.def("nemo_transform", &decode);
  m.def("create_virtual_node", &create_virtual_node);
  m.def("create_tmp_node", &create_tmp_node);
  m.def("is_virtual_node", &is_virtual_node);
  m.def("consolidate", &consolidate);
  m.def("time_s", &time_s);
  m.def("time_ms", &time_ms);
  m.def("time_us", &time_us);
  m.def("mod", &mod, py::arg("module_name") = py::str(),
        "load python module with a string-type module_name without exception");
  // m.def("mod2", &mod2);
  m.def("_exe", &_exe, py::arg("s") = py::str(), py::arg("l") = py::dict());
  m.def("uuid", &get_uuid);
  // m.def("deserialize", &deserialize);
}

// https://stackoverflow.com/questions/15452828/pydoc-supported-python-metadata-such-as-version-0-1
