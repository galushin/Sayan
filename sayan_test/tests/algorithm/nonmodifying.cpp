#include <sayan/algorithm.hpp>

#include <sayan/cursor/istream.hpp>

#include <catch/catch.hpp>

#include <algorithm>

TEST_CASE("algorithms/nonmodifying/for_each")
{
    std::string const src{"Alex Stepanov"};

    std::string out;

    auto cur = sayan::for_each(::sayan::cursor(std::istringstream(src)),
                               [&out](char c){ out.push_back(c);});

    CHECK(out == src);
    CHECK(cur.empty());
}

TEST_CASE("algorithms/nonmodifying/equal")
{
    std::vector<std::string> const strs
        = {"Art", "Artist", "Burden", "art", "artist", "buRDen"};

    for(auto const & x : strs)
    for(auto const & y : strs)
    {
        auto const r_std = std::equal(x.begin(), x.end(),
                                      y.begin(), y.end());

        auto const r_sayan
            = ::sayan::equal(sayan::cursor(std::istringstream(x)),
                             sayan::cursor(std::istringstream(y)));

        CHECK(r_std == r_sayan);
    }
}

TEST_CASE("algorithms/nonmodifying/equal with predicate")
{
    std::vector<std::string> const strs
        = {"Art", "Artist", "Burden", "art", "artist", "buRDen"};

    auto eq = [](char x, char y)
    {
        return std::tolower(x) == std::tolower(y);
    };

    for(auto const & x : strs)
    for(auto const & y : strs)
    {
        auto const r_std = std::equal(x.begin(), x.end(),
                                      y.begin(), y.end(), eq);

        auto const r_sayan
            = ::sayan::equal(sayan::cursor(std::istringstream(x)),
                             sayan::cursor(std::istringstream(y)), eq);

        CHECK(r_std == r_sayan);
    }
}
