#define UUID_SYSTEM_GENERATOR
#include "shared.hpp"
#include <random>

/*
Python 3.8.10 (default, Sep 28 2021, 16:10:42)
[GCC 9.3.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import time, _hiq
>>> a=time.monotonic();_hiq.uuid();b=time.monotonic();
'20211025Kyg1AXUCmEbVSObB'
>>> (b-a)*1e6
138.62783089280128
>>> a=time.monotonic();_hiq.uuid();b=time.monotonic();
'20211025imrQhq0tguhmI33Y'
>>> (b-a)*1e6
22.32613041996956
>>> len('20211025imrQhq0tguhmI33Y')
24
*/

string get_dt() {
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);

    ostringstream osTime;
    osTime << (now->tm_mon + 1) << now->tm_mday;
    return osTime.str();
}

static const string now_dt = get_dt();
const static int SZ = 1024;
static const size_t S_LEN = 16;
static vector<string> cache = vector<string>(SZ, string(S_LEN, '0'));

void gen() {
    // https://stackoverflow.com/questions/19515557/fast-generate-a-large-set-of-random-strings-from-number-and-alphabet
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    static random_device rd;
    static default_random_engine rng(rd());
    static uniform_int_distribution<> dist(0, sizeof(alphabet) / sizeof(*alphabet) - 2);

    REP(k, 0, SZ) {
        REP(i, 0, S_LEN) { cache[k][i] = alphabet[dist(rng)]; }
    }
}

string get_uuid() {
    static int i = 0;
    if (i == 0) {
        gen();
    } else if (i == SZ - 1) {
        i = 0;
        gen();
    }
    return now_dt + cache[i++];
}
