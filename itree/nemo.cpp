#include "nemo.h"
#include "b64.h"
#include <regex>
#include <zlib.h>

/** Compress a STL string using zlib with given compression level and return
 * the binary data. */
std::string compress_string(const std::string &str, int compressionlevel) {
    z_stream zs; // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef *) str.data();
    zs.avail_in = str.size(); // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) { // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
std::string decompress_string(const std::string &str) {
    z_stream zs; // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef *) str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef *>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) { // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

/*
def encode(s: str, codec="base64") -> str:
    b = zlib.compress(s.encode("utf-8"), level=3)
    r = codecs.encode(b, codec).decode("utf-8").strip()
    r = r.replace("=", "`")
    f = (r + r)[len(r) // 3 : len(r) + len(r) // 3]
    return f


def decode(s: str, codec="base64") -> str:
    s = s.replace("`", "=")
    m = len(s) - len(s) // 3
    s = (s + s)[m : m + len(s)]
    s = codecs.decode(s.encode("utf-8"), codec)
    return zlib.decompress(s).decode("utf-8")
*/

static map<string, string> TRANSFORMER = {{"==", "🅳"}, {"1", "🐚"}, {"2", "🦉"}, {"3", "🅹"}};

string encode(string s) {
    string r = compress_string(s, Z_BEST_COMPRESSION);
    string s_ = Base64::Encode(r.data(), r.size());
    string s2 = s_ + s_;
    int m = int(s_.size() / 3);
    string tmp = s2.substr(m, s_.size());
    for (size_t i = 0; i < tmp.size(); i++) {
        if (isalpha(tmp[i]))
            tmp[i] ^= 0b100000;
    }
    for (auto &kv : TRANSFORMER) {
        tmp = std::regex_replace(tmp, std::regex(kv.first), kv.second);
    }
    return tmp;
}

string decode(const py::str& ds) {
    string s_ = static_cast<string>(ds);
    for (auto &kv : TRANSFORMER) {
        s_ = std::regex_replace(s_, std::regex(kv.second), kv.first);
    }
    for (size_t i = 0; i < s_.size(); i++) {
        if (isalpha(s_[i]))
            s_[i] ^= 0b100000;
    }
    int m = s_.size() - int(s_.size() / 3);
    string s2 = s_ + s_;
    s_ = s2.substr(m, s_.size());
    string o;
    Base64::Decode(s_, o);
    s_ = decompress_string(o);
    return s_;
}
