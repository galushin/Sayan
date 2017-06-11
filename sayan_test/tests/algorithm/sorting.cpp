#include <sayan/algorithm.hpp>

#include <catch/catch.hpp>

#include <forward_list>

TEST_CASE("algorithm/is_sorted")
{
    std::forward_list<int> const xs{1,2,3,3,5};

    auto const r_std = std::is_sorted(xs.begin(), xs.end());

    auto const r = ::sayan::is_sorted(xs);

    CHECK(r == r_std);
}

TEST_CASE("algorithm/is_sorted: custom compare")
{
    std::forward_list<int> const xs{1,2,3,3,5};
    auto const cmp = std::greater<>{};

    auto const r_std = std::is_sorted(xs.begin(), xs.end(), cmp);

    auto const r = ::sayan::is_sorted(xs, cmp);

    CHECK(r == r_std);
}

TEST_CASE("algorithm/is_sorted_until")
{
    std::forward_list<int> const xs{1,2,3,5,4};

    auto const r_std = std::is_sorted_until(xs.begin(), xs.end());

    auto const r = ::sayan::is_sorted_until(xs);

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/is_sorted_until: custom compare, success")
{
    std::forward_list<int> const xs{5,4,5,1,4};
    auto const cmp = std::greater<>{};

    auto const r_std = std::is_sorted_until(xs.begin(), xs.end(), cmp);

    auto const r = ::sayan::is_sorted_until(xs, cmp);

    CHECK(r_std != xs.end());
    CHECK(!!r);

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/is_sorted_until: custom compare, fail")
{
    std::forward_list<int> const xs{5,4,3,2,1};
    auto const cmp = std::greater<>{};

    auto const r_std = std::is_sorted_until(xs.begin(), xs.end(), cmp);

    auto const r = ::sayan::is_sorted_until(xs, cmp);

    CHECK(r_std == xs.end());
    CHECK(!r);

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(r.begin() == r_std);
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/lower_bound")
{
    std::forward_list<int> const xs{2, 3, 3, 4, 4, 4, 6, 7};
    CHECK(sayan::is_sorted(xs));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::lower_bound(xs.begin(), xs.end(), value);
        auto const r_sayan = sayan::lower_bound(xs, value);

        CHECK(r_sayan.traversed_begin() == xs.begin());
        CHECK(r_sayan.begin() == r_std);
        CHECK(r_sayan.end() == xs.end());
    }
}

TEST_CASE("algorithm/lower_bound: custom predicate")
{
    std::vector<int> const xs{7, 6, 4, 4, 4, 3, 3, 2};
    auto const pred = std::greater<>{};

    CHECK(sayan::is_sorted(xs, pred));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::lower_bound(xs.begin(), xs.end(), value, pred);
        auto const r_sayan = sayan::lower_bound(xs, value, pred);

        CHECK(r_sayan.traversed_begin() == xs.begin());
        CHECK(r_sayan.begin() == r_std);
        CHECK(r_sayan.end() == xs.end());
    }
}

TEST_CASE("algorithm/upper_bound")
{
    std::forward_list<int> const xs{2, 3, 3, 4, 4, 4, 6, 7};
    CHECK(sayan::is_sorted(xs));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::upper_bound(xs.begin(), xs.end(), value);
        auto const r_sayan = sayan::upper_bound(xs, value);

        CHECK(r_sayan.traversed_begin() == xs.begin());
        CHECK(r_sayan.begin() == r_std);
        CHECK(r_sayan.end() == xs.end());
    }
}

TEST_CASE("algorithm/upper_bound: custom predicate")
{
    std::vector<int> const xs{7, 6, 4, 4, 4, 3, 3, 2};
    auto const pred = std::greater<>{};

    CHECK(sayan::is_sorted(xs, pred));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::upper_bound(xs.begin(), xs.end(), value, pred);
        auto const r_sayan = sayan::upper_bound(xs, value, pred);

        CHECK(r_sayan.traversed_begin() == xs.begin());
        CHECK(r_sayan.begin() == r_std);
        CHECK(r_sayan.end() == xs.end());
    }
}

TEST_CASE("algorithm/binary_search")
{
    std::forward_list<int> const xs{2, 3, 3, 4, 4, 4, 6, 7};
    CHECK(sayan::is_sorted(xs));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::binary_search(xs.begin(), xs.end(), value);
        auto const r_sayan = sayan::binary_search(xs, value);

        CHECK(r_sayan == r_std);
    }
}

TEST_CASE("algorithm/binary_search: custom predicate")
{
    std::vector<int> const xs{7, 6, 4, 4, 4, 3, 3, 2};
    auto const pred = std::greater<>{};

    CHECK(sayan::is_sorted(xs, pred));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::binary_search(xs.begin(), xs.end(), value, pred);
        auto const r_sayan = sayan::binary_search(xs, value, pred);

        CHECK(r_sayan == r_std);
    }
}

TEST_CASE("algorithm/equal_range")
{
    std::forward_list<int> const xs{2, 3, 3, 4, 4, 4, 6, 7};
    CHECK(sayan::is_sorted(xs));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::equal_range(xs.begin(), xs.end(), value);
        auto const r_sayan = sayan::equal_range(xs, value);

        CHECK(r_sayan.traversed_begin() == xs.begin());
        CHECK(r_sayan.begin() == r_std.first);
        CHECK(r_sayan.end() == r_std.second);
    }
}

TEST_CASE("algorithm/equal_range: custom predicate")
{
    std::vector<int> const xs{7, 6, 4, 4, 4, 3, 3, 2};
    auto const pred = std::greater<>{};

    CHECK(sayan::is_sorted(xs, pred));

    std::forward_list<int> const to_find{1, 4, 5, 8};

    for(auto const & value : to_find)
    {
        auto const r_std = std::equal_range(xs.begin(), xs.end(), value, pred);
        auto const r_sayan = sayan::equal_range(xs, value, pred);

        CHECK(r_sayan.traversed_begin() == xs.begin());
        CHECK(r_sayan.begin() == r_std.first);
        CHECK(r_sayan.end() == r_std.second);
    }
}
