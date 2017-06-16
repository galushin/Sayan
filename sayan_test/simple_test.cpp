#include "simple_test.hpp"

#include <ctime>

namespace sayan
{
namespace test
{
    random_engine_type & random_engine()
    {
        // @todo Инициализация
        // @todo Учесть многопточность
        static random_engine_type rnd(std::time(0));

        return rnd;
    }
}
// namespace test
}
// namespace sayan
