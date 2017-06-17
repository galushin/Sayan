#include <sayan/cursor/back_inserter.hpp>

#include <sayan/algorithm.hpp>

#include <catch/catch.hpp>

#include "../../simple_test.hpp"

TEST_CASE("cursors/back_inserter")
{
    std::string src;
    for(auto n = 20; n > 0; -- n, src.push_back(sayan::test::get_arbitrary<char>()))
    {
        INFO("src = " << src);

        std::string dest;
        sayan::copy(src, sayan::back_inserter(dest));

        CHECK(dest == src);
    }
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
