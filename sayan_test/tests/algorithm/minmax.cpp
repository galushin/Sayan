#include <sayan/algorithm.hpp>

#include <forward_list>

#include <catch/catch.hpp>

TEST_CASE("algorithm/min_element")
{
    std::forward_list<int> const xs{3, 1, 4, 1, 5, 9, 2, 6};

    auto const r_std = std::min_element(xs.begin(), xs.end());
    auto const r = sayan::min_element(xs);

    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());

    // @todo Проверка пройденной части последовательности
}

TEST_CASE("algorithm/min_element: custom predicate")
{
    std::forward_list<int> const xs{3, 1, 4, 1, 5, 9, 2, 6, 9};
    auto const cmp = std::greater<>{};

    auto const r_std = std::min_element(xs.begin(), xs.end(), cmp);
    auto const r = sayan::min_element(xs, cmp);

    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());

    // @todo Проверка пройденной части последовательности
}

TEST_CASE("algorithm/max_element")
{
    std::forward_list<int> const xs{3, 1, 4, 1, 5, 9, 2, 6};

    auto const r_std = std::max_element(xs.begin(), xs.end());
    auto const r = sayan::max_element(xs);

    CHECK(sayan::count(r, *r) == 1);

    CHECK(*r.begin() == *r_std);
    CHECK(r.end() == xs.end());

    // @todo Проверка пройденной части последовательности
}

TEST_CASE("algorithm/max_element: custom predicate")
{
    std::forward_list<int> const xs{3, 1, 4, 1, 5, 9, 2, 6, 9};
    auto const cmp = std::greater<>{};

    auto const r_std = std::max_element(xs.begin(), xs.end(), cmp);
    auto const r = sayan::max_element(xs, cmp);

    CHECK(sayan::count(r, *r) == 1);

    CHECK(*r.begin() == *r_std);
    CHECK(r.end() == xs.end());

    // @todo Проверка пройденной части последовательности
}

TEST_CASE("algorithm/minmax_element")
{
    std::forward_list<int> const xs{3, 1, 4, 1, 5, 9, 2, 6};

    auto const r_min = sayan::min_element(xs);
    auto const r_max = sayan::max_element(xs);
    auto const r = sayan::minmax_element(xs);

    CHECK(r.first.begin() == r_min.begin());
    CHECK(r.first.end() == r_min.end());
    CHECK(r.second.begin() == r_max.begin());
    CHECK(r.second.end() == r_max.end());

    // @todo Проверка пройденной части последовательности
}

TEST_CASE("algorithm/minmax_element: custom predicate")
{
    std::forward_list<int> const xs{3, 1, 4, 1, 5, 9, 2, 6, 9};
    auto const cmp = std::greater<>{};

    auto const r_min = sayan::min_element(xs, cmp);
    auto const r_max = sayan::max_element(xs, cmp);
    auto const r = sayan::minmax_element(xs, cmp);

    CHECK(r.first.begin() == r_min.begin());
    CHECK(r.first.end() == r_min.end());
    CHECK(r.second.begin() == r_max.begin());
    CHECK(r.second.end() == r_max.end());

    // @todo Проверка пройденной части последовательности
}
