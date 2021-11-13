#include "node.h"
#include "common.h"
#include "node_utils.h"
#include "fmt.h"

// we don't include nid here because it doesn't make sense at node level
// it makes sense at tree level to differentiate the nodes with the same name
// a function should be provided at tree level to print its node with nid
string Node::__str__() {
    if (span()==0.0)
      return string_format("[🔵 n=%s]", name.c_str());
    if (nodes.empty())
      return string_format("[🍁 n=%s,s=%.2f,e=%.2f,x=%ld,c=%ld]",
                           name.c_str(), start, end, extra.size(),
                           nodes.size());
    return string_format("[🟢 n=%s,s=%.2f,e=%.2f,x=%ld,c=%ld]",
     name.c_str(), start, end, extra.size(), nodes.size());
}
string Node::__repr__() { return __str__(); }
double Node::span() { return end - start; }
void Node::append(const shared_ptr<Node> &a) { this->nodes.emplace_back(a); }
void Node::add_child(const Node &a) {
  auto n = make_shared<Node>(a.name, a.start, a.end, a.extra);
  if (!a.nodes.empty())
    n->nodes = a.nodes;
  this->nodes.emplace_back(n);
}
