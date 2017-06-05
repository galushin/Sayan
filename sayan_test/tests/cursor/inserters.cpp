#include <sayan/cursor/back_inserter.hpp>

#include <sayan/algorithm.hpp>

#include <catch/catch.hpp>

TEST_CASE("cursors/back_inserter")
{
    std::string const src{"Herb Sutter"};

    std::string dest;
    sayan::copy(src, sayan::back_inserter(dest));

    CHECK(dest == src);
}

TEST_CASE("cursors/back_inserter: rvalue")
{
    std::vector<int> const ns{1,2,3,5,8,13};
    std::vector<std::unique_ptr<int>> src;
    for(auto const & n : ns)
    {
        src.emplace_back(std::make_unique<int>(n));
    }

    std::vector<std::unique_ptr<int>> dest;
    auto cur = sayan::back_inserter(dest);
    for(auto & x : src)
    {
        cur << std::move(x);
    }

    CHECK(std::all_of(src.begin(), src.end(),
                      [](std::unique_ptr<int> const & p) { return !p;}));

    std::vector<int> unptr;
    for(auto const & p : dest)
    {
        unptr.emplace_back(*p);
    }

    CHECK(unptr == ns);
}
