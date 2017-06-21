#include <sayan/algorithm.hpp>

#include <algorithm>

#include <catch/catch.hpp>
#include "../../simple_test.hpp"

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

TEST_CASE("algorithm/inplace_merge")
{
    std::vector<int> xs;

    for(auto n = 12; n > 0; -- n, xs.push_back(sayan::test::get_arbitrary<short>()))
    {
        auto const m = ::sayan::test::random_integral(0*xs.size(), xs.size());

        // Готовим данные
        auto xs_temp = xs;
        {
            auto const i = ::std::next(xs_temp.begin(), m);
            ::std::sort(xs_temp.begin(), i);
            ::std::sort(i, xs_temp.end());
        }
        CAPTURE(xs_temp);

        std::vector<int> xs_std(xs_temp.begin(), xs_temp.end());
        std::vector<int> xs_sayan = xs_std;

        // Выполняем
        ::std::inplace_merge(xs_std.begin(), ::std::next(xs_std.begin(), m), xs_std.end());

        ::sayan::inplace_merge(::sayan::next(::sayan::cursor(xs_sayan), m));

        // Проверяем
        REQUIRE(xs_sayan == xs_std);
    }
}

TEST_CASE("algorithm/inplace_merge: custom compare")
{
    std::vector<int> xs;
    auto const cmp = ::std::greater<>{};

    for(auto n = 12; n > 0; -- n, xs.push_back(sayan::test::get_arbitrary<short>()))
    {
        auto const m = ::sayan::test::random_integral(0*xs.size(), xs.size());

        // Готовим данные
        auto xs_temp = xs;
        {
            auto const i = ::std::next(xs_temp.begin(), m);
            ::std::sort(xs_temp.begin(), i, cmp);
            ::std::sort(i, xs_temp.end(), cmp);
        }
        CAPTURE(xs_temp);

        std::list<int> xs_std(xs_temp.begin(), xs_temp.end());
        std::list<int> xs_sayan = xs_std;

        // Выполняем
        ::std::inplace_merge(xs_std.begin(), ::std::next(xs_std.begin(), m), xs_std.end(), cmp);

        ::sayan::inplace_merge(::sayan::next(::sayan::cursor(xs_sayan), m), cmp);

        // Проверяем
        REQUIRE(xs_sayan == xs_std);
    }
}

TEST_CASE("algorithm/is_heap_until")
{
    std::vector<int> xs;
    for(auto n = 12; n > 0; -- n)
    {
        CAPTURE(xs);

        auto const r_std = std::is_heap_until(xs.begin(), xs.end());

        auto const r = ::sayan::is_heap_until(xs);

        REQUIRE(r.traversed_begin() == xs.begin());
        REQUIRE((r.begin() - xs.begin()) == (r_std - xs.begin()));
        REQUIRE(r.end() == xs.end());
        REQUIRE(r.traversed_end() == xs.end());

        xs.push_back(sayan::test::get_arbitrary<int>());
    }
}

TEST_CASE("algorithm/is_heap_until: custom predicate")
{
    std::vector<int> xs;
    auto const cmp = ::std::greater<>{};

    for(auto n = 12; n > 0; -- n)
    {
        CAPTURE(xs);

        auto const r_std = std::is_heap_until(xs.begin(), xs.end(), cmp);

        auto const r = ::sayan::is_heap_until(xs, cmp);

        REQUIRE(r.traversed_begin() == xs.begin());
        REQUIRE(r.begin() == r_std);
        REQUIRE(r.end() == xs.end());
        REQUIRE(r.traversed_end() == xs.end());

        xs.push_back(sayan::test::get_arbitrary<int>());
    }
}

TEST_CASE("algorithm/is_heap_until: custom predicate, on heap")
{
    std::vector<int> xs;
    auto const cmp = ::std::greater<>{};

    for(auto n = 12; n > 0; -- n)
    {
        CAPTURE(xs);

        auto const r_std = std::is_heap_until(xs.begin(), xs.end(), cmp);

        auto const r = ::sayan::is_heap_until(xs, cmp);

        REQUIRE(r_std == xs.end());

        REQUIRE(r.traversed_begin() == xs.begin());
        REQUIRE(r.begin() == r_std);
        REQUIRE(r.end() == xs.end());
        REQUIRE(r.traversed_end() == xs.end());

        xs.push_back(sayan::test::get_arbitrary<int>());
        std::push_heap(xs.begin(), xs.end(), cmp);
    }
}

TEST_CASE("algorithm/is_heap")
{
    std::vector<int> xs;
    for(auto n = 12; n > 0; -- n)
    {
        CAPTURE(xs);

        auto const r_std = std::is_heap(xs.begin(), xs.end());

        auto const r = ::sayan::is_heap(xs);

        REQUIRE(r == r_std);

        xs.push_back(sayan::test::get_arbitrary<int>());
    }
}

TEST_CASE("algorithm/is_heap: custom predicate")
{
    using Input = std::vector<int>;
    auto test_code = [](Input const & xs)
    {
        auto const cmp = ::std::greater<>{};

        CAPTURE(xs);
        auto const r_std = std::is_heap(xs.begin(), xs.end(), cmp);

        auto const r = ::sayan::is_heap(xs, cmp);

        REQUIRE(r == r_std);

        auto xs_heap = xs;
        std::make_heap(xs_heap.begin(), xs_heap.end(), cmp);

        REQUIRE(::sayan::is_heap(xs_heap, cmp));
    };

    Input xs;

    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());

        test_code(xs);
    }
}

TEST_CASE("algorithm/push_heap")
{
    using Input = std::vector<int>;

    auto test_code = [](Input xs)
    {
        CAPTURE(xs);

        REQUIRE((xs.empty() || std::is_heap(xs.begin(), xs.end() - 1)));

        ::sayan::push_heap(xs);

        REQUIRE(::std::is_heap(xs.begin(), xs.end()));
        REQUIRE(::sayan::is_heap(xs));
    };

    Input xs;

    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());
        std::push_heap(xs.begin(), xs.end());

        test_code(xs);
    }
}

TEST_CASE("algorithm/push_heap: custom predicate")
{
    using Input = std::vector<int>;

    auto const cmp = ::std::greater<>{};

    auto test_code = [=](Input xs)
    {
        CAPTURE(xs);
        REQUIRE((xs.empty() || std::is_heap(xs.begin(), xs.end() - 1, cmp)));

        ::sayan::push_heap(xs, cmp);

        REQUIRE(::std::is_heap(xs.begin(), xs.end(), cmp));
        REQUIRE(::sayan::is_heap(xs, cmp));
    };

    Input xs;

    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());
        std::push_heap(xs.begin(), xs.end(), cmp);

        test_code(xs);
    }
}

TEST_CASE("algorithm/pop_heap")
{
    using Input = std::vector<int>;

    auto test_code = [](Input xs)
    {
        CAPTURE(xs);

        REQUIRE(std::is_heap(xs.begin(), xs.end()));

        if(xs.empty())
        {
            ::sayan::pop_heap(xs);
        }
        else
        {
            auto const old_top = xs.front();

            ::sayan::pop_heap(xs);

            CAPTURE(xs);
            REQUIRE(xs.back() == old_top);
            REQUIRE((xs.empty() || ::std::is_heap(xs.begin(), xs.end() - 1)));
        }
    };

    Input xs;
    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());
        std::push_heap(xs.begin(), xs.end());

        test_code(xs);
    }
}

TEST_CASE("algorithm/pop_heap: custom predicate")
{
    using Input = std::vector<int>;
    auto const cmp = ::std::greater<>{};

    auto test_code = [=](Input xs)
    {
        CAPTURE(xs);

        REQUIRE(std::is_heap(xs.begin(), xs.end(), cmp));

        if(xs.empty())
        {
            ::sayan::pop_heap(xs, cmp);
        }
        else
        {
            auto const old_top = xs.front();

            ::sayan::pop_heap(xs, cmp);

            REQUIRE(xs.back() == old_top);
            REQUIRE((xs.empty() || ::std::is_heap(xs.begin(), xs.end() - 1, cmp)));
        }
    };

    Input xs;
    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());
        std::push_heap(xs.begin(), xs.end(), cmp);

        test_code(xs);
    }
}

TEST_CASE("algorithm/make_heap")
{
    using Input = std::vector<int>;

    auto test_code = [](Input xs)
    {
        CAPTURE(xs);

        ::sayan::make_heap(xs);

        CAPTURE(xs);
        REQUIRE(::std::is_heap(xs.begin(), xs.end()));
    };

    Input xs;
    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());

        test_code(xs);
    }
}

TEST_CASE("algorithm/make_heap: custom predicate")
{
    using Input = std::vector<int>;
    auto const cmp = ::std::greater<>{};

    auto test_code = [=](Input xs)
    {
        CAPTURE(xs);

        ::sayan::make_heap(xs, cmp);

        CAPTURE(xs);
        REQUIRE(::std::is_heap(xs.begin(), xs.end(), cmp));
    };

    Input xs;
    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());

        test_code(xs);
    }
}

TEST_CASE("algorithm/sort_heap")
{
    using Input = std::vector<int>;

    auto test_code = [=](Input xs)
    {
        CAPTURE(xs);

        ::sayan::sort_heap(xs);

        CAPTURE(xs);
        REQUIRE(::std::is_sorted(xs.begin(), xs.end()));
    };

    Input xs;
    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());
        ::sayan::push_heap(xs);

        test_code(xs);
    }
}

TEST_CASE("algorithm/sort_heap: custom predicate")
{
    using Input = std::vector<int>;
    auto const cmp = ::std::greater<>{};

    auto test_code = [=](Input xs)
    {
        CAPTURE(xs);

        ::sayan::sort_heap(xs, cmp);

        CAPTURE(xs);
        REQUIRE(::std::is_sorted(xs.begin(), xs.end(), cmp));
    };

    Input xs;
    test_code(xs);
    for(auto n = 12; n > 0; -- n)
    {
        xs.push_back(sayan::test::get_arbitrary<int>());
        ::sayan::push_heap(xs, cmp);

        test_code(xs);
    }
}
