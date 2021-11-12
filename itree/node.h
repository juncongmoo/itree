#pragma once
#include "shared.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;
struct PYBIND11_EXPORT Node {
  unsigned int nid = 0U;
  string name;
  double start;
  double end;
  py::dict extra;
  list<shared_ptr<Node>> nodes;

  Node(const string &name_, double start_, double end_, const py::dict &extra_)
      : name(name_), start(start_), end(end_), extra(extra_) {
    nodes = {};
  }

  string __str__();
  string __repr__();
  double span();
  void append(const shared_ptr<Node> &);
};
