#include <sayan/algorithm.hpp>

#include <forward_list>
#include <catch/catch.hpp>

#include <iostream>

TEST_CASE("algorithm/copy: to infinite")
{
    std::string const src{"Bjarne Stroustrup"};

    std::ostringstream os;
    std::istringstream is(src);

    auto const result = ::sayan::copy(is, os);

    CHECK(os.str() == src);

    CHECK(std::get<0>(result).empty());
}

TEST_CASE("algorithm/copy: to infinite, explicit cursor")
{
    std::string const src{"Bjarne Stroustrup"};

    std::ostringstream os;
    std::istringstream is(src);

    auto const result = ::sayan::copy(sayan::cursor(is), sayan::cursor(os));

    CHECK(os.str() == src);

    CHECK(std::get<0>(result).empty());
}

TEST_CASE("algorithm/copy: to shorter")
{
    std::string const src{"Bjarne Stroustrup"};

    std::string dest(src.size() / 2, '*');

    CHECK(dest.size() < src.size());

    auto const result = ::sayan::copy(src, dest);

    CHECK(src.substr(0, dest.size()) == dest);

    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).empty());
    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/copy: to longer")
{
    std::string const src{"Bjarne Stroustrup"};

    std::string dest(src.size() * 2, '*');
    auto const dest_old = dest;

    CHECK(dest.size() > src.size());

    auto const result = ::sayan::copy(src, dest);

    CHECK(dest.substr(0, src.size()) == src);
    CHECK(dest.substr(src.size()) == dest_old.substr(src.size()));

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.begin() + src.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/transform")
{
    std::string const src{"Bjarne Stroustrup"};

    auto const fun = [](char c) -> char { return std::toupper(c);};

    std::string r_std;
    std::transform(src.begin(), src.end(), std::back_inserter(r_std), fun);

    std::ostringstream os;
    sayan::transform(std::istringstream(src), os, fun);

    CHECK(os.str() == r_std);
}

TEST_CASE("algorithm/transform: to shorter")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const fun = [](char c) -> char { return std::toupper(c);};

    std::string dest(src.size() / 2, '*');

    CHECK(dest.size() < src.size());

    std::string r_std;
    std::transform(src.begin(), src.end(), std::back_inserter(r_std), fun);

    auto const result = ::sayan::transform(src, dest, fun);

    CHECK(dest == r_std.substr(0, dest.size()));

    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).empty());
    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/transform: to longer")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const fun = [](char c) -> char { return std::toupper(c);};

    std::string dest(src.size() * 2, '*');
    auto const dest_old = dest;

    CHECK(dest.size() > src.size());

    std::string r_std;
    std::transform(src.begin(), src.end(), std::back_inserter(r_std), fun);

    auto const result = ::sayan::transform(src, dest, fun);

    CHECK(dest.substr(0, src.size()) == r_std);
    CHECK(dest.substr(src.size()) == dest_old.substr(src.size()));

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.begin() + src.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/transform2")
{
    std::string const src1{"Alex Stepanov"};
    std::string const src2{"Bjarne Stroustrup"};

    auto const fun = std::bit_xor<char>{};

    CHECK(src1.size() <= src2.size());

    std::string r_std;
    std::transform(src1.begin(), src1.end(), src2.begin(),
                   std::back_inserter(r_std), fun);

    std::ostringstream os;
    sayan::transform(std::istringstream(src1), std::istringstream(src2),
                     os, fun);

    CHECK(os.str() == r_std);
}

TEST_CASE("algorithm/transform2: in1 shorter")
{
    std::vector<int> in1{1, 3, 5, 7};
    std::vector<int> in2{2, 4, 6, 8, 10};

    auto const n = std::min(in1.size(), in2.size());

    auto const op = std::plus<>{};

    std::vector<int> out_std;
    std::transform(in1.begin(), in1.begin() + n, in2.begin(),
                   std::back_inserter(out_std), op);

    std::vector<int> out(n + 2, -1);
    auto const out_old = out;

    CHECK(in1.size() <= in2.size());
    CHECK(in1.size() <= out.size());

    auto result = sayan::transform(in1, in2, out, op);

    std::vector<int> out_1(out.begin(), out.begin() + n);
    CHECK(out_1 == out_std);

    CHECK(std::equal(out.begin() + n, out.end(),
                     out_old.begin() + n, out_old.end()));

    CHECK(std::get<0>(result).begin() == in1.end());
    CHECK(std::get<0>(result).end() == in1.end());

    CHECK(std::get<1>(result).begin() == in2.begin() + n);
    CHECK(std::get<1>(result).end() == in2.end());

    CHECK(std::get<2>(result).begin() == out.begin() + n);
    CHECK(std::get<2>(result).end() == out.end());
}

TEST_CASE("algorithm/transform2: in2 shorter")
{
    std::vector<int> in1{1, 3, 5, 7, 9};
    std::vector<int> in2{2, 4, 6, 8};

    auto const n = std::min(in1.size(), in2.size());

    auto const op = std::plus<>{};

    std::vector<int> out_std;
    std::transform(in1.begin(), in1.begin() + n, in2.begin(),
                   std::back_inserter(out_std), op);

    std::vector<int> out(n + 2, -1);
    auto const out_old = out;

    CHECK(in2.size() <= in1.size());
    CHECK(in2.size() <= out.size());

    auto result = sayan::transform(in1, in2, out, op);

    std::vector<int> out_1(out.begin(), out.begin() + n);
    CHECK(out_1 == out_std);

    CHECK(std::equal(out.begin() + n, out.end(),
                     out_old.begin() + n, out_old.end()));

    CHECK(std::get<0>(result).begin() == in1.begin() + n);
    CHECK(std::get<0>(result).end() == in1.end());

    CHECK(std::get<1>(result).begin() == in2.end());
    CHECK(std::get<1>(result).end() == in2.end());

    CHECK(std::get<2>(result).begin() == out.begin() + n);
    CHECK(std::get<2>(result).end() == out.end());
}

TEST_CASE("algorithm/transform2: out shorter")
{
    std::vector<int> in1{1, 3, 5, 7, 9};
    std::vector<int> in2{2, 4, 6, 8};

    auto const op = std::plus<>{};

    std::vector<int> out(std::min(in1.size(), in2.size()) / 2, -1);
    auto out_std = out;

    auto const n = std::min({in1.size(), in2.size(), out.size()});

    std::transform(in1.begin(), in1.begin() + n, in2.begin(), out_std.begin(), op);

    CHECK(out.size() <= in1.size());
    CHECK(out.size() <= in2.size());

    auto result = sayan::transform(in1, in2, out, op);

    CHECK(out == out_std);

    CHECK(std::get<0>(result).begin() == in1.begin() + n);
    CHECK(std::get<0>(result).end() == in1.end());

    CHECK(std::get<1>(result).begin() == in2.begin() + n);
    CHECK(std::get<1>(result).end() == in2.end());

    CHECK(std::get<2>(result).begin() == out.end());
    CHECK(std::get<2>(result).end() == out.end());
}

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

TEST_CASE("algorithm/remove_copy")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const x = 'r';

    std::string r_std;
    std::remove_copy(src.begin(), src.end(), std::back_inserter(r_std), x);

    std::ostringstream os;
    auto result = sayan::remove_copy(std::istringstream(src), os, x);

    CHECK(os.str() == r_std);

    CHECK(std::get<0>(result).empty());
    CHECK(!std::get<1>(result).empty());
}

TEST_CASE("algorithm/remove_copy: to shorter")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const x = 'r';

    std::string r_std;
    std::remove_copy(src.begin(), src.end(), std::back_inserter(r_std), x);

    std::string dest(r_std.size() / 2, '*');

    CHECK(dest.size() < r_std.size());

    auto result = sayan::remove_copy(src, dest, x);

    CHECK(dest == r_std.substr(0, dest.size()));

    auto const stop = std::get<0>(result).begin();
    auto const n_removed = std::count(src.begin(), stop, x);

    CHECK(stop == src.begin() + dest.size() + n_removed);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/remove_copy: to longer")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const x = 'r';

    std::string dest(src.size() * 2, '*');
    auto const dest_old = dest;

    CHECK(dest.size() > src.size());

    std::string r_std;
    std::remove_copy(src.begin(), src.end(), std::back_inserter(r_std), x);

    auto result = ::sayan::remove_copy(src, dest, x);

    CHECK(dest.substr(0, r_std.size()) == r_std);
    CHECK(dest.substr(r_std.size()) == dest_old.substr(r_std.size()));

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.begin() + r_std.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/remove_copy_if")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const pred = [](char c) { return std::isupper(c); };

    std::string r_std;
    std::remove_copy_if(src.begin(), src.end(), std::back_inserter(r_std), pred);

    std::ostringstream os;
    auto result = sayan::remove_copy_if(std::istringstream(src), os, pred);

    CHECK(os.str() == r_std);

    CHECK(std::get<0>(result).empty());
    CHECK(!std::get<1>(result).empty());
}

TEST_CASE("algorithm/remove_copy_if: to shorter")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const pred = [](char c) { return std::isupper(c); };

    std::string r_std;
    std::remove_copy_if(src.begin(), src.end(), std::back_inserter(r_std), pred);

    std::string dest(r_std.size() / 2, '*');

    CHECK(dest.size() < r_std.size());

    auto result = sayan::remove_copy_if(src, dest, pred);

    CHECK(dest == r_std.substr(0, dest.size()));

    auto const stop = std::get<0>(result).begin();
    auto const n_removed = std::count_if(src.begin(), stop, pred);

    CHECK(stop == src.begin() + dest.size() + n_removed);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/remove_copy_if: to longer")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const pred = [](char c) { return std::isupper(c); };

    std::string dest(src.size() * 2, '*');
    auto const dest_old = dest;

    CHECK(dest.size() > src.size());

    std::string r_std;
    std::remove_copy_if(src.begin(), src.end(), std::back_inserter(r_std), pred);

    auto result = ::sayan::remove_copy_if(src, dest, pred);

    CHECK(dest.substr(0, r_std.size()) == r_std);
    CHECK(dest.substr(r_std.size()) == dest_old.substr(r_std.size()));

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.begin() + r_std.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/copy_if")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const pred = [](char c) { return std::islower(c); };

    std::string r_std;
    std::copy_if(src.begin(), src.end(), std::back_inserter(r_std), pred);

    std::ostringstream os;
    auto result = sayan::copy_if(std::istringstream(src), os, pred);

    CHECK(os.str() == r_std);

    CHECK(std::get<0>(result).empty());
    CHECK(!std::get<1>(result).empty());
}

TEST_CASE("algorithm/copy_if: to shorter")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const pred = [](char c) { return std::islower(c); };

    std::string r_std;
    std::copy_if(src.begin(), src.end(), std::back_inserter(r_std), pred);

    std::string dest(r_std.size() / 2, '*');

    CHECK(dest.size() < r_std.size());

    auto result = sayan::copy_if(src, dest, pred);

    CHECK(dest == r_std.substr(0, dest.size()));

    auto const stop = std::get<0>(result).begin();
    auto const n_removed = (stop - src.begin()) - std::count_if(src.begin(), stop, pred);

    CHECK(stop == src.begin() + dest.size() + n_removed);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/copy_if: to longer")
{
    std::string const src{"Bjarne Stroustrup"};
    auto const pred = [](char c) { return std::islower(c); };

    std::string dest(src.size() * 2, '*');
    auto const dest_old = dest;

    CHECK(dest.size() > src.size());

    std::string r_std;
    std::copy_if(src.begin(), src.end(), std::back_inserter(r_std), pred);

    auto result = ::sayan::copy_if(src, dest, pred);

    CHECK(dest.substr(0, r_std.size()) == r_std);
    CHECK(dest.substr(r_std.size()) == dest_old.substr(r_std.size()));

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.begin() + r_std.size());
    CHECK(std::get<1>(result).end() == dest.end());
}
