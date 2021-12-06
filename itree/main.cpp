// https://pybind11.readthedocs.io/en/stable/advanced/embedding.html
#include "nemo.h"
#include "shared.h"
#include "tree.h"
#include <pybind11/embed.h> // <= You need this header

namespace py = pybind11;
using namespace py::literals;

// py::dict can be cast to string!!!
void test_tree() {
    string s = "{\"root\":1,15,14{\"l1\":2,5,3{\"l2\":3,4,1}}{\"l2\":7,14,7}}";
    s += s;
    auto x = py::dict("name"_a = "W", "number"_a = 4);
    auto kwargs = py::dict("name"_a = "World", "number"_a = 42, "x"_a = x);
    auto message = "Hello, {name}! The answer is {number}"_s.format(**kwargs);
    auto y = kwargs.cast<py::str>();

    auto ast = py::module::import("ast");
    auto z = ast.attr("literal_eval")(y);

    // auto z = y.cast<py::dict>();
    py::print(kwargs.cast<py::str>());
    py::print(z);
    // Tree t("", "None", extra, false);
    // t.deserialize(s);
    // cout << t.get_root()->span() << endl;
}

void test_nemo() {
    string o = encode("juncongmoo");
    cout << o << endl;
    string r = decode(o);
    cout << r << endl;
}

int main() {
    py::scoped_interpreter guard{}; // <= Initialize the interpreter
    test_nemo();
    test_tree();
    return 0;
}
