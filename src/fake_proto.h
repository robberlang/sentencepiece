#pragma once

#include <string>
#include <vector>

namespace fake_proto {

template <class T>
class Vector {
private:
  std::vector<T> data_;

public:
  T* Add() { data_.emplace_back(); return &data_.back(); }
  T* Mutable(int index) { return &data_[index]; }
  void SwapElements(int i, int j) { std::swap(data_[i], data_[j]); }

  size_t size() const { return data_.size(); }
  void clear() { data_.clear(); }

  typename std::vector<T>::iterator begin() { return data_.begin(); }
  typename std::vector<T>::iterator end()   { return data_.end(); }

  const typename std::vector<T>::const_iterator begin() const { return data_.cbegin(); }
  const typename std::vector<T>::const_iterator end() const   { return data_.cend(); }
};

#define PB_REPEAT(type, variable) \
private: \
  ::fake_proto::Vector<type> variable ## _; \
public: \
  const ::fake_proto::Vector<type>& variable() const { return variable ## _; } \
  type* add_ ## variable() { return variable ## _.Add(); } \
  type* mutable_ ## variable(int index) { return variable ## _.Mutable(index); } \
  ::fake_proto::Vector<type>* mutable_ ## variable() { return & variable ## _ ; } \
  int variable ## _size() const { return variable ## _.size(); } \

#define PB_STRING(variable) \
private: \
  std::string variable ## _; \
public: \
  const std::string& variable() const { return variable ## _; } \
  void set_ ## variable(const std::string& value) { variable ## _ = value; } \
  void set_ ## variable(std::string&& value) { variable ## _ = value; } \
  void set_ ## variable(const char* value) { variable ## _ = value; } \
  void set_ ## variable(const char* value, size_t size) { variable ## _ = std::string(value, size); }; \
  std::string* mutable_ ## variable() { return & variable ## _ ; }

#define PB_UINT32(variable) \
private: \
  uint32_t variable ## _; \
public: \
  const uint32_t& variable() const { return variable ## _; } \
  void set_ ## variable(uint32_t value) { variable ## _ = value; }

#define PB_FLOAT(variable) \
private: \
  float variable ## _; \
public: \
  const float& variable() const { return variable ## _; } \
  void set_ ## variable(float value) { variable ## _ = value; }


}