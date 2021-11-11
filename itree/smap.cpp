#include "smap.h"

void test1(void) {
    std::cout << "string->int" << std::endl;
    smap<std::string, int> m;
    m["test1"] = 1;
    m["test2"] = 2;
    m["test3"] = 3;
    std::vector<char> buffer = m.serialize();
    smap<std::string, int> m2;
    m2.deserialize(buffer);
    m2.show();
}

void test2(void) {
    std::cout << "int->string" << std::endl;
    smap<int, std::string> m;
    m[1] = "test1";
    m[2] = "test2";
    m[3] = "test3";
    std::vector<char> buffer = m.serialize();
    smap<int, std::string> m2;
    m2.deserialize(buffer);
    m2.show();
}

void test3(void) {
    std::cout << "int->int" << std::endl;
    smap<int, int> m;
    m[1] = 1;
    m[2] = 2;
    m[3] = 3;
    std::vector<char> buffer = m.serialize();
    smap<int, int> m2;
    m2.deserialize(buffer);
    m2.show();
}

int main(void) {
    test1();
    test2();
    test3();
}
