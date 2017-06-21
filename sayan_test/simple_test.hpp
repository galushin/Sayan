#ifndef Z_SAYAN_SIMPLE_TEST_HPP_INCLUDED
#define Z_SAYAN_SIMPLE_TEST_HPP_INCLUDED

#include <random>
#include <type_traits>

namespace sayan
{
namespace test
{
    using random_engine_type = std::mt19937;

    random_engine_type & random_engine();

    template <class T>
    T random_integral(T first, T last)
    {
        std::uniform_int_distribution<T> d(first, last);
        return d(::sayan::test::random_engine());
    }

    template <class T, class AlwaysVoid = void>
    struct arbitrary;

    template <class T>
    struct arbitrary<T, std::enable_if_t<std::is_integral<T>::value>>
    {
        static T make()
        {
            using L = std::numeric_limits<T>;
            return ::sayan::test::random_integral<T>(L::min(), L::max());
        }
    };

    template <class T>
    T get_arbitrary()
    {
        return arbitrary<T>::make();
    }

    template <class Container>
    Container get_arbitrary_container(typename Container::size_type n)
    {
        Container cs;

        for(; n > 0; -- n)
        {
            cs.push_back(::sayan::test::get_arbitrary<typename Container::value_type>());
        }

        return cs;
    }
}
// namespace test
}
// namespace sayan

#endif
// Z_SAYAN_SIMPLE_TEST_HPP_INCLUDED
