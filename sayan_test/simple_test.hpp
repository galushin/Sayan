#ifndef Z_SAYAN_SIMPLE_TEST_HPP_INCLUDED
#define Z_SAYAN_SIMPLE_TEST_HPP_INCLUDED

#include <random>
#include <type_traits>
#include <algorithm>
#include <ostream>

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

        std::generate_n(std::inserter(cs, cs.end()), n,
                        &::sayan::test::get_arbitrary<typename Container::value_type>);

        return cs;
    }

    template <class Container>
    Container
    get_arbitrary_container(typename Container::size_type n1, typename Container::size_type n2)
    {
        auto const n = ::sayan::test::random_integral(n1, n2);
        return ::sayan::test::get_arbitrary_container<Container>(n);
    }

    template <class T>
    class move_only
    {
        friend bool operator==(move_only const & x, move_only const & y)
        {
            return x.value() == y.value();
        }

        friend bool operator<(move_only const & x, move_only const & y)
        {
            return x.value() < y.value();
        }

    public:
        explicit move_only(T value)
         : value_(std::move(value))
        {}

        move_only(move_only const &) = delete;
        move_only(move_only &&) = default;

        move_only & operator=(move_only const &) = delete;
        move_only & operator=(move_only &&) = default;

        T const & value() const
        {
            return this->value_;
        }

    private:
        T value_;
    };

    template <class Char, class Traits, class T>
    std::basic_ostream<Char, Traits> &
    operator<<(std::basic_ostream<Char, Traits> & os, move_only<T> const & x)
    {
        os << x.value();
        return os;
    }

    template <class T>
    struct arbitrary<move_only<T>>
    {
        static move_only<T> make()
        {
            return move_only<T>{arbitrary<T>::make()};
        }
    };
}
// namespace test
}
// namespace sayan

#endif
// Z_SAYAN_SIMPLE_TEST_HPP_INCLUDED
