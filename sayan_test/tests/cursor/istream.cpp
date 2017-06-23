#include <sayan/cursor/sequence_to_cursor.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("istream_as_char_sequence")
{
    std::string const src = "Andrei Alexandrescu";

    std::istringstream is(src);

    auto cur = ::sayan::cursor(is);

    std::string result;

    for(; !cur.empty(); cur.drop(sayan::front))
    {
        result.push_back(cur[sayan::front]);
    }

    CHECK(result == src);

    CHECK(cur.empty());

    CHECK_THROWS(cur[sayan::front]);
    CHECK_THROWS(cur.drop(sayan::front));
}

TEST_CASE("rvalue_istream_as_char_sequence")
{
    std::string const src = "Andrei Alexandrescu";

    auto cur = ::sayan::cursor(std::istringstream(src));

    std::string result;

    for(; !cur.empty(); cur.drop(sayan::front))
    {
        result.push_back(cur[sayan::front]);
    }

    CHECK(result == src);

    CHECK(cur.empty());

    CHECK_THROWS(cur[sayan::front]);
    CHECK_THROWS(cur.drop(sayan::front));
}

TEST_CASE("is_based_on_istream_test")
{
    static_assert(::sayan::is_based_on_istream<std::istream>::value, "");
    static_assert(::sayan::is_based_on_istream<std::istream&>::value, "");
    static_assert(::sayan::is_based_on_istream<std::istringstream>::value, "");
    static_assert(::sayan::is_based_on_istream<std::istringstream&>::value, "");

    static_assert(!::sayan::is_based_on_istream<std::ostream>::value, "");
    static_assert(!::sayan::is_based_on_istream<std::istream const>::value, "");
    static_assert(!::sayan::is_based_on_istream<int>::value, "");
    static_assert(!::sayan::is_based_on_istream<std::vector<int>>::value, "");

    CHECK(::sayan::is_based_on_istream<std::istream>::value);
    CHECK(::sayan::is_based_on_istream<std::istream&>::value);
    CHECK(::sayan::is_based_on_istream<std::istringstream>::value);
    CHECK(::sayan::is_based_on_istream<std::istringstream&>::value);

    CHECK_FALSE(::sayan::is_based_on_istream<std::ostream>::value);
    CHECK_FALSE(::sayan::is_based_on_istream<std::istream const>::value);
    CHECK_FALSE(::sayan::is_based_on_istream<int>::value);
    CHECK_FALSE(::sayan::is_based_on_istream<std::vector<int>>::value);
}

#include <sayan/cursor/istream.hpp>
#include <sayan/cursor/inserters.hpp>
#include <sayan/algorithm.hpp>

TEST_CASE("istream_cursor")
{
    using Input = std::vector<int>;

    auto const test_code = [](Input const & src)
    {
        std::ostringstream os;
        for(auto const & x : src)
        {
            os << x << " ";
        }

        std::istringstream is(os.str());

        std::vector<int> xs;
        ::sayan::copy(::sayan::make_istream_cursor<int>(is), sayan::back_inserter(xs));

        REQUIRE(xs == src);
    };

    Input src;
    test_code(src);

    for(; src.size() < 20; src.push_back(::sayan::test::get_arbitrary<int>()))
    {
        test_code(src);
    }
}

TEST_CASE("istream_cursor: on rvalue")
{
    using Input = std::vector<int>;

    auto const test_code = [](Input const & src)
    {
        std::ostringstream os;
        for(auto const & x : src)
        {
            os << x << " ";
        }

        std::vector<int> xs;
        ::sayan::copy(::sayan::make_istream_cursor<int>(std::istringstream(os.str())),
                      sayan::back_inserter(xs));

        REQUIRE(xs == src);
    };

    Input src;
    test_code(src);

    for(; src.size() < 20; src.push_back(::sayan::test::get_arbitrary<int>()))
    {
        test_code(src);
    }
}
