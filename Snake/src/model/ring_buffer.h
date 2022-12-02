#ifndef SNAKE_SNAKE_GAME_RING_BUFFER_H_
#define SNAKE_SNAKE_GAME_RING_BUFFER_H_
#include <queue>
#include <algorithm>

namespace snake {
template <typename T>
struct RingBuffer {
 public:
  RingBuffer(const int& capacity)
      : buffer_(std::vector<T>()), capacity_(capacity), cursor_(-1) {}

  void Add(T&& arg) {
    cursor_ = (cursor_ + 1) % capacity_;
    if (buffer_.size() == capacity_) {
      std::swap(buffer_[cursor_], arg);
    } else {
      buffer_.emplace_back(std::move(arg));
    }
  };

  int Size() & { return buffer_.size(); };
  int getCapacity() { return capacity_; };

  T& operator[](const int i) & {
    int offset = (cursor_ - i) % capacity_;
    return buffer_[offset < 0 ? capacity_ + offset : offset];
  }

 private:
  std::vector<T> buffer_;
  int capacity_;
  int cursor_;
};
}  // namespace snake
#endif  // SNAKE_SNAKE_GAME_RING_BUFFER_H_
