#define BOOST_TEST_MODULE SolutionTest

#include <boost/test/included/unit_test.hpp>

#include "../src/service/game_models.h"

namespace {
struct Controllable {
  // static int control;
  int *resource_;
  Controllable() : resource_(new int(42)) { std::cout << "ctor\n"; }

  Controllable(Controllable &&rhs) noexcept : resource_(rhs.resource_) {
    std::cout << "move ctor\n";
    rhs.resource_ = nullptr;
  }
  Controllable &operator=(Controllable &&rhs) noexcept {
    std::swap(resource_, rhs.resource_);
    std::cout << "move assign\n";
    return *this;
  }
  Controllable(const Controllable &rhs) : resource_(new int(*rhs.resource_)) {
    std::cout << "copy ctor\n";
#if 0
    if (control == 0) {
      control = 5;
      std::cout << "Control reached\n";
      throw std::bad_alloc{};
    }
    control -= 1;
#endif
  }
  Controllable &operator=(const Controllable &rhs) {
    Controllable tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

  ~Controllable() {
    std::cout << "dtor \n";
    delete resource_;
  }
};

struct Dummy {
  Dummy(int id) : id_(id) {
    std::cout << "direct ctor " << std::to_string(id_) << " : " << this
              << std::endl;
  }
  Dummy(Dummy &src) : id_(src.id_) {
    std::cout << "copy ctor " << std::to_string(id_) << " : " << this
              << std::endl;
  }
  Dummy(Dummy &&) noexcept { std::cout << "move ctor " << this << std::endl; }
  ~Dummy() noexcept { std::cout << "dtor " << std::endl; }

  int getId() const { return id_; };

 private:
  int id_;
};

template <typename T>
class GameBufferAllocator {
 public:
  T* arr_;
  int size_ = 0, capacity_;

  GameBufferAllocator(int capacity = 0)
      : capacity_(capacity),
        arr_(capacity == 0
                 ? nullptr
                 : static_cast<T*>(::operator new(sizeof(T) * capacity))) {}

  GameBufferAllocator(GameBufferAllocator& rhs) = delete;
  GameBufferAllocator(GameBufferAllocator&& rhs) = delete;
  GameBufferAllocator(const GameBufferAllocator& rhs) = delete;
  GameBufferAllocator(const GameBufferAllocator&& rhs) = delete;

  GameBufferAllocator operator=(GameBufferAllocator&& rhs) = delete;
  GameBufferAllocator operator=(const GameBufferAllocator&& rhs) = delete;

  ~GameBufferAllocator() {
    int used = size_ > capacity_ ? capacity_ : size_;
    while (--used >= 0) {
      (arr_ + used)->~T();
    }
    ::operator delete(arr_);
  }
};

template <typename T>
class GameStateBuffer final : private GameBufferAllocator<T> {
 private:
  using GameBufferAllocator::arr_;
  using GameBufferAllocator::capacity_;
  using GameBufferAllocator::size_;

 public:
  GameStateBuffer(const int& capacity = 0) : GameBufferAllocator(capacity){};

  void add(const T& item) {
    T* p = arr_ + size_ % capacity_;
    if (size_ >= capacity_) {
      (p)->~T();
    }
    new (p) T(item);
    ++size_;
  };

  const T& head() const noexcept { return *(arr_[size_ % capacity_]); }

  const T& operator[](const int& index) const {
    return arr_[index % capacity_];
  }

  int getSize() const noexcept { return size_; }
};

}  // namespace

BOOST_AUTO_TEST_CASE(sampleTest) {
  // arrange
  GameStateBuffer<Controllable> buf(3);

  for (int i = 0; i < 10; ++i) {
    buf.add(Controllable());
  }

  for (int i = 0; i < 3; ++i) {
    auto it = buf[i];
    std::cout << *(it.resource_) << std::endl;
  }
}
