#pragma once

#include <map>
using namespace std;

// Whit this you can use map.lower_bound(VALUE) to find the
// relevant version.

template <typename T> using vmap = map<size_t, T, greater<size_t>>;

enum class Side {
  LEFT,
  RIGHT,
};
