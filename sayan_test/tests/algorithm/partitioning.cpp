#include <sayan/algorithm.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

#include <forward_list>

TEST_CASE("algorithm/partition_copy")
{
    std::string const src{"standard algorithms are great"};

    auto const pred = [](char c) { return c % 2; };

    // std
    std::string out_true_std;
    std::string out_false_std;

    std::partition_copy(src.begin(), src.end(),
                        std::back_inserter(out_true_std),
                        std::back_inserter(out_false_std), pred);

    // sayan
    std::ostringstream os_true;
    std::ostringstream os_false;

    sayan::partition_copy(std::istringstream(src), os_true, os_false, pred);

    CHECK(os_true.str() == out_true_std);
    CHECK(os_false.str() == out_false_std);
}

TEST_CASE("algorithm/partition_copy: to longer")
{
    std::string const src{"standard algorithms are great"};

    auto const pred = [](char c) { return c % 2; };

    // std
    std::string out_true_std(src.size() + 2, '*');
    std::string out_false_std(src.size() + 2, '*');

    auto const result_std = std::partition_copy(src.begin(), src.end(),
                                                out_true_std.begin(), out_false_std.begin(), pred);

    // sayan
    std::string out_true(src.size() + 2, '*');
    std::string out_false(src.size() + 2, '*');

    auto const result = sayan::partition_copy(src, out_true, out_false, pred);

    CHECK(out_true == out_true_std);
    CHECK(out_false == out_false_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == out_true.begin());
    CHECK(std::get<1>(result).begin() == out_true.begin() + (result_std.first - out_true_std.begin()));
    CHECK(std::get<1>(result).end() == out_true.end());

    CHECK(std::get<2>(result).traversed_begin() == out_false.begin());
    CHECK(std::get<2>(result).begin() == out_false.begin() + (result_std.second - out_false_std.begin()));
    CHECK(std::get<2>(result).end() == out_false.end());
}

TEST_CASE("algorithm/partition_copy: stops by out_true")
{
    std::string const src{"standard algorithms are great"};
    auto const pred = [](char c) { return c % 2; };

    auto const n_true = sayan::count_if(src, pred);

    // sayan
    std::string out_true(n_true / 2, '*');
    std::string out_false(src.size() - n_true + 2, '*');

    auto const result = sayan::partition_copy(src, out_true, out_false, pred);
    auto n_taken = std::get<0>(result).begin() - src.begin();

    // std
    std::string out_true_std(out_true.size(), '*');
    std::string out_false_std(out_false.size(), '*');

    auto const result_std = std::partition_copy(src.begin(), src.begin() + n_taken,
                                                out_true_std.begin(), out_false_std.begin(), pred);

    // Проверки
    CHECK((out_true.size() <= src.size() && out_true.size() <= out_false.size()));

    CHECK(out_true == out_true_std);
    CHECK(out_false == out_false_std);

    auto const n_out_true = (result_std.first - out_true_std.begin());
    auto const n_out_false = (result_std.second - out_false_std.begin());

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + n_out_true + n_out_false);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == out_true.begin());
    CHECK(std::get<1>(result).begin() == out_true.begin() + n_out_true);
    CHECK(std::get<1>(result).end() == out_true.end());

    CHECK(std::get<2>(result).traversed_begin() == out_false.begin());
    CHECK(std::get<2>(result).begin() == out_false.begin() + n_out_false);
    CHECK(std::get<2>(result).end() == out_false.end());
}

TEST_CASE("algorithm/partition_copy: stops by out_false")
{
    std::string const src{"standard algorithms are great"};
    auto const pred = [](char c) { return c % 2; };

    auto const n_true = sayan::count_if(src, pred);

    // sayan
    std::string out_true(n_true, '*');
    std::string out_false((src.size() - n_true) / 2, '*');

    auto const result = sayan::partition_copy(src, out_true, out_false, pred);
    auto n_taken = std::get<0>(result).begin() - src.begin();

    // std
    std::string out_true_std(out_true.size(), '*');
    std::string out_false_std(out_false.size(), '*');

    auto const result_std = std::partition_copy(src.begin(), src.begin() + n_taken,
                                                out_true_std.begin(), out_false_std.begin(), pred);

    // Проверки
    CHECK((out_false.size() <= src.size() && out_false.size() <= out_true.size()));

    CHECK(out_true == out_true_std);
    CHECK(out_false == out_false_std);

    auto const n_out_true = (result_std.first - out_true_std.begin());
    auto const n_out_false = (result_std.second - out_false_std.begin());

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + n_out_true + n_out_false);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == out_true.begin());
    CHECK(std::get<1>(result).begin() == out_true.begin() + n_out_true);
    CHECK(std::get<1>(result).end() == out_true.end());

    CHECK(std::get<2>(result).traversed_begin() == out_false.begin());
    CHECK(std::get<2>(result).begin() == out_false.begin() + n_out_false);
    CHECK(std::get<2>(result).end() == out_false.end());
}

TEST_CASE("algorithm/partition")
{
    std::forward_list<int> xs{1,2,3,5,6,8,9,11,13,14};
    auto const xs_old = xs;
    auto const pred = [](auto const & x) { return x % 2 == 0; };

    auto const r = sayan::partition(xs, pred);

    CHECK(::std::is_permutation(xs.begin(), xs.end(), xs_old.begin(), xs_old.end()));
    CHECK(::sayan::is_permutation(xs, xs_old));

    CHECK(::std::is_partitioned(xs.begin(), xs.end(), pred));
    CHECK(::sayan::is_partitioned(xs, pred));

    CHECK(std::all_of(xs.begin(), r.begin(), pred));
    CHECK(sayan::all_of(r.traversed(sayan::front), pred));

    CHECK(std::none_of(r.begin(), r.end(), pred));
    CHECK(sayan::none_of(r, pred));

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/stable_partition")
{
    std::vector<int> xs;
    auto const pred = [](auto const & x) { return x % 2 == 0; };

    for(auto n = 20; n > 0; -- n, xs.push_back(sayan::test::get_arbitrary<int>()))
    {
        CAPTURE(xs);

        auto xs_std = xs;
        auto xs_sayan = xs;

        auto const r_std = std::stable_partition(xs_std.begin(), xs_std.end(), pred);

        auto const r_sayan = sayan::stable_partition(xs_sayan, pred);

        REQUIRE(xs_std == xs_sayan);

        REQUIRE(r_sayan.traversed_begin() == xs_sayan.begin());
        REQUIRE(std::distance(xs_sayan.begin(), r_sayan.begin())
                == std::distance(xs_std.begin(), r_std));
        REQUIRE(r_sayan.end() == xs_sayan.end());
        REQUIRE(r_sayan.traversed_end() == xs_sayan.end());
    }
}

TEST_CASE("algorithm/partition_point")
{
    auto const pred = [](int x){ return x % 2 == 0;};

    auto xs = [&]()
    {
        std::forward_list<int> xs{1,2,3,5,6,8,9,11,13,14};
        std::partition(xs.begin(), xs.end(), pred);
        return xs;
    }();

    auto const r_std = std::partition_point(xs.begin(), xs.end(), pred);
    auto const r_sayan = sayan::partition_point(xs, pred);

    CHECK(r_sayan.traversed_begin() == xs.begin());
    CHECK(r_sayan.begin() == r_std);
    CHECK(r_sayan.end() == xs.end());
}

TEST_CASE("algorithm/partition_point: empty sequence")
{
    auto const pred = [](int x){ return x % 2 == 0;};

    std::forward_list<int> xs;

    auto const r_sayan = sayan::partition_point(xs, pred);

    CHECK(r_sayan.traversed_begin() == xs.begin());
    CHECK(r_sayan.begin() == xs.end());
    CHECK(r_sayan.end() == xs.end());
}

TEST_CASE("algorithm/partition_point: all true")
{
    auto const pred = [](int x){ return x % 2 == 0;};

    std::forward_list<int> xs{2,6,8,14};

    CHECK(sayan::all_of(xs, pred));

    auto const r_sayan = sayan::partition_point(xs, pred);

    CHECK(r_sayan.traversed_begin() == xs.begin());
    CHECK(r_sayan.begin() == xs.end());
    CHECK(r_sayan.end() == xs.end());
}

TEST_CASE("algorithm/partition_point: all false")
{
    auto const pred = [](int x){ return x % 2 == 0;};

    std::forward_list<int> xs{1,3,5,9,11,13};

    CHECK(sayan::none_of(xs, pred));

    auto const r_sayan = sayan::partition_point(xs, pred);

    CHECK(r_sayan.traversed_begin() == xs.begin());
    CHECK(r_sayan.begin() == xs.begin());
    CHECK(r_sayan.end() == xs.end());
}
