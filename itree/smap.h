#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

template <class Key, class Value> class smap : public std::map<Key, Value> {
private:
    size_t offset;

    template <class T> void write(std::stringstream &ss, T &t) { ss.write((char *)(&t), sizeof(t)); }

    void write(std::stringstream &ss, std::string &str) {
        size_t size = str.size();
        ss.write((char *)(&size), sizeof(size));
        ss.write((char *)(str.data()), str.length());
    }

    template <class T> void read(std::vector<char> &buffer, T &t) {
        t = (T)(*(buffer.data() + offset));
        offset += sizeof(T);
    }

    void read(std::vector<char> &buffer, std::string &str) {
        size_t size = (int)(*(buffer.data() + offset));
        offset += sizeof(size_t);
        std::string str2(buffer.data() + offset, buffer.data() + offset + size);
        str = str2;
        offset += size;
    }

public:
    std::vector<char> serialize() {
        std::vector<char> buffer;
        std::stringstream ss;
        for (auto &i : (*this)) {
            Key str = i.first;
            Value value = i.second;
            write(ss, str);
            write(ss, value);
        }
        size_t size = ss.str().size();
        buffer.resize(size);
        ss.read(buffer.data(), size);
        return buffer;
    }
    void deserialize(std::vector<char> &buffer) {
        offset = 0;
        while (offset < buffer.size()) {
            Key key;
            Value value;
            read(buffer, key);
            read(buffer, value);
            (*this)[key] = value;
        }
    }
    void show(void) {
        for (auto &i : (*this)) {
            std::cout << i.first << ":" << i.second << std::endl;
        }
        std::cout << std::endl;
    }
};
