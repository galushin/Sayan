#include <sayan/algorithm.hpp>

#include <forward_list>
#include <list>
#include <vector>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("algorithm/is_permutation: default predicate")
{
    std::forward_list<int> const xs_1{1,2,2,3,3,3,4,5};
    std::forward_list<int> const xs_2{1,2,3,4,5,2,3,3};

    auto const r_std = std::is_permutation(xs_1.begin(), xs_1.end(), xs_2.begin(), xs_2.end());
    CHECK(r_std == true);

    auto const r = sayan::is_permutation(xs_1, xs_2);

    CHECK(r == r_std);
}

TEST_CASE("algorithm/is_permutation: true")
{
    std::forward_list<int> const xs_1{1,2,2,3,3,3,4,5};
    std::forward_list<int> const xs_2{1,-2,-3,4,5,2,-3,-3};

    auto const pred = [](int x, int y) { return std::abs(x) == std::abs(y); };

    auto const r_std = std::is_permutation(xs_1.begin(), xs_1.end(),
                                           xs_2.begin(), xs_2.end(), pred);

    CHECK(r_std == true);

    auto const r = sayan::is_permutation(xs_1, xs_2, pred);

    CHECK(r == r_std);
}

TEST_CASE("algorithm/is_permutation: first is shorter")
{
    std::list<int> const xs_1{1,2,2,3,3,3};
    std::vector<int> const xs_2{1,-2,-2,-3,-3,-3,4};

    CHECK(xs_1.size() < xs_2.size());

    auto const pred = [](int x, int y) { return std::abs(x) == std::abs(y); };

    auto const r_std = std::is_permutation(xs_1.begin(), xs_1.end(),
                                           xs_2.begin(), xs_2.end(), pred);

    CHECK(r_std == false);

    auto const r = sayan::is_permutation(xs_1, xs_2, pred);

    CHECK(r == r_std);
}

TEST_CASE("algorithm/is_permutation: second is shorter")
{
    std::vector<int> const xs_1{1,-2,-2,-3,-3,-3,4};
    std::list<int> const xs_2{1,2,2,3,3,3};

    CHECK(xs_2.size() < xs_1.size());

    auto const pred = [](int x, int y) { return std::abs(x) == std::abs(y); };

    auto const r_std = std::is_permutation(xs_1.begin(), xs_1.end(),
                                           xs_2.begin(), xs_2.end(), pred);

    CHECK(r_std == false);

    auto const r = sayan::is_permutation(xs_1, xs_2, pred);

    CHECK(r == r_std);
}

TEST_CASE("algorithm/is_permutation: same length, different element")
{
    std::list<int> const xs_1{1,2,2,3,3,3,4,5};
    std::vector<int> const xs_2{1,-2,-3,5,2,2,-3,-3};

    CHECK(xs_2.size() == xs_1.size());

    auto const pred = [](int x, int y) { return std::abs(x) == std::abs(y); };

    auto const r_std = std::is_permutation(xs_1.begin(), xs_1.end(),
                                           xs_2.begin(), xs_2.end(), pred);

    CHECK(r_std == false);

    auto const r = sayan::is_permutation(xs_1, xs_2, pred);

    CHECK(r == r_std);
}

TEST_CASE("algorithm/next_permutation")
{
    std::list<int> xs;
    for(auto n = 12; n > 0; -- n, xs.push_back(sayan::test::get_arbitrary<int>()))
    {
        CAPTURE(xs);

        auto xs_std = xs;
        auto const r_std = std::next_permutation(xs_std.begin(), xs_std.end());

        auto xs_sayan = xs;
        auto const r_sayan = sayan::next_permutation(xs_sayan);

        REQUIRE(xs_sayan == xs_std);
        REQUIRE(r_sayan == r_std);

        if(r_sayan)
        {
            REQUIRE(::sayan::lexicographical_compare(xs, xs_sayan));
        }
        else
        {
            REQUIRE(::sayan::is_sorted(xs_sayan));
        }
    }
}

TEST_CASE("algorithm/next_permutation: custom compare")
{
    auto const cmp = ::std::greater<>{};

    std::list<int> xs;
    for(auto n = 12; n > 0; -- n, xs.push_back(sayan::test::get_arbitrary<int>()))
    {
        CAPTURE(xs);

        auto xs_std = xs;
        auto const r_std = std::next_permutation(xs_std.begin(), xs_std.end(), cmp);

        auto xs_sayan = xs;
        auto const r_sayan = sayan::next_permutation(xs_sayan, cmp);

        REQUIRE(xs_sayan == xs_std);
        REQUIRE(r_sayan == r_std);

        if(r_sayan)
        {
            REQUIRE(::sayan::lexicographical_compare(xs, xs_sayan, cmp));
        }
        else
        {
            REQUIRE(::sayan::is_sorted(xs_sayan, cmp));
        }
    }
}

TEST_CASE("algorithm/prev_permutation")
{
    std::list<int> xs;
    for(auto n = 12; n > 0; -- n, xs.push_back(sayan::test::get_arbitrary<int>()))
    {
        CAPTURE(xs);

        auto xs_std = xs;
        auto const r_std = std::prev_permutation(xs_std.begin(), xs_std.end());

        auto xs_sayan = xs;
        auto const r_sayan = sayan::prev_permutation(xs_sayan);

        REQUIRE(xs_sayan == xs_std);
        REQUIRE(r_sayan == r_std);

        if(r_sayan)
        {
            REQUIRE(::sayan::lexicographical_compare(xs_sayan, xs));
        }
        else
        {
            REQUIRE(::sayan::is_sorted(sayan::make_reverse_cursor(xs_sayan)));
        }
    }
}

TEST_CASE("algorithm/prev_permutation: custom compare")
{
    auto const cmp = ::std::greater<>{};

    std::list<int> xs;
    for(auto n = 12; n > 0; -- n, xs.push_back(sayan::test::get_arbitrary<int>()))
    {
        CAPTURE(xs);

        auto xs_std = xs;
        auto const r_std = std::prev_permutation(xs_std.begin(), xs_std.end(), cmp);

        auto xs_sayan = xs;
        auto const r_sayan = sayan::prev_permutation(xs_sayan, cmp);

        REQUIRE(xs_sayan == xs_std);
        REQUIRE(r_sayan == r_std);

        if(r_sayan)
        {
            REQUIRE(::sayan::lexicographical_compare(xs_sayan, xs, cmp));
        }
        else
        {
            REQUIRE(::sayan::is_sorted(sayan::make_reverse_cursor(xs_sayan), cmp));
        }
    }
}
