#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>
#include <vector>
#include <set>

template <class T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  out << "[";
  size_t last = v.size() - 1;
  for(size_t i = 0; i <= last; ++i) {
    out << v[i];
    if (i != last) {
      out << ", ";
    }
  }
  out << "]";
  return out;
}

template <class T>
bool vectorContains(std::vector<T>& v, T element) {
  if(std::find(v.begin(), v.end(), element) != v.end()) {
    return true;
  }
  return false;
}

template <class T>
std::vector<T> vectorDiff(std::vector<T>& v1, std::vector<T>& v2) {
  std::set<int> s_v1(v1.begin(), v1.end());
  std::set<int> s_v2(v2.begin(), v2.end());
  std::vector<int> result;
  std::set_difference(s_v1.begin(), s_v1.end(), s_v2.begin(), s_v2.end(),
      std::back_inserter(result));
  return result;
}

template <class T>
bool vectorSameValues(std::vector<T>& v1, std::vector<T>& v2) {
  if(v1.size() != v2.size()) {
    return false;
  }
  std::set<int> s_v1(v1.begin(), v1.end());
  std::set<int> s_v2(v2.begin(), v2.end());
  std::vector<int> result;
  std::set_difference(s_v1.begin(), s_v1.end(), s_v2.begin(), s_v2.end(),
      std::back_inserter(result));
  return result.size() == 0;
}

#endif
