#pragma once
#include "node.h"
#include <memory>

shared_ptr<Node> create_virtual_node_();
shared_ptr<Node> create_tmp_node();
bool is_virtual_node_(const shared_ptr<Node> &n);
// shared_ptr<Node> deserialize(const string &);

// merge multiple virtual nodes into one
shared_ptr<Node> consolidate(const shared_ptr<Node> &node);
string to_dot_string(const shared_ptr<Node> &root, const string &node_shape);
