#include <sayan/algorithm.hpp>

#include <forward_list>
#include <catch/catch.hpp>

#include <iostream>

TEST_CASE("algorithm/fill")
{
    using Value = int;
    using Container = std::forward_list<Value>;

    auto const x_old = Value{-1};
    auto const x_new = Value{13};
    auto const n = size_t{42};

    CHECK(x_old != x_new);

    Container const obj(n, x_new);

    Container xs(n, x_old);

    CHECK(xs != obj);

    ::sayan::fill(xs, x_new);

    CHECK(xs == obj);
}

TEST_CASE("algorithm/generate")
{
    using Value = int;
    using Container = std::forward_list<Value>;

    auto const x_old = Value{-1};

    auto const acc_init = Value{0};
    auto acc = acc_init;

    auto const gen = [&acc]() { return ++acc;};

    auto const n = size_t{42};

    Container xs_std(n, x_old);
    auto xs = xs_std;

    std::generate(xs_std.begin(), xs_std.end(), gen);

    acc = acc_init;
    sayan::generate(xs, gen);

    CHECK(xs == xs_std);
}
