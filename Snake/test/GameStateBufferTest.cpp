#define BOOST_TEST_MODULE SolutionTest

#include "../src/service/GameModels.h"
#include <boost/test/included/unit_test.hpp>

namespace {
struct Dummy {

    Dummy(int id) : id_(id) { std::cout << "direct constructor " << std::to_string(id_) << " : " << this << std::endl; }
    Dummy(Dummy const& src) : id_(src.id_) { std::cout << "copy constructor " << std::to_string(id_) << " : " << this << std::endl; }
    Dummy(Dummy&&) noexcept { std::cout << "move constructor " << this << std::endl; }
    ~Dummy() noexcept { std::cout << "destructor " << std::to_string(id_) << " : " << this << std::endl; }

    int getId() const { return id_; };

private:
    int id_;
};
}

BOOST_AUTO_TEST_CASE(testGameStateBufferOneElem) {
    using namespace snake;
    // arrange
    GameStateBuffer<Dummy> data(3);
    data.add(new Dummy(42));

    // act
    // arrange
    BOOST_CHECK_EQUAL(1, data.getSize());
    BOOST_CHECK_EQUAL(42, data[0].getId());
}

BOOST_AUTO_TEST_CASE(testGameStateBufferSize) {
    using namespace snake;
    // arrange
    GameStateBuffer<Dummy> data(3);

    // act
    // arrange
    int size = data.getSize();
    BOOST_CHECK_EQUAL(0, data.getSize());
}

BOOST_AUTO_TEST_CASE(testGameStateBufferErasion) {
    using namespace snake;
    // arrange
    int bufferSize = 3;
    int elemCount = 6;

    GameStateBuffer<Dummy> data(bufferSize);

    for (size_t i = 0; i < elemCount; i++) {
        data.add(new Dummy(i));
    }

    // act
    // arrange
    BOOST_CHECK_EQUAL(3, data.getSize());
    BOOST_CHECK_EQUAL(3, data[0].getId());
    BOOST_CHECK_EQUAL(4, data[1].getId());
    BOOST_CHECK_EQUAL(5, data[2].getId());
}
