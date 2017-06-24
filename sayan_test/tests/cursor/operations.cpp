#include <sayan/cursor/sequence_to_cursor.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("cursor/advance and size")
{
    for (auto T = 100; T > 0; --T)
    {
        auto const n = sayan::test::random_integral<size_t>(0, 20);
        auto const xs = sayan::test::get_arbitrary_container<std::vector<int>>(n);
        auto const k = ::sayan::test::random_integral(0*n, n);

        auto cur = ::sayan::cursor(xs);

        auto b = cur.begin();

        sayan::advance(cur, k);
        std::advance(b, k);

        REQUIRE(cur.traversed_begin() == xs.begin());
        REQUIRE(cur.begin() == b);
        REQUIRE(cur.end() == xs.end());
        REQUIRE(cur.traversed_end() == xs.end());

        REQUIRE(::sayan::size(cur) == (xs.end() - b));

        REQUIRE_THROWS(sayan::advance(cur, xs.size()+1));
    }
}

TEST_CASE("cursor/next and size")
{
    for (auto T = 100; T > 0; --T)
    {
        auto const n = sayan::test::random_integral<size_t>(0, 20);
        auto const xs = sayan::test::get_arbitrary_container<std::vector<int>>(n);
        auto const k = ::sayan::test::random_integral(0*n, n);

        auto const cur = ::sayan::cursor(xs);
        auto b = cur.begin();

        auto const cur_k = sayan::next(cur, k);
        std::advance(b, k);

        REQUIRE(cur_k.traversed_begin() == xs.begin());
        REQUIRE(cur_k.begin() == b);
        REQUIRE(cur_k.end() == xs.end());
        REQUIRE(cur_k.traversed_end() == xs.end());

        REQUIRE(::sayan::size(cur_k) == (xs.end() - b));

        REQUIRE_THROWS(sayan::next(cur, xs.size()+1));
    }
}
