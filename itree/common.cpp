#include "common.h"
#include "b64.h"
#include <pybind11/eval.h>


/*VS split(string s, const string &delimiter) {
    VS r;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != string::npos) {
        r.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.size()); // or s=s.substr(pos+delimiter.size())
    }
    if (!s.empty())
        r.push_back(s); // Don't forget this
    return r;
}

string vc_to_str(const vector<char> &vc) {
    std::string str(vc.begin(), vc.end());
    return str;
}

vector<char> str_to_vc(const string &s) {
    vector<char> vc;
    std::copy(s.begin(), s.end(), std::back_inserter(vc));
    return vc;
}

string dict_to_b64_str(map<string, string> &m) {
    string buffer = map_to_string(m);
    return Base64::Encode(&buffer[0], buffer.size());
}

map<string, string> b64_str_to_dict(const string &s) {
    string out;
    Base64::Decode(s, out);
    return str_to_map(out);
}

std::string _strip(const std::string &s) {
    auto start_it = s.begin();
    auto end_it = s.rbegin();
    while (std::isspace(*start_it))
        ++start_it;
    while (std::isspace(*end_it))
        ++end_it;
    return std::string(start_it, end_it.base());
}

// A stop-gap function
bool is_str_dict(const string &s) {
    string s_ = _strip(s);
    return !s_.empty() and s_[0] == '{' and s_.back() == '}';
}

void _read(const string &buffer, string &str, int &offset) {
    size_t size = (int)(*(buffer.data() + offset));
    offset += sizeof(size_t);
    std::string str2(buffer.data() + offset, buffer.data() + offset + size);
    str = str2;
    offset += size;
}

void _write(std::stringstream &ss, std::string &str) {
    size_t size = str.size();
    ss.write((char *)(&size), sizeof(size));
    ss.write((char *)(str.data()), str.length());
}


string map_to_string(const map<string, string> &m) {
    std::stringstream ss;
    for (auto &i : m) {
        auto str = i.first;
        auto value = i.second;
        _write(ss, str);
        _write(ss, value);
    }
    return ss.str();
}
map<string, string> str_to_map(const string &s) {
    map<string, string> m;
    int offset = 0;
    while (offset < s.size()) {
        string key;
        string value;
        _read(s, key, offset);
        _read(s, value, offset);
        m[key] = value;
    }
    return m;
}*/

void update_dict(py::dict &a, const py::dict &b) {
    for (const auto &item : b) {
        a[item.first] = item.second;
    }
}

uint64_t time_us() {
    return chrono::duration_cast<std::chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t time_ms() {
    return chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

uint64_t time_s() {
    return chrono::duration_cast<std::chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
}

py::object mod(const py::str &s) {
    try {
        return py::module::import("importlib").attr("import_module")(s);
    } catch (...) {
        return py::none();
    }
}

py::dict _exe(const string &s, py::dict &l) {
    py::exec(s, py::globals(), l);
    return l;
}

/*py::object load_json() {
    const static std::vector<string> t{"ujson", "rapidjson", "json"};
    const static py::list json_list = py::cast(t);
    return mod2(json_list);
}*/

vector<string> split(string str, string token, int max_) {
  vector<string> result;
  int count = 0;
  while (str.size()) {
    size_t index = str.find(token);
    if (index != string::npos) {
      result.push_back(str.substr(0, index));
      str = str.substr(index + token.size());
      if (str.size() == 0)
        result.push_back(str);
    } else {
      result.push_back(str);
      str = "";
    }
    if (++count == max_) {
      if (!str.empty()) {
        result.push_back(str);
      }
      break;
    }
  }
  return result;
}
