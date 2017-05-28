#include <sayan/cursor/ostream.hpp>

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

    auto out = ::sayan::cursor(os, ", ");

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

