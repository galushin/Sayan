#include <sayan/algorithm.hpp>

#include <catch/catch.hpp>

#include <forward_list>

TEST_CASE("algorithm/is_sorted")
{
    std::forward_list<int> const xs{1,2,3,3,5};

    auto const r_std = std::is_sorted(xs.begin(), xs.end());

    auto const r = ::sayan::is_sorted(xs);

    CHECK(r == r_std);

    // @todo Проверить пройденную часть последовательности
}

TEST_CASE("algorithm/is_sorted: custom compare")
{
    std::forward_list<int> const xs{1,2,3,3,5};
    auto const cmp = std::greater<>{};

    auto const r_std = std::is_sorted(xs.begin(), xs.end(), cmp);

    auto const r = ::sayan::is_sorted(xs, cmp);

    CHECK(r == r_std);

    // @todo Проверить пройденную часть последовательности
}

TEST_CASE("algorithm/is_sorted_until")
{
    std::forward_list<int> const xs{1,2,3,5,4};

    auto const r_std = std::is_sorted_until(xs.begin(), xs.end());

    auto const r = ::sayan::is_sorted_until(xs);

    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());

    // @todo Проверить пройденную часть последовательности
}

TEST_CASE("algorithm/is_sorted_until: custom compare, success")
{
    std::forward_list<int> const xs{5,4,5,1,4};
    auto const cmp = std::greater<>{};

    auto const r_std = std::is_sorted_until(xs.begin(), xs.end(), cmp);

    auto const r = ::sayan::is_sorted_until(xs, cmp);

    CHECK(r_std != xs.end());
    CHECK(!!r);

    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());

    // @todo Проверить пройденную часть последовательности
}

TEST_CASE("algorithm/is_sorted_until: custom compare, fail")
{
    std::forward_list<int> const xs{5,4,3,2,1};
    auto const cmp = std::greater<>{};

    auto const r_std = std::is_sorted_until(xs.begin(), xs.end(), cmp);

    auto const r = ::sayan::is_sorted_until(xs, cmp);

    CHECK(r_std == xs.end());
    CHECK(!r);

    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());

    // @todo Проверить пройденную часть последовательности
}
