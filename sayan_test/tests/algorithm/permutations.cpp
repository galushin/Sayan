#include <sayan/algorithm.hpp>

#include <forward_list>
#include <list>
#include <vector>

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
