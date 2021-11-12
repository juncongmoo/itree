#pragma once
#include "node.h"
#include <memory>

shared_ptr<Node> create_virtual_node();
shared_ptr<Node> create_tmp_node();
bool is_virtual_node(const shared_ptr<Node> &n);
// shared_ptr<Node> deserialize(const string &);
shared_ptr<Node> consolidate(const shared_ptr<Node> &node);
string to_dot_string(const shared_ptr<Node> &root, const string &node_shape);
