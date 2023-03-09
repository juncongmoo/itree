#include "tree.h"
#include "fmt.h"
#include "node_utils.h"
#include "pickle.h"
#include <assert.h>

bool Tree::is_valid() { return root != nullptr && !stk.empty(); }

shared_ptr<Node> Tree::get_root() { return root; }

#if 0
void Tree::deserialize(const string &s) {
    root = ::deserialize(s);
    stk[0] = root;
}
#endif

void Tree::discover(const string &name, double start_, const py::dict &extra) {
    auto n = make_shared<Node>(name, start_, 0, extra);
    if (stk.empty()) {
        root = n;
    } else {
        // add it as a child
        stk.back()->append(n);
    }
    stk.emplace_back(n);
    ++count;
}

const static string ZERO_SPAN_NAME = "\t";
void remove_zin(list<shared_ptr<Node>> &nodes);

void Tree::finish(const string &name, double end, const py::dict &extra) {
    if (stk.empty() || (stk.back()->name != name)) {
        printf("💀 debug info: %zu, %s, %s\n", stk.size(), stk.back()->name.c_str(), name.c_str());
        throw runtime_error("stk should be non-empty and name should match!");
    }

#if 1
    if (mode == 0 && abs(end - stk.back()->start) < zin_threshold && stk.back()->nodes.empty()) {
        // printf("debug info: %d, %s, %s\n", stk.size(), stk.back()->name.c_str(),
        // name.c_str());
        stk.back()->name = ZERO_SPAN_NAME;
        auto tmp = stk.back();
        stk.pop_back();
        remove_zin(stk.back()->nodes);
        return;
    }
#endif

    stk.back()->end = end;
    if (!extra.empty()) {
        update_dict(stk.back()->extra, extra);
    }

// remove ZIN
#ifdef REMOVE_ZSN
    if (!stk.back()->nodes.empty() && mode == 0 || (mode == 1 && extra.empty())) {
        auto itr = stk.back()->nodes.cbegin();
        while (itr != stk.back()->nodes.cend()) {
            if ((*itr)->name == ZERO_SPAN_NAME) {
                itr = stk.back()->nodes.erase(itr);
            } else {
                ++itr;
            }
        }
    }
#endif
    if (depth < stk.size()) {
        depth = stk.size();
    }
    stk.pop_back();
}

bool Tree::add_node(shared_ptr<Node> n) {
    if (n == nullptr || n->span() < zin_threshold)
        return false;
    if (stk.empty()) {
        root = n;
    } else {
        // add it as a child
        stk.back()->append(n);
    }
    ++count;
    return true;
}

string Tree::repr() {
    return string_format(
        "(🌳 id=%s,c=%d,x=%d,d=%d,m=%d,o=%d)", tid.c_str(), count, extra.size(), depth, mode, monotonic);
}

string Tree::to_dot_string(const string &node_shape) { return ::to_dot_string(root, node_shape); }

void Tree::deserialize(const py::str &s) { _deserialize_tree(this, s); }

bool Tree::__eq__(const shared_ptr<Tree> &other) {
    bool b = (tid == other->tid && pid == other->pid && mode == other->mode && count == other->count
              && depth == other->depth && monotonic == other->monotonic && zin_threshold == other->zin_threshold);
    if (!b) {
        return false;
    }
    auto x1 = py::cast<py::str>(extra);
    auto x2 = py::cast<py::str>(other->extra);

    b = static_cast<string>(x1) == static_cast<string>(x2);
    if (!b) {
        return false;
    }

    auto s1 = serialize_node_(this->root);
    auto s2 = serialize_node_(other->root);
    return static_cast<string>(s1) == static_cast<string>(s2);
}

void remove_zin(list<shared_ptr<Node>> &nodes) {
    auto itr = nodes.cbegin();
    while (itr != nodes.cend()) {
        if ((*itr)->name == ZERO_SPAN_NAME) {
            itr = nodes.erase(itr);
        } else {
            ++itr;
        }
    }
}

shared_ptr<Tree> create_tmp_tree() {
    auto tree = make_shared<Tree>("", py::dict(), true, 1024, 1e-7);
    return tree;
}
