#pragma once
#include "common.h"
#include "node_utils.h"

using namespace std;
/*
Format:

[v123,run_length of tree_extra,tree_extra]

*/

struct PYBIND11_EXPORT Tree {
    string tid;
    string pid = "";
    shared_ptr<Node> root;
    vector<shared_ptr<Node>> stk;
    py::dict extra;

    // ZERO INTERVAL NODE
    //////////////////////////////////////////////////////////////////////
    // 0 - concise mode. do not record zero-interval node [default]
    // 1 - verbose mode. record zero-interval node when extra is not empty
    // 2 - debug mode. always record zero span node
    int mode = 0;

    // STATISTICS
    //////////////////////////////////////////////////////////////////////
    // the total number of the childen not including the virtual root node
    int count = 0;
    // the max depth of the nary tree, where virtual root has depth 1
    unsigned int depth = 1;
    // the node value is monotonically increasing or decreasing, true by default
    // used when we want to collect statistics for irregular tree like memory tree
    bool monotonic = true;

    // ZIN threshold
    double zin_threshold;

    // https://github.com/pybind/pybind11/issues/957
    Tree(const string &tid_, const py::dict &extra_, bool monotonic_, int capacity, double zin_threshold_)
        : tid(tid_), extra(extra_), monotonic(monotonic_), zin_threshold(zin_threshold_) {
        root = create_virtual_node_();
        stk.reserve(capacity);
        stk.push_back(root);
    }
    bool is_valid();
    shared_ptr<Node> get_root();
    bool add_node(shared_ptr<Node>);
    // void deserialize(const string &);
    void discover(const string &name, double start_, const py::dict &extra);
    void finish(const string &name, double end_, const py::dict &extra);
    // don't want to put in the constructor to make it more complex
    inline void non_mono() { monotonic = false; }
    string repr();
    string to_dot_string(const string &);
    void deserialize(const py::str &);
    bool __eq__(const shared_ptr<Tree>& other);
};

shared_ptr<Tree> create_tmp_tree();
