#pragma once
#include "common.h"
#include "node_utils.h"

using namespace std;

struct PYBIND11_EXPORT Forest {
  long long init_time_us;
  // disk I / O
  long long dio_bytes_r = 0;
  long long dio_bytes_w = 0;

  // system I / O
  long long sio_bytes_r = 0;
  long long sio_bytes_w = 0;

  // network I / O
  long long nio_bytes_r = 0;
  long long nio_bytes_w = 0;

  // latency overhead in microseconds
  long long overhead_us;

  // tracing enabled
  bool enabled = false;

  // attach timestamp for non-time metrics?
  bool attach_timestamp = false;

  // itree template
  string itree_tpl = "";

  // when not populating exceptions and exception happens, do not swallow the
  // exception
  bool fast_tail = false;

  void load_tpl(const string &template_path) {
    if (!template_path.empty()) {
      ifstream t(template_path);
      stringstream buffer;
      buffer << t.rdbuf();
      itree_tpl = buffer.str();
    }
  }

  // metric function

  Forest() : init_time_us(time_us()), overhead_us(0) {}
  inline long long get_dio_bytes_r() { return dio_bytes_r; }
  inline long long get_dio_bytes_w() { return dio_bytes_w; }
  inline long long get_sio_bytes_r() { return sio_bytes_r; }
  inline long long get_sio_bytes_w() { return sio_bytes_w; }
  inline long long get_nio_bytes_r() { return nio_bytes_r; }
  inline long long get_nio_bytes_w() { return nio_bytes_w; }
};
