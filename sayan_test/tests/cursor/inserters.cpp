#include <sayan/cursor/inserters.hpp>

#include <sayan/algorithm.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("cursors/back_inserter: concept check")
{
    std::string dest;
    auto cur = sayan::back_inserter(dest);

    static_assert(sayan::is_output_cursor<decltype(cur), char>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(cur), void*>::value, "");
    static_assert(!sayan::is_input_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_forward_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_bidirectional_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_random_access_cursor<decltype(cur)>::value, "");
}

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

TEST_CASE("cursors/front_inserter: concept check")
{
    std::list<int> dest;
    auto cur = sayan::front_inserter(dest);

    static_assert(sayan::is_output_cursor<decltype(cur), int>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(cur), void*>::value, "");
    static_assert(!sayan::is_input_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_forward_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_bidirectional_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_random_access_cursor<decltype(cur)>::value, "");
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

TEST_CASE("cursors/inserter: concept check")
{
    std::list<int> dest;
    auto cur = sayan::inserter(dest, dest.begin());

    static_assert(sayan::is_output_cursor<decltype(cur), int>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(cur), void*>::value, "");
    static_assert(!sayan::is_input_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_forward_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_bidirectional_cursor<decltype(cur)>::value, "");
    static_assert(!sayan::is_random_access_cursor<decltype(cur)>::value, "");
}

TEST_CASE("cursors/inserter: to list")
{
    for(auto T = 100; T > 0; -- T)
    {
        auto const src = sayan::test::get_arbitrary_container<std::vector<int>>(0, 20);

        auto dest_std = sayan::test::get_arbitrary_container<std::list<int>>(0, 20);
        auto dest_sayan = dest_std;

        auto const k = sayan::test::random_integral<size_t>(0*dest_std.size(), dest_std.size());
        auto out_std = std::inserter(dest_std, std::next(dest_std.begin(), k));
        auto out_sayan = sayan::inserter(dest_sayan, std::next(dest_sayan.begin(), k));

        std::copy(src.begin(), src.end(), out_std);
        sayan::copy(src, out_sayan);

        REQUIRE(dest_sayan == dest_std);
    }
}

TEST_CASE("cursors/inserter: to list, rvalue")
{
    using Value = sayan::test::move_only<int>;
    for(auto T = 100; T > 0; -- T)
    {
        auto src_std = sayan::test::get_arbitrary_container<std::vector<Value>>(0, 20);

        std::vector<Value> src_sayan;
        for(auto const & x : src_std)
        {
            src_sayan.emplace_back(x.value());
        }

        auto dest_std = sayan::test::get_arbitrary_container<std::list<Value>>(0, 20);

        std::list<Value> dest_sayan;
        for(auto const & x : dest_std)
        {
            dest_sayan.emplace_back(x.value());
        }

        auto const k = sayan::test::random_integral<size_t>(0*dest_std.size(), dest_std.size());
        auto out_std = std::inserter(dest_std, std::next(dest_std.begin(), k));
        auto out_sayan = sayan::inserter(dest_sayan, std::next(dest_sayan.begin(), k));

        std::move(src_std.begin(), src_std.end(), out_std);
        sayan::move(src_sayan, out_sayan);

        REQUIRE(dest_sayan == dest_std);
    }
}

TEST_CASE("cursors/inserter: to set")
{
    for(auto T = 100; T > 0; -- T)
    {
        auto const src = sayan::test::get_arbitrary_container<std::vector<int>>(0, 20);

        auto dest_std = sayan::test::get_arbitrary_container<std::set<int>>(0, 20);
        auto dest_sayan = dest_std;

        auto const k = sayan::test::random_integral<size_t>(0*dest_std.size(), dest_std.size());
        auto out_std = std::inserter(dest_std, std::next(dest_std.begin(), k));
        auto out_sayan = sayan::inserter(dest_sayan, std::next(dest_sayan.begin(), k));

        std::copy(src.begin(), src.end(), out_std);
        sayan::copy(src, out_sayan);

        REQUIRE(dest_sayan == dest_std);
    }
}

TEST_CASE("cursors/inserter: to set, rvalue")
{
    using Value = sayan::test::move_only<int>;
    for(auto T = 100; T > 0; -- T)
    {
        auto src_std = sayan::test::get_arbitrary_container<std::vector<Value>>(0, 20);

        std::vector<Value> src_sayan;
        for(auto const & x : src_std)
        {
            src_sayan.emplace_back(x.value());
        }

        auto dest_std = sayan::test::get_arbitrary_container<std::set<Value>>(0, 20);

        std::set<Value> dest_sayan;
        for(auto const & x : dest_std)
        {
            dest_sayan.emplace(x.value());
        }

        auto const k = sayan::test::random_integral<size_t>(0*dest_std.size(), dest_std.size());
        auto out_std = std::inserter(dest_std, std::next(dest_std.begin(), k));
        auto out_sayan = sayan::inserter(dest_sayan, std::next(dest_sayan.begin(), k));

        std::move(src_std.begin(), src_std.end(), out_std);
        sayan::move(src_sayan, out_sayan);

        REQUIRE(dest_sayan == dest_std);
    }
}
