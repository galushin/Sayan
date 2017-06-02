#include <sayan/cursor/sequence_to_cursor.hpp>

#include <catch/catch.hpp>

#include <string>
#include <sstream>
#include <vector>

TEST_CASE("ostream_cursor range-for copy")
{
    std::string const src("Alex Stepanov");
    std::ostringstream os;

    auto out = ::sayan::cursor(os);

    for(auto const & c : src)
    {
        out << c;
    }

    CHECK(src == os.str());
}

TEST_CASE("ostream_cursor range-for copy with delimiter")
{
    std::vector<int> const src = {1, 2, 3, 4, 5};
    std::ostringstream os;

    auto out = ::sayan::ostream_cursor(os, ", ");

    for(auto const & x : src)
    {
        out << x;
    }

    CHECK(os.str() == std::string{"1, 2, 3, 4, 5"});
}

TEST_CASE("ostream_cursor from rvalue range-for copy")
{
    std::string const src("Alex Stepanov");

    auto out = ::sayan::cursor(std::ostringstream{});

    for(auto const & c : src)
    {
        out << c;
    }

    auto os = out.release();

    CHECK(src == os.str());
}

TEST_CASE("is_based_ostream_test")
{
    static_assert(::sayan::is_based_on_ostream<std::ostream>::value, "");
    static_assert(::sayan::is_based_on_ostream<std::ostream&>::value, "");
    static_assert(::sayan::is_based_on_ostream<std::ostringstream>::value, "");

    static_assert(!::sayan::is_based_on_ostream<std::ostream const>::value, "");
    static_assert(!::sayan::is_based_on_ostream<int>::value, "");
    static_assert(!::sayan::is_based_on_ostream<std::vector<int>>::value, "");

    CHECK(::sayan::is_based_on_ostream<std::ostream>::value);
    CHECK(::sayan::is_based_on_ostream<std::ostream&>::value);
    CHECK(::sayan::is_based_on_ostream<std::ostringstream>::value);

    CHECK_FALSE(::sayan::is_based_on_ostream<std::ostream const>::value);
    CHECK_FALSE(::sayan::is_based_on_ostream<int>::value);
    CHECK_FALSE(::sayan::is_based_on_ostream<std::vector<int>>::value);
}
