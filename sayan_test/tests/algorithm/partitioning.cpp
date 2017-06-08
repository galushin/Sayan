#include <sayan/algorithm.hpp>

#include <catch/catch.hpp>

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

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == out_true.begin() + (result_std.first - out_true_std.begin()));
    CHECK(std::get<1>(result).end() == out_true.end());

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

    CHECK(std::get<0>(result).begin() == src.begin() + n_out_true + n_out_false);
    CHECK(std::get<0>(result).end() == src.end());
    CHECK(std::get<1>(result).begin() == out_true.begin() + n_out_true);
    CHECK(std::get<1>(result).end() == out_true.end());
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

    CHECK(std::get<0>(result).begin() == src.begin() + n_out_true + n_out_false);
    CHECK(std::get<0>(result).end() == src.end());
    CHECK(std::get<1>(result).begin() == out_true.begin() + n_out_true);
    CHECK(std::get<1>(result).end() == out_true.end());
    CHECK(std::get<2>(result).begin() == out_false.begin() + n_out_false);
    CHECK(std::get<2>(result).end() == out_false.end());
}
