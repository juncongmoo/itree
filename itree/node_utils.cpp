#include "node_utils.h"
#include "common.h"
#include "shared.h"

static const double inf = std::numeric_limits<double>::infinity();

shared_ptr<Node> create_virtual_node() {
    py::dict m = {};
    return make_shared<Node>(VNODE_NAME, inf, inf, m);
}

shared_ptr<Node> create_tmp_node() {
    py::dict m = {};
    return make_shared<Node>("", 0, 0, m);
}

string str_to_lower(const string &s) {
    string data(s);
    transform(data.begin(), data.end(), data.begin(), [](unsigned char c) { return tolower(c); });
    return data;
}

bool is_virtual_node(const shared_ptr<Node> &n) {
    // auto a = to_string(n->start);
    // auto b = to_string(n->end);
    // string lower_name = str_to_lower(n->name);
    return ((n->start == inf and n->end == inf) or (n->start == -inf and n->end == inf) or
            (n->start == inf and n->end == -inf));
}

/*void __parse_node(const string &s, double &start, double &end, py::dict &extra) {
    VS v = split(s, ",");
    assert(v.size() == 3);

    start = stod(v[0]);
    end = stod(v[1]);
    string tmp = v[2];
    // if (!tmp.empty()) {
    //    extra = b64_str_to_dict(tmp);
    //}
}

shared_ptr<Node> deserialize(const string &bs) {
    vector<shared_ptr<Node>> stk;
    stk.push_back(create_virtual_node());
    string s;
    REP(i, 0, bs.size()) {
        char ch = bs[i];
        if (ch == '{') {
            if (stk.size() == 1) {
                stk.push_back(create_tmp_node());
                stk[0]->nodes.push_back(stk[1]);
            } else {
                if (!s.empty()) {
                    auto kv = split(s, ":");
                    stk.back()->name = kv[0];
                    __parse_node(kv[1], stk.back()->start, stk.back()->end, stk.back()->extra);
                    s = "";
                }
                shared_ptr<Node> new_node = create_tmp_node();
                stk.back()->nodes.push_back(new_node);
                stk.push_back(new_node);
            }
        } else if (ch == '}') {
            if (!s.empty()) {
                auto kv = split(s, ":");
                stk.back()->name = kv[0];
                __parse_node(kv[1], stk.back()->start, stk.back()->end, stk.back()->extra);
                s = "";
            }
            if (stk.size() > 1) {
                stk.pop_back();
            }
        } else {
            s += ch;
        }
    }
    assert(stk.size() == 1);
    return stk[0];
}*/

shared_ptr<Node> consolidate(const shared_ptr<Node> &node) {
    shared_ptr<Node> n(node);
    while (is_virtual_node(n)) {
        if (n->nodes.size() == 1 and is_virtual_node(n->nodes.front())) {
            n = n->nodes.front();
        } else {
            break;
        }
    }

    if (is_virtual_node(n) and !n->nodes.empty()) {
        for (const auto &e : n->nodes) {
            if (n->start == inf or n->start == -inf) {
                n->start = e->start;
            } else {
                n->start = min(n->start, e->start);
            }
            if (n->end == inf or n->end == -inf) {
                n->end = e->end;
            } else
                n->end = max(n->end, e->end);
        }
    }
    return n;
}
