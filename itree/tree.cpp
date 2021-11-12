#include "tree.h"
#include "node_utils.h"
#include <assert.h>
#include "fmt.h"

bool Tree::is_valid() { return root != nullptr and !stk.empty(); }

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
void remove_zsn(list<shared_ptr<Node>> &nodes);

void Tree::finish(const string &name, double end, const py::dict &extra) {
  if (stk.empty() or (stk.back()->name != name)) {
    printf("💀 debug info: %zu, %s, %s\n", stk.size(), stk.back()->name.c_str(),
           name.c_str());
    throw std::runtime_error("stk should be non-empty and name should match!");
  }

#if 1
  if (mode == 0 and abs(end - stk.back()->start) < zsn_threshold and
      stk.back()->nodes.empty()) {
    // printf("debug info: %d, %s, %s\n", stk.size(), stk.back()->name.c_str(),
    // name.c_str());
    stk.back()->name = ZERO_SPAN_NAME;
    auto tmp = stk.back();
    stk.pop_back();
    remove_zsn(stk.back()->nodes);
    return;
  }
#endif

  stk.back()->end = end;
  if (!extra.empty()) {
    update_dict(stk.back()->extra, extra);
  }

// remove ZSN
#ifdef REMOVE_ZSN
  if (!stk.back()->nodes.empty() and mode == 0 or
      (mode == 1 and extra.empty())) {
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

void Tree::add_node(shared_ptr<Node> n) {
  if (n->span() < zsn_threshold)
    return;
  if (stk.empty()) {
    root = n;
  } else {
    // add it as a child
    stk.back()->append(n);
  }
  ++count;
}

string Tree::repr() {
  return string_format("(🌳 id=%s,c=%d,x=%d,d=%d,m=%d,o=%d)", tree_id.c_str(), count,
                     extra.size(), depth, mode, monotonic);
}

void __dfs(const shared_ptr<Node> &n, unordered_set<string> &x,
           unordered_map<string, VS> &y, string &max_leaf_node_name,
           double &max_interval) {
  x.insert(n->name);
  if (n->nodes.empty()) {
    if (n->span() > max_interval) {
      max_leaf_node_name = n->name;
      max_interval = n->span();
    }
  }
  for (const auto &e : n->nodes) {
    y[n->name].push_back(e->name);
    __dfs(e, x, y, max_leaf_node_name, max_interval);
  }
}


/*
digraph tree{
        node [shape=record margin=0 fontcolor=blue fontsize=8 width=0.5 style=filled fixedsize=true]
        edge [fontsize=7]
        rankdir=LR

        "unicorn" [shape=record label="unicorn | c | d "];
        "pizza" [shape=record label="one|123|456|1.23"];
        "monkey";
        "root";
        "l2" [color="green" style="filled" fillcolor="yellow" label="one|123|456|1.23"];
        "None" [shape="doublecircle" color="orange" style="filled" fillcolor="red"];
        "piggy";
        "icecream";
        "l1";
        "egg";
        "unicorn" -> "monkey" ;
        "root" -> "l1";
        "root" -> "l2";
        "root" -> "l2";
        "root" -> "piggy";
        "l2" -> "egg";
        "None" -> "root";
        "piggy" -> "unicorn";
        "icecream" -> "pizza";
        "l1" -> "l2";
        "egg" -> "icecream";
}
*/
string Tree::to_dot_string(const string &node_shape) {
  string s = "digraph tree{\n";
  s += string_format(
      "\tnode [shape=%s margin=0 fontcolor=blue fontsize=32 width=0.5 style=filled]"
      "\n\tedge [fontsize=8]"
      "\n\trankdir=LR\n\n",
      node_shape.c_str());
  unordered_set<string> node_names;
  unordered_map<string, VS> successors;
  double max_interval = -2147483648.0;
  string max_leaf_node_name = root->name;
  __dfs(root, node_names, successors, max_leaf_node_name, max_interval);
  for (const auto &e : node_names) {
    s += string_format("\t\"%s\"", e.c_str());
    if (e.substr(0,4) == "None") {
      s += " [shape=\"doublecircle\" color=\"orange\" style=\"filled\" "
           "fillcolor=\"green\"]";
    } else if (e == max_leaf_node_name) {
      s += " [shape=\"doublecircle\" color=\"green\" style=\"filled\" "
           "fillcolor=\"yellow\"]";
    }
    s += ";\n";
  }
  for (const auto &e : successors) {
    for (const auto &v : e.second)
      s += string_format("\t\"%s\" -> \"%s\";\n", e.first.c_str(), v.c_str());
  }
  s += "}";
  return s;
}

void remove_zsn(list<shared_ptr<Node>> &nodes) {
  auto itr = nodes.cbegin();
  while (itr != nodes.cend()) {
    if ((*itr)->name == ZERO_SPAN_NAME) {
      itr = nodes.erase(itr);
    } else {
      ++itr;
    }
  }
}
