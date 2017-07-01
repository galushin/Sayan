#include <sayan/cursor/inserters.hpp>

#include <sayan/algorithm.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("cursors/back_inserter")
{
    std::string src;
    for(auto n = 20; n > 0; -- n, src.push_back(sayan::test::get_arbitrary<char>()))
    {
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

TEST_CASE("cursors/front_inserter")
{
    using Input = std::list<int>;
    auto const test_code = [](Input const & src)
    {
        Input dest;
        sayan::copy(src, sayan::front_inserter(dest));
        std::reverse(dest.begin(), dest.end());

        CHECK(dest == src);
    };

    Input src;
    test_code(src);

    for(auto n = 20; n > 0; -- n)
    {
        src.push_back(sayan::test::get_arbitrary<int>());
        test_code(src);
    }
}

TEST_CASE("cursors/front_inserter: rvalue")
{
    std::vector<int> const ns{1,2,3,5,8,13};
    std::vector<std::unique_ptr<int>> src;
    for(auto const & n : ns)
    {
        src.emplace_back(std::make_unique<int>(n));
    }

    std::list<std::unique_ptr<int>> dest;
    auto cur = sayan::front_inserter(dest);
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
    std::reverse(unptr.begin(), unptr.end());

    CHECK(unptr == ns);
}
