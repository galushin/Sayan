#include <sayan/cursor/reverse.hpp>

#include <sayan/cursor/inserters.hpp>

#include <sayan/algorithm.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("cursor/reverse: concept checks for concept check")
{
    std::list<int> bidir_src;
    std::vector<int> ra_src;

    auto bidir    = ::sayan::cursor(bidir_src);
    auto ra       = ::sayan::cursor(ra_src);

    // Курсор ввода
    static_assert(sayan::is_input_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_input_cursor<decltype(ra)>::value, "");

    // Курсор вывода
    static_assert(sayan::is_output_cursor<decltype(bidir), int>::value, "");
    static_assert(sayan::is_output_cursor<decltype(ra), int>::value, "");

    static_assert(!sayan::is_output_cursor<decltype(bidir), void*>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(ra), void*>::value, "");

    // Прямой курсор
    static_assert(sayan::is_forward_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_forward_cursor<decltype(ra)>::value, "");

    // Двунаправленный курсор
    static_assert(sayan::is_bidirectional_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_bidirectional_cursor<decltype(ra)>::value, "");

    // Курсор произвольного доступа
    static_assert(!sayan::is_random_access_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_random_access_cursor<decltype(ra)>::value, "");
}

TEST_CASE("cursors/reverse: concept checks for const containers")
{
    std::list<int> const bidir_src;
    std::vector<int> const ra_src;

    auto bidir    = ::sayan::cursor(bidir_src);
    auto ra       = ::sayan::cursor(ra_src);

    // Курсор ввода
    static_assert(sayan::is_input_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_input_cursor<decltype(ra)>::value, "");

    // Курсор вывода
    static_assert(!sayan::is_output_cursor<decltype(bidir), int>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(ra), int>::value, "");

    static_assert(!sayan::is_output_cursor<decltype(bidir), void*>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(ra), void*>::value, "");

    // Прямой курсор
    static_assert(sayan::is_forward_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_forward_cursor<decltype(ra)>::value, "");

    // Двунаправленный курсор
    static_assert(sayan::is_bidirectional_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_bidirectional_cursor<decltype(ra)>::value, "");

    // Курсор произвольного доступа
    static_assert(!sayan::is_random_access_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_random_access_cursor<decltype(ra)>::value, "");
}

TEST_CASE("cursor/reverse: double reverse is identity")
{
    auto const n = sayan::test::random_integral<size_t>(0, 20);
    auto const xs = sayan::test::get_arbitrary_container<std::vector<int>>(n);

    auto const cur = ::sayan::cursor(xs);
    auto const r_cur = ::sayan::make_reverse_cursor(cur);
    auto const r2_cur = ::sayan::make_reverse_cursor(r_cur);

    static_assert(std::is_same<decltype(cur), decltype(r2_cur)>::value, "must be same type");

    REQUIRE(r2_cur.traversed_begin() == cur.traversed_begin());
    REQUIRE(r2_cur.begin() == cur.begin());
    REQUIRE(r2_cur.end() == cur.end());
    REQUIRE(r2_cur.traversed_end() == cur.traversed_end());
}

TEST_CASE("cursor/reverse: input")
{
    auto const n = sayan::test::random_integral<size_t>(0, 20);
    auto const xs = sayan::test::get_arbitrary_container<std::vector<int>>(n);

    auto const cur = ::sayan::cursor(xs);
    auto const r_cur = ::sayan::make_reverse_cursor(xs);

    auto const pred = [](int x) { return x % 3 == 0;};

    auto const n1 = ::sayan::count_if(cur, pred);
    auto const n2 = ::sayan::count_if(r_cur, pred);

    REQUIRE(n1 == n2);
}

TEST_CASE("cursor/reverse: output")
{
    auto const n = sayan::test::random_integral<size_t>(0, 20);
    auto xs = sayan::test::get_arbitrary_container<std::vector<int>>(n);
    auto const new_value = 42;

    auto const r_cur = ::sayan::make_reverse_cursor(xs);

    ::sayan::fill(r_cur, new_value);
    std::vector<int> const z(xs.size(), new_value);

    REQUIRE(xs == z);
}

TEST_CASE("cursor/reverse: rotate_copy")
{
    for(auto T = 100; T > 0; -- T)
    {
        auto const n = sayan::test::random_integral<size_t>(0, 20);
        auto const k = sayan::test::random_integral(0*n, n);
        auto xs = sayan::test::get_arbitrary_container<std::list<int>>(n);

        auto const cur = sayan::next(sayan::cursor(xs), k);
        auto const cur_r = ::sayan::next(::sayan::make_reverse_cursor(xs), n-k);

        std::vector<int> out;
        ::sayan::rotate_copy(cur, sayan::back_inserter(out));

        std::vector<int> out_r;
        ::sayan::rotate_copy(cur_r, sayan::back_inserter(out_r));
        sayan::reverse(out_r);

        REQUIRE(out_r == out);
    }
}

TEST_CASE("cursor/reverse: rotate")
{
    for(auto T = 100; T > 0; -- T)
    {
        auto const n = sayan::test::random_integral<size_t>(0, 20);
        auto const k = sayan::test::random_integral(0*n, n);
        auto xs = sayan::test::get_arbitrary_container<std::list<int>>(n);

        auto xs_r = xs;

        auto const cur = sayan::next(sayan::cursor(xs), k);
        auto const cur_r = ::sayan::next(::sayan::make_reverse_cursor(xs_r), n-k);

        ::sayan::rotate(cur);
        ::sayan::rotate(cur_r);

        REQUIRE(xs_r == xs);
    }
}

TEST_CASE("cursor/reverse: random access, sort")
{
    for(auto T = 100; T > 0; -- T)
    {
        auto const n = sayan::test::random_integral<size_t>(0, 20);
        auto xs = sayan::test::get_arbitrary_container<std::vector<int>>(n);
        auto const xs_old = xs;

        auto cur = ::sayan::make_reverse_cursor(xs);

        REQUIRE(cur.size() == xs.size());

        ::sayan::sort(cur);
        ::sayan::reverse(cur);

        CAPTURE(xs_old);
        CAPTURE(xs);

        REQUIRE(sayan::is_permutation(xs, xs_old));
        REQUIRE(sayan::is_sorted(xs));
    }
}

TEST_CASE("cursor/reverse: random access, stable_sort")
{
    for(auto T = 100; T > 0; -- T)
    {
        auto const n = sayan::test::random_integral<size_t>(0, 20);
        auto xs = sayan::test::get_arbitrary_container<std::vector<int>>(n);
        auto const xs_old = xs;

        auto cur = ::sayan::make_reverse_cursor(xs);

        REQUIRE(cur.size() == xs.size());

        ::sayan::stable_sort(cur);
        ::sayan::reverse(cur);

        CAPTURE(xs_old);
        CAPTURE(xs);

        REQUIRE(sayan::is_permutation(xs, xs_old));
        REQUIRE(sayan::is_sorted(xs));
    }
}
