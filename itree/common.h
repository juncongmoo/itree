#ifndef ITREE_SRC_COMMON
#define ITREE_SRC_COMMON
#include "shared.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

static const std::string VNODE_NAME = "None";

// VS split(string s, const string &delimiter);
// string vc_to_str(const vector<char> &);
// vector<char> str_to_vc(const string &);

/*
string dict_to_b64_str(const map<string, string> &);
py::dict b64_str_to_dict(const string &);

string map_to_string(const py::dict &);
py::dict str_to_map(const string &s);
*/

// bool is_str_dict();

void update_dict(py::dict &a, const py::dict &b);

uint64_t time_us();
uint64_t time_ms();
uint64_t time_s();

// py::object __mod(const string &);
py::object mod(const py::str &, const py::str& attr="", py::bool_ verbose=true);
// py::object mod2(const py::list &);
py::dict _exe(const string &, py::dict &l);
// py::object load_json();
string get_uuid();

vector<string> split(string str, const string &token, int max_ = INT_MAX);
int _stoi(const string &, const string &);
string str_to_b64(const string &s);
string b64_to_str(const string &s);
#endif
