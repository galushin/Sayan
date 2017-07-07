#include <sayan/cursor/move.hpp>

#include <sayan/cursor/inserters.hpp>
#include <sayan/algorithm.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("cursor/move: input")
{
    using Value = int;
    auto const src = sayan::test::get_arbitrary_container<std::vector<Value>>(0, 20);

    std::vector<std::unique_ptr<Value>> src_p;
    for(auto const & x : src)
    {
        src_p.emplace_back(std::make_unique<Value>(x));
    }

    auto in = sayan::make_move_cursor(src_p);
    REQUIRE(in.size() == src_p.size());

    std::list<std::unique_ptr<Value>> dest_p;
    sayan::copy(in, sayan::back_inserter(dest_p));

    std::vector<Value> dest;
    for(auto const & p : dest_p)
    {
        dest.emplace_back(*p);
    }

    REQUIRE(dest == src);

    REQUIRE(sayan::all_of(src_p, [](std::unique_ptr<Value> const & p) { return !p; }));
}

TEST_CASE("cursor/move: traversed front")
{
    std::forward_list<std::string> xs(13, "abc");

    auto cur = sayan::make_move_cursor(xs);

    auto t = cur.traversed(sayan::front);

    CHECK(t.base().traversed_begin() == xs.begin());
    CHECK(t.base().begin() == xs.begin());
    CHECK(t.base().end() == xs.begin());
    CHECK(t.base().traversed_end() == xs.begin());
}

TEST_CASE("cursor/move: exhaust front")
{
    std::forward_list<std::string> xs(13, "abc");

    auto cur = sayan::make_move_cursor(xs);

    cur.exhaust(sayan::front);

    CHECK(cur.base().traversed_begin() == xs.begin());
    CHECK(cur.base().begin() == xs.end());
    CHECK(cur.base().end() == xs.end());
    CHECK(cur.base().traversed_end() == xs.end());
}

TEST_CASE("cursor/move: forget front")
{
    auto const n = 13;
    auto const k = n / 3;
    std::forward_list<std::string> xs(n, "abc");

    auto cur = sayan::next(sayan::make_move_cursor(xs), k);
    auto const iter = std::next(xs.begin(), k);

    cur.forget(sayan::front);

    CHECK(cur.base().traversed_begin() == iter);
    CHECK(cur.base().begin() == iter);
    CHECK(cur.base().end() == xs.end());
    CHECK(cur.base().traversed_end() == xs.end());
}

TEST_CASE("cursor/move: splice")
{
    auto const n = 13;
    auto const k = n / 3;
    std::forward_list<std::string> xs(n, "abc");

    auto cur = sayan::next(sayan::make_move_cursor(xs), k);
    auto tf = cur.traversed(sayan::front);
    tf.splice(cur);

    CHECK(tf.base().traversed_begin() == xs.begin());
    CHECK(tf.base().begin() == xs.begin());
    CHECK(tf.base().end() == xs.end());
    CHECK(tf.base().traversed_end() == xs.end());
}

TEST_CASE("cursor/move: traversed front and back")
{
    std::list<std::string> xs(13, "abc");

    auto cur = sayan::make_move_cursor(xs);

    auto const tf = cur.traversed(sayan::front);

    CHECK(tf.base().traversed_begin() == xs.begin());
    CHECK(tf.base().begin() == xs.begin());
    CHECK(tf.base().end() == xs.begin());
    CHECK(tf.base().traversed_end() == xs.begin());

    auto const tb = cur.traversed(sayan::back);

    CHECK(tb.base().traversed_begin() == xs.end());
    CHECK(tb.base().begin() == xs.end());
    CHECK(tb.base().end() == xs.end());
    CHECK(tb.base().traversed_end() == xs.end());
}

TEST_CASE("cursor/move: exhaust back")
{
    std::list<std::string> xs(13, "abc");

    auto cur = sayan::make_move_cursor(xs);

    cur.exhaust(sayan::back);

    CHECK(cur.base().traversed_begin() == xs.begin());
    CHECK(cur.base().begin() == xs.begin());
    CHECK(cur.base().end() == xs.begin());
    CHECK(cur.base().traversed_end() == xs.end());
}

TEST_CASE("cursor/move: forget front and back")
{
    auto const n = 13;
    std::list<std::string> xs(n, "abc");

    auto cur = sayan::make_move_cursor(xs);

    cur.drop(sayan::front);
    cur.drop(sayan::back);
    cur.forget(sayan::front);
    cur.forget(sayan::back);

    auto const first = std::next(xs.begin());
    auto const last = std::prev(xs.end());

    CHECK(cur.base().traversed_begin() == first);
    CHECK(cur.base().begin() == first);
    CHECK(cur.base().end() == last);
    CHECK(cur.base().traversed_end() == last);
}

TEST_CASE("cursor/move: copy_backward")
{
    using Value = int;
    auto const src = sayan::test::get_arbitrary_container<std::vector<Value>>(0, 20);

    std::vector<std::unique_ptr<Value>> src_p;
    for(auto const & x : src)
    {
        src_p.emplace_back(std::make_unique<Value>(x));
    }

    std::list<std::unique_ptr<Value>> dest_p(src.size());
    sayan::copy_backward(sayan::make_move_cursor(src_p), dest_p);

    std::vector<Value> dest;
    for(auto const & p : dest_p)
    {
        dest.emplace_back(*p);
    }

    REQUIRE(dest == src);

    REQUIRE(sayan::all_of(src_p, [](std::unique_ptr<Value> const & p) { return !p; }));
}

TEST_CASE("cursor/move: drop_n")
{
    auto const n = 13;
    auto const k = n / 3;
    std::vector<std::string> xs(n, "abc");

    auto cur = sayan::make_move_cursor(xs);
    cur.drop(sayan::front, k);
    cur.drop(sayan::back, k);

    CHECK(cur.base().traversed_begin() == xs.begin());
    CHECK(cur.base().begin() == xs.begin() + k);
    CHECK(cur.base().end() == xs.end() - k);
    CHECK(cur.base().traversed_end() == xs.end());
}
