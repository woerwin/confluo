#ifndef DIALOG_RECORD_H_
#define DIALOG_RECORD_H_

#include <vector>
#include <cstdint>

#include "ptr.h"

namespace dialog {

struct record_t {
 public:
  typedef std::vector<field_t>::iterator iterator;
  typedef std::vector<field_t>::const_iterator const_iterator;

  record_t()
      : timestamp_(0),
        log_offset_(0),
        ro_data_ptr_(),
        data_(nullptr),
        size_(0),
        version_(0) {
  }

  /**
   * Constructor using a read-only pointer to guarantee the data's lifetime
   * @param log_offset log offset
   * @param data read-only pointer to data
   * @param size size of data
   */
  record_t(size_t log_offset, storage::read_only_ptr<uint8_t> data, size_t size)
      : timestamp_(*reinterpret_cast<int64_t*>(data.get())),
        log_offset_(log_offset),
        ro_data_ptr_(data),
        data_(data.get()),
        size_(size),
        version_(log_offset + size) {
  }

  /**
   * Constructor using a raw pointer
   * @param log_offset log offset
   * @param data raw pointer to data
   * @param size size of data
   */
  record_t(size_t log_offset, uint8_t* data, size_t size)
      : timestamp_(*reinterpret_cast<int64_t*>(data)),
        log_offset_(log_offset),
        ro_data_ptr_(),
        data_(data),
        size_(size),
        version_(log_offset + size) {
  }

  void reserve(size_t n) {
    fields_.reserve(n);
  }

  void push_back(const field_t& val) {
    fields_.push_back(val);
  }

  void push_back(field_t&& val) {
    fields_.push_back(val);
  }

  const field_t& operator[](uint16_t idx) const {
    return fields_.at(idx);
  }

  const field_t& at(uint16_t idx) const {
    return fields_.at(idx);
  }

  uint64_t timestamp() const {
    return timestamp_;
  }

  size_t log_offset() const {
    return log_offset_;
  }

  uint8_t* data() const {
    return data_;
  }

  uint64_t version() const {
    return version_;
  }

  iterator begin() {
    return fields_.begin();
  }

  iterator end() {
    return fields_.end();
  }

  const_iterator begin() const {
    return fields_.begin();
  }

  const_iterator end() const {
    return fields_.end();
  }

  size_t length() const {
    return size_;
  }

  std::string to_string() const {
    std::string str = "(";
    for (auto& f: *this) {
      str += f.to_string() + ", ";
    }
    str.pop_back();
    str += ")";
    return str;
  }

 private:
  int64_t timestamp_;
  size_t log_offset_;
  storage::read_only_ptr<uint8_t> ro_data_ptr_;
  uint8_t* data_;
  size_t size_;
  uint64_t version_;
  std::vector<field_t> fields_;
};

}

#endif /* DIALOG_RECORD_H_ */
