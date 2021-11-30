#pragma once
#include "shared.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;
struct PYBIND11_EXPORT Node {
    // node id
    unsigned int nid = 0U;
    // node name
    string name;
    // node start value
    double start;
    // node end value
    double end;
    // extra info
    py::dict extra;
    // children nodes
    list<shared_ptr<Node>> nodes;

    Node(const string &name_, double start_, double end_, const py::dict &extra_)
        : name(name_), start(start_), end(end_), extra(extra_) {
        if (name.find(",") != string::npos)
            throw std::runtime_error("name cannot contain comma");
        nodes = {};
    }

    string __str__();
    string __repr__();
    double span();
    void append(const shared_ptr<Node> &);
    void add_child(const Node &);
};
