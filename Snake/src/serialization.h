#ifndef SNAKE_SERIALIZATION_H_
#define SNAKE_SERIALIZATION_H_

#include <algorithm>
#include <bitset>
#include <iostream>
#include <string>
#include <vector>

#include "util.h"

namespace serialisation {
using byte = unsigned char;

template <typename T>
byte* write(byte** dst, const T& val) {
  auto begin = *dst;
  memcpy(begin, &val, sizeof(T));
  return begin + sizeof(T);
}

template <typename T>
byte* write(byte** dst, const std::vector<T>& val) {
  auto begin = write(dst, val.size());

  memcpy(begin, val.data(), sizeof(T) * val.size());
  return begin + sizeof(T) * val.size();
}

template <>
byte* write(byte** dst, const std::vector<int>& val) {
  auto begin = write(dst, val.size());

  memcpy(begin, val.data(), sizeof(int) * val.size());
  return begin + sizeof(int) * val.size();
}

template <>
byte* write(byte** dst, const std::vector<bool>& val) {
  std::bitset<2> bits(0);  // 2 is max of users
  for (int i = 0, sz = std::min<int>(val.size(), 16); i < sz; ++i)
    bits[i] = val[i];
  return write(dst, static_cast<int>(bits.to_ulong()));
}

byte* readInt(byte* dst, int* val) {
  memcpy(val, dst, sizeof(int));
  return dst + sizeof(int);
}

template <typename T>
byte* read(byte* dst, T* val) {
  memcpy(val, dst, sizeof(T));
  return dst + sizeof(T);
}

template <typename T>
byte* readVector(byte* dst, std::vector<T>* val) {
  size_t size = 0;
  dst = read(dst, &size);

  val->resize(size);
  memcpy(val->data(), dst, sizeof(T) * size);
  return dst + sizeof(T) * size;
}

template <>
byte* readVector(byte* dst, std::vector<bool>* val) {
  int data = 0;
  dst = readInt(dst, &data);
  val->resize(2);
  std::bitset<2> bits(data);  // 2 is max of users
  for (int i = 0; i < val->size(); ++i) val->operator[](i) = bits[i];
  return dst;
}

}  // namespace serialisation
#endif  // SNAKE_SERIALIZATION_H_
