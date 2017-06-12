#include <sayan/numeric.hpp>

#include <sayan/cursor/back_inserter.hpp>

#include <forward_list>
#include <numeric>
#include <iterator>

#include <catch/catch.hpp>

TEST_CASE("numeric/iota_test")
{
    using Value = int;
    using Container = std::forward_list<Value>;
    auto const n = size_t{13};
    auto const init_value = Value{42};

    Container xs_std(n, -1);
    auto xs = xs_std;

    std::iota(xs_std.begin(), xs_std.end(), init_value);

    auto const result = sayan::iota(xs, init_value);

    CHECK(xs == xs_std);

    CHECK(result.begin() == xs.end());
    CHECK(result.end() == xs.end());
}

TEST_CASE("numeric/iota_test: explicit cursor")
{
    using Value = int;
    using Container = std::forward_list<Value>;
    auto const n = size_t{13};
    auto const init_value = Value{42};

    Container xs_std(n, -1);
    auto xs = xs_std;

    std::iota(xs_std.begin(), xs_std.end(), init_value);

    auto const result = sayan::iota(sayan::cursor(xs), init_value);

    CHECK(xs == xs_std);

    CHECK(result.begin() == xs.end());
    CHECK(result.end() == xs.end());
}

TEST_CASE("numeric/accumulate")
{
    std::string const src{"0123456789abcdef"};

    auto const init_value = int{0};

    auto const r_std = std::accumulate(src.begin(), src.end(), init_value);

    auto const r = sayan::accumulate(std::istringstream(src), init_value);

    CHECK(r == r_std);
}

TEST_CASE("numeric/accumulate: custom operation")
{
    std::string const src{"0123456789abcdef"};

    auto const init_value = int{0};
    auto const op = std::bit_xor<>{};

    auto const r_std = std::accumulate(src.begin(), src.end(), init_value, op);

    auto const r = sayan::accumulate(std::istringstream(src), init_value, op);

    CHECK(r == r_std);
}

TEST_CASE("numeric/innter_product")
{
    std::string const src_1{"abcdef"};
    std::string const src_2{"0123456789"};

    auto const init_value = int{0};

    auto const r_std = std::inner_product(src_1.begin(), src_1.end(), src_2.begin(), init_value);

    auto const r = sayan::inner_product(src_1, src_2, init_value);

    CHECK(r == r_std);
}

TEST_CASE("numeric/innter_product: custom ops")
{
    std::string const src_1{"abcdef"};
    std::string const src_2{"0123456789"};

    auto const init_value = int{0};
    auto const add = std::bit_or<int>{};
    auto const mult = std::bit_and<int>{};

    auto const r_std
        = std::inner_product(src_1.begin(), src_1.end(), src_2.begin(), init_value, add, mult);

    auto const r = sayan::inner_product(src_1, src_2, init_value, add, mult);

    CHECK(r == r_std);
}

TEST_CASE("numeric/inner_product: curstom ops, first is shorter")
{
    using Container = std::vector<int>;
    Container const src_1{1, 2, 4, 8, 16, 32};
    Container const src_2{1, 2, 4, 6, 8,  10, 12};

    CHECK(src_1.size() < src_2.size());

    auto const init_value = int{1};
    auto const add = std::multiplies<>{};
    auto const mult = [](auto x, auto y) { return std::min(x, y); };

    auto const r = sayan::inner_product(src_1, src_2, init_value, add, mult);

    auto const r_std = std::inner_product(src_1.begin(), src_1.end(), src_2.begin(),
                                          init_value, add, mult);

    CHECK(r == r_std);
}

TEST_CASE("numeric/inner_product: curstom ops, second is shorter")
{
    using Container = std::vector<int>;
    Container const src_1{1, 2, 4, 8, 16, 32, 64};
    Container const src_2{1, 2, 4, 6, 8,  10};

    CHECK(src_2.size() < src_1.size());

    auto const init_value = int{1};
    auto const add = std::multiplies<>{};
    auto const mult = [](auto x, auto y) { return std::min(x, y); };

    auto const r = sayan::inner_product(src_1, src_2, init_value, add, mult);

    auto const r_std = std::inner_product(src_1.begin(), src_1.begin() + src_2.size(),
                                          src_2.begin(), init_value, add, mult);

    CHECK(r == r_std);
}

TEST_CASE("numeric/partial_sum")
{
    std::string const src{"0123456789abcdef"};

    std::ostringstream os_std;
    std::partial_sum(src.begin(), src.end(), std::ostream_iterator<char>(os_std));

    std::ostringstream os;
    auto const r = sayan::partial_sum(std::istringstream(src), os);

    CHECK(os.str() == os_std.str());
}

TEST_CASE("numeric/partial_sum: custom operation")
{
    std::string const src{"0123456789abcdef"};
    auto const op = std::bit_xor<>{};

    std::ostringstream os_std;
    std::partial_sum(src.begin(), src.end(), std::ostream_iterator<char>(os_std), op);

    std::ostringstream os;
    auto const r = sayan::partial_sum(std::istringstream(src), os, op);

    CHECK(os.str() == os_std.str());
}

TEST_CASE("numeric/partial_sum: custom operation, to longer")
{
    using Container = std::vector<int>;
    Container const src{1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    auto const op = std::multiplies<>{};

    Container xs_std(src.size() * 2, -1);
    auto xs = xs_std;

    auto const result_std = std::partial_sum(src.begin(), src.end(), xs_std.begin(), op);

    auto const result = sayan::partial_sum(src, xs, op);

    CHECK(xs == xs_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == xs.begin());
    CHECK(std::get<1>(result).begin() == xs.begin() + (result_std - xs_std.begin()));
    CHECK(std::get<1>(result).end() == xs.end());
}

TEST_CASE("numeric/partial_sum: custom operation, to shorter")
{
    using Container = std::vector<int>;
    Container const src{1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    auto const op = std::multiplies<>{};

    Container xs(src.size() / 2, -1);
    auto const result = sayan::partial_sum(src, xs, op);

    Container xs_std;
    std::partial_sum(src.begin(), src.begin() + xs.size(), std::back_inserter(xs_std), op);

    CHECK(xs == xs_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + xs.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == xs.begin());
    CHECK(std::get<1>(result).begin() == xs.end());
    CHECK(std::get<1>(result).end() == xs.end());
}

TEST_CASE("numeric/adjacent_difference")
{
    std::string const src{"0123456789abcdef"};

    std::vector<int> xs_std;
    std::adjacent_difference(src.begin(), src.end(), std::back_inserter(xs_std));

    std::vector<int> xs;
    auto const r = sayan::adjacent_difference(std::istringstream(src), sayan::back_inserter(xs));

    CHECK(xs == xs_std);
}

TEST_CASE("numeric/adjacent_difference: custom operation")
{
    std::string const src{"0123456789abcdef"};
    auto const op = std::bit_xor<>{};

    std::vector<int> xs_std;
    std::adjacent_difference(src.begin(), src.end(), std::back_inserter(xs_std), op);

    std::vector<int> xs;
    auto const r
        = sayan::adjacent_difference(std::istringstream(src), sayan::back_inserter(xs), op);

    CHECK(xs == xs_std);
}

TEST_CASE("numeric/adjacent_difference: custom operation, to longer")
{
    using Container = std::vector<int>;
    Container const src{1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    auto const op = std::divides<>{};

    Container xs_std(src.size() * 2, -1);
    auto xs = xs_std;

    auto const result_std = std::adjacent_difference(src.begin(), src.end(), xs_std.begin(), op);

    auto const result = sayan::adjacent_difference(src, xs, op);

    CHECK(xs == xs_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == xs.begin());
    CHECK(std::get<1>(result).begin() == xs.begin() + (result_std - xs_std.begin()));
    CHECK(std::get<1>(result).end() == xs.end());
}

TEST_CASE("numeric/adjacent_difference: custom operation, to shorter")
{
    using Container = std::vector<int>;
    Container const src{1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    auto const op = std::divides<>{};

    Container xs(src.size() / 2, -1);
    auto const result = sayan::adjacent_difference(src, xs, op);

    Container xs_std;
    std::adjacent_difference(src.begin(), src.begin() + xs.size(), std::back_inserter(xs_std), op);

    CHECK(xs == xs_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + xs.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == xs.begin());
    CHECK(std::get<1>(result).begin() == xs.end());
    CHECK(std::get<1>(result).end() == xs.end());
}
