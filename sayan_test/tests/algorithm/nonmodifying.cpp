#include <sayan/algorithm.hpp>

#include <sayan/cursor/istream.hpp>

#include <catch/catch.hpp>

#include <algorithm>

TEST_CASE("algorithms/nonmodifying/quantors: all_of, any_of, some_of")
{
    std::vector<std::string> const strs{"", "generic_programming",
                                        "Alex Stepanov", "STL"};

    auto const pred = [](char c) { return std::isupper(c); };

    for(auto const & s : strs)
    {
        CHECK(::sayan::all_of(sayan::cursor(std::istringstream(s)), pred)
              == std::all_of(s.begin(), s.end(), pred));
        CHECK(::sayan::any_of(sayan::cursor(std::istringstream(s)), pred)
              == std::any_of(s.begin(), s.end(), pred));
        CHECK(::sayan::none_of(sayan::cursor(std::istringstream(s)), pred)
              == std::none_of(s.begin(), s.end(), pred));
    }
}

TEST_CASE("algorithms/nonmodifying/for_each")
{
    std::string const src{"Alex Stepanov"};

    std::string out;

    auto cur = sayan::for_each(::sayan::cursor(std::istringstream(src)),
                               [&out](char c){ out.push_back(c);});

    CHECK(out == src);
    CHECK(cur.empty());
}

TEST_CASE("algorithms/nonmodifying/find: success")
{
    std::string const src{"Alex Stepanov"};

    auto const value = 'e';

    auto const r_std = std::find(src.begin(), src.end(), value);

    CHECK((r_std != src.end()));

    std::istringstream is(src);

    auto const r = sayan::find(sayan::cursor(is), value);

    CHECK(!r.empty());

    std::string rest;
    sayan::for_each(r, [&rest](char c){ rest.push_back(c);});

    CHECK(rest == std::string(r_std, src.end()));
}

TEST_CASE("algorithms/nonmodifying/find: fail")
{
    std::string const src{"Alex Stepanov"};

    auto const value = 'z';

    auto const r_std = std::find(src.begin(), src.end(), value);

    CHECK((r_std == src.end()));

    std::istringstream is(src);

    auto const r = sayan::find(sayan::cursor(is), value);

    CHECK(r.empty());
}

TEST_CASE("algorithms/nonmodifying/find_if_not: success")
{
    std::string const src{"Alex Stepanov"};

    auto const pred = [](char c) { return std::isupper(c); };

    auto const r_std = std::find_if_not(src.begin(), src.end(), pred);

    CHECK((r_std != src.end()));

    std::istringstream is(src);

    auto const r = sayan::find_if_not(sayan::cursor(is), pred);

    CHECK(!r.empty());

    std::string rest;
    sayan::for_each(r, [&rest](char c){ rest.push_back(c);});

    CHECK(rest == std::string(r_std, src.end()));
}

TEST_CASE("algorithms/nonmodifying/find_if_not: fail")
{
    std::string const src{"STL"};

    auto const pred = [](char c) { return std::isupper(c); };

    auto const r_std = std::find_if_not(src.begin(), src.end(), pred);

    CHECK((r_std == src.end()));

    std::istringstream is(src);

    auto const r = sayan::find_if_not(sayan::cursor(is), pred);

    CHECK(r.empty());
}

TEST_CASE("algorithms/nonmodifying/count")
{
    std::vector<std::string> const strs{"", "generic_programming",
                                        "Alex Stepanov", "STL"};

    auto const value = 'e';

    for(auto const & s : strs)
    {
        std::istringstream is(s);
        CHECK(::sayan::count(sayan::cursor(is), value)
              == std::count(s.begin(), s.end(), value));
    }
}

TEST_CASE("algorithms/nonmodifying/count_if")
{
    std::vector<std::string> const strs{"", "generic_programming",
                                        "Alex Stepanov", "STL"};

    auto const pred = [](char c) { return std::isupper(c); };

    for(auto const & s : strs)
    {
        std::istringstream is(s);
        CHECK(::sayan::count_if(sayan::cursor(is), pred)
              == std::count_if(s.begin(), s.end(), pred));
    }
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
