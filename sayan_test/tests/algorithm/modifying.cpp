#include <sayan/algorithm.hpp>

#include <sayan/cursor/back_inserter.hpp>

#include <algorithm>

#include <forward_list>
#include <catch/catch.hpp>

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

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).empty());
    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
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

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.begin() + src.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("cursors/move")
{
    std::vector<int> const ns{1,2,3,5,8,13};
    std::vector<std::unique_ptr<int>> src;
    for(auto const & n : ns)
    {
        src.emplace_back(std::make_unique<int>(n));
    }

    std::vector<std::unique_ptr<int>> dest;
    sayan::move(src, sayan::back_inserter(dest));

    CHECK(std::all_of(src.begin(), src.end(),
                      [](std::unique_ptr<int> const & p) { return !p;}));

    std::vector<int> unptr;
    for(auto const & p : dest)
    {
        CHECK(!!p);

        unptr.emplace_back(*p);
    }

    CHECK(unptr == ns);
}

TEST_CASE("cursors/move: to longer")
{
    // Подготовка
    std::vector<int> const ns{1,2,3,5,8,13};
    std::vector<std::unique_ptr<int>> src;
    for(auto const & n : ns)
    {
        src.emplace_back(std::make_unique<int>(n));
    }

    std::vector<std::unique_ptr<int>> dest(src.size() * 2);

    CHECK(src.size() < dest.size());

    // Выполнение алгоритма
    auto const result = sayan::move(src, dest);

    auto const dest_rest = std::get<1>(result);

    // Проверки
    auto const has_value = [](std::unique_ptr<int> const & p) { return !!p;};
    CHECK(std::none_of(src.begin(), src.end(), has_value));

    CHECK(std::all_of(dest.begin(), dest_rest.begin(), has_value));
    CHECK(std::none_of(dest_rest.begin(), dest_rest.end(), has_value));

    std::vector<int> dest_ns;
    for(auto p = dest.begin(); p != dest_rest.begin(); ++ p)
    {
        dest_ns.push_back(**p);
    }

    CHECK(dest_ns == ns);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.begin() + src.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("cursors/move: to shorter")
{
    // Подготовка
    std::vector<int> const ns{1,2,3,5,8,13,21,34};
    std::vector<std::unique_ptr<int>> src;
    for(auto const & n : ns)
    {
        src.emplace_back(std::make_unique<int>(n));
    }

    std::vector<std::unique_ptr<int>> dest(src.size() / 2);

    CHECK(src.size() > dest.size());

    // Выполнение алгоритма
    auto const result = sayan::move(src, dest);

    auto const src_rest = std::get<0>(result);

    // Проверки
    auto const has_value = [](std::unique_ptr<int> const & p) { return !!p;};
    CHECK(std::none_of(src.begin(), src_rest.begin(), has_value));
    CHECK(std::all_of(src_rest.begin(), src.end(), has_value));

    CHECK(std::all_of(dest.begin(), dest.end(), has_value));

    std::vector<int> dest_ns;
    for(auto p = dest.begin(); p != dest.end(); ++ p)
    {
        dest_ns.push_back(**p);
    }

    CHECK(dest_ns == std::vector<int>(ns.begin(), ns.begin() + dest.size()));

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.end());
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

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).empty());
    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
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

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
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

    CHECK(std::get<0>(result).traversed_begin() == in1.begin());
    CHECK(std::get<0>(result).begin() == in1.end());
    CHECK(std::get<0>(result).end() == in1.end());

    CHECK(std::get<1>(result).traversed_begin() == in2.begin());
    CHECK(std::get<1>(result).begin() == in2.begin() + n);
    CHECK(std::get<1>(result).end() == in2.end());

    CHECK(std::get<2>(result).traversed_begin() == out.begin());
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

    CHECK(std::get<0>(result).traversed_begin() == in1.begin());
    CHECK(std::get<0>(result).begin() == in1.begin() + n);
    CHECK(std::get<0>(result).end() == in1.end());

    CHECK(std::get<1>(result).traversed_begin() == in2.begin());
    CHECK(std::get<1>(result).begin() == in2.end());
    CHECK(std::get<1>(result).end() == in2.end());

    CHECK(std::get<2>(result).traversed_begin() == out.begin());
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

    CHECK(std::get<0>(result).traversed_begin() == in1.begin());
    CHECK(std::get<0>(result).begin() == in1.begin() + n);
    CHECK(std::get<0>(result).end() == in1.end());

    CHECK(std::get<1>(result).traversed_begin() == in2.begin());
    CHECK(std::get<1>(result).begin() == in2.begin() + n);
    CHECK(std::get<1>(result).end() == in2.end());

    CHECK(std::get<2>(result).traversed_begin() == out.begin());
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

TEST_CASE("algorithm/remove")
{
    std::forward_list<int> xs_std{1, 2, -1, 3, -1, 4, - 1, 5, 6};
    auto xs = xs_std;

    auto const value = int{-1};

    auto r_std = std::remove(xs_std.begin(), xs_std.end(), value);
    std::vector<int> xs_std_head(xs_std.begin(), r_std);

    auto r = sayan::remove(xs, value);
    std::vector<int> xs_head(xs.begin(), r.begin());

    CHECK(xs_head == xs_std_head);

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(std::distance(xs.begin(), r.begin()) == std::distance(xs_std.begin(), r_std));
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/remove_if")
{
    std::forward_list<int> xs_std{1, 2, -1, 3, -2, 0, - 5, 5, 6};
    auto xs = xs_std;

    auto const pred = [](auto x) { return x < 0; };

    auto r_std = std::remove_if(xs_std.begin(), xs_std.end(), pred);
    std::vector<int> xs_std_head(xs_std.begin(), r_std);

    auto r = sayan::remove_if(xs, pred);

    std::vector<int> xs_head(xs.begin(), r.begin());

    CHECK(xs_head == xs_std_head);

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(std::distance(xs.begin(), r.begin()) == std::distance(xs_std.begin(), r_std));
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/remove_if: nothing to remove")
{
    std::forward_list<int> xs{1, 2, 3, 0, 5, 5, 6};
    auto const xs_old = xs;

    auto const pred = [](auto x) { return x < 0; };

    CHECK(::sayan::none_of(xs, pred));

    auto r = sayan::remove_if(xs, pred);

    CHECK(xs == xs_old);

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(r.begin() == xs.end());
    CHECK(r.end() == xs.end());
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

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(stop == src.begin() + dest.size() + n_removed);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
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

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.begin() + r_std.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/replace")
{
    std::forward_list<int> xs_std{1,2,3,2,5,2};
    auto xs = xs_std;

    auto const old_value = int{2};
    auto const new_value = int{22};

    std::replace(xs_std.begin(), xs_std.end(), old_value, new_value);

    auto const result = sayan::replace(xs, old_value, new_value);

    CHECK(xs == xs_std);

    CHECK(result.traversed_begin() == xs.begin());
    CHECK(result.begin() == xs.end());
    CHECK(result.end() == xs.end());
}

TEST_CASE("algorithm/replace_if")
{
    std::forward_list<int> xs_std{1,2,3,4,5,6};
    auto xs = xs_std;

    auto const pred = [](auto const & x) { return x % 2 == 0; };
    auto const new_value = int{-1};

    std::replace_if(xs_std.begin(), xs_std.end(), pred, new_value);

    auto const result = sayan::replace_if(xs, pred, new_value);

    CHECK(xs == xs_std);

    CHECK(result.traversed_begin() == xs.begin());
    CHECK(result.begin() == xs.end());
    CHECK(result.end() == xs.end());
}

TEST_CASE("algorithm/replace_copy")
{
    std::string const src{"Human readable name"};

    auto const old_value = ' ';
    auto const new_value = '_';

    std::string r_std;
    std::replace_copy(src.begin(), src.end(), std::back_inserter(r_std),
                      old_value, new_value);

    std::ostringstream os;
    sayan::replace_copy(std::istringstream(src), os, old_value, new_value);

    CHECK(os.str() == r_std);
}

TEST_CASE("algorithm/replace_copy: to longer")
{
    std::string const src{"Human readable name"};

    auto const old_value = ' ';
    auto const new_value = '_';

    std::string dest_std(src.size() * 2, '*');
    auto dest = dest_std;

    CHECK(dest.size() > src.size());

    // Выполнение
    std::replace_copy(src.begin(), src.end(), dest_std.begin(),
                      old_value, new_value);

    auto result = sayan::replace_copy(src, dest, old_value, new_value);

    // Проверки
    CHECK(dest == dest_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.begin() + src.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/replace_copy: to shorter")
{
    std::string const src{"Human readable name"};

    auto const old_value = ' ';
    auto const new_value = '_';

    std::string dest(src.size() / 2, '*');
    std::string dest_std;

    CHECK(dest.size() < src.size());

    // Выполнение
    std::replace_copy(src.begin(), src.begin() + dest.size(),
                      std::back_inserter(dest_std), old_value, new_value);

    auto result = sayan::replace_copy(src, dest, old_value, new_value);

    // Проверки
    CHECK(dest == dest_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/replace_copy_if")
{
    std::string const src{"Human readable\tname"};

    auto const new_value = '_';
    auto pred = [](char x) { return std::isspace(x); };

    std::string r_std;
    std::replace_copy_if(src.begin(), src.end(), std::back_inserter(r_std),
                      pred, new_value);

    std::ostringstream os;
    sayan::replace_copy_if(std::istringstream(src), os, pred, new_value);

    CHECK(os.str() == r_std);
}

TEST_CASE("algorithm/replace_copy_if: to longer")
{
    std::string const src{"Human readable\tname"};

    auto const new_value = '_';
    auto pred = [](char x) { return std::isspace(x); };

    std::string dest_std(src.size() * 2, '*');
    auto dest = dest_std;

    CHECK(dest.size() > src.size());

    // Выполнение
    std::replace_copy_if(src.begin(), src.end(), dest_std.begin(),
                      pred, new_value);

    auto result = sayan::replace_copy_if(src, dest, pred, new_value);

    // Проверки
    CHECK(dest == dest_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.begin() + src.size());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/replace_copy_if: to shorter")
{
    std::string const src{"Human readable\tname"};

    auto const new_value = '_';
    auto pred = [](char x) { return std::isspace(x); };

    std::string dest(src.size() / 2, '*');
    std::string dest_std;

    CHECK(dest.size() < src.size());

    // Выполнение
    std::replace_copy_if(src.begin(), src.begin() + dest.size(),
                      std::back_inserter(dest_std), pred, new_value);

    auto result = sayan::replace_copy_if(src, dest, pred, new_value);

    // Проверки
    CHECK(dest == dest_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/unique")
{
    std::forward_list<int> xs_std{1, 2, 2, 3, 3, 4, 5, 5, 6, 2, 3, 5};
    auto xs = xs_std;

    auto const r_std = std::unique(xs_std.begin(), xs_std.end());
    auto const r = sayan::unique(xs);

    std::vector<int> out_std(xs_std.begin(), r_std);
    std::vector<int> out(xs.begin(), r.begin());

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(out == out_std);
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/unique: custom predicate")
{
    std::string const src("common string \t with   repeating  \n\n\n\n  spaces");
    std::forward_list<char> xs_std(src.begin(), src.end());
    auto xs = xs_std;
    auto const pred = [](char x, char y) { return std::isspace(x) && std::isspace(y) && x == y; };

    auto const r_std = std::unique(xs_std.begin(), xs_std.end(), pred);
    auto const r = sayan::unique(xs, pred);

    std::string out_std(xs_std.begin(), r_std);
    std::string out(xs.begin(), r.begin());

    CHECK(r.traversed_begin() == xs.begin());
    CHECK(out == out_std);
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/unique_copy")
{
    std::string const src = []()
    {
        std::string s{"Line   with   many     spaces"};
        std::sort(s.begin(), s.end());
        return s;
    }();

    std::string dest_std;
    std::unique_copy(src.begin(), src.end(), std::back_inserter(dest_std));

    std::ostringstream os;
    sayan::unique_copy(std::istringstream(src), os);

    CHECK(os.str() == dest_std);
}

TEST_CASE("algorithm/unique_copy: to longer")
{
    std::string const src = []()
    {
        std::string s{"Line   with   many     spaces"};
        std::sort(s.begin(), s.end());
        return s;
    }();

    std::string dest_std(2*src.size(), '*');
    auto stop_std = std::unique_copy(src.begin(), src.end(), dest_std.begin());

    std::string dest(2*src.size(), '*');
    CHECK(dest.size() > src.size());

    auto result = sayan::unique_copy(src, dest);

    CHECK(dest == dest_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.begin() + (stop_std - dest_std.begin()));
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/unique_copy: to shorter")
{
    std::string const src = []()
    {
        std::string s{"Line   with   many     spaces"};
        std::sort(s.begin(), s.end());
        return s;
    }();

    // std
    std::string dest_std;
    std::unique_copy(src.begin(), src.end(),
                     std::back_inserter(dest_std));

    // sayan
    std::string dest(dest_std.size()/2, '*');
    CHECK(dest.size() < dest_std.size());

    auto result = sayan::unique_copy(src, dest);

    auto to_erase = intmax_t{0};
    for(auto i = src.begin(); i != std::get<0>(result).begin(); ++ i)
    {
        to_erase += (*i == *(i - 1));
    }

    // Проверки
    CHECK(dest == dest_std.substr(0, dest.size()));

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK((std::get<0>(result).begin() - src.begin()) == dest.size() + to_erase);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/unique_copy: custom predicate")
{
    std::string const src{"Line   with   many     spaces"};

    auto const pred = [](char x, char y) { return std::isspace(x) && std::isspace(y);};

    std::string dest_std;
    std::unique_copy(src.begin(), src.end(), std::back_inserter(dest_std), pred);

    std::ostringstream os;
    sayan::unique_copy(std::istringstream(src), os, pred);

    CHECK(os.str() == dest_std);
}

TEST_CASE("algorithm/unique_copy: custom predicate, to longer")
{
    std::string const src{"Line   with   many     spaces"};

    auto const pred = [](char x, char y) { return std::isspace(x) && std::isspace(y);};

    std::string dest_std(2*src.size(), '*');
    auto stop_std = std::unique_copy(src.begin(), src.end(), dest_std.begin(), pred);

    std::string dest(2*src.size(), '*');
    CHECK(dest.size() > src.size());

    auto result = sayan::unique_copy(src, dest, pred);

    CHECK(dest == dest_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.begin() + (stop_std - dest_std.begin()));
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/unique_copy: custom predicate, to shorter")
{
    std::string const src{"Line    with   many     spaces"};

    auto const pred = [](char x, char y) { return std::isspace(x) && std::isspace(y);};

    // sayan
    std::string dest(src.size()/2, '*');
    CHECK(dest.size() < src.size());

    auto result = sayan::unique_copy(src, dest, pred);

    auto to_erase = intmax_t{0};

    for(auto i = src.begin(); i != std::get<0>(result).begin(); ++ i)
    {
        to_erase += !(pred(*i, *(i - 1)) == false);
    }

    // std
    std::string dest_std;
    std::unique_copy(src.begin(), std::get<0>(result).begin(),
                     std::back_inserter(dest_std), pred);

    // Проверки
    CHECK(dest == dest_std);

    CHECK(std::get<0>(result).traversed_begin() == src.begin());
    CHECK((std::get<0>(result).begin() - src.begin()) == dest_std.size() + to_erase);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).traversed_begin() == dest.begin());
    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

TEST_CASE("algorithm/swap_ranges")
{
    std::forward_list<int> xs1{1,3,5,7};
    std::forward_list<int> xs2{2,4,6,8};

    auto const xs1_old = xs1;
    auto const xs2_old = xs2;

    auto const r = sayan::swap_ranges(xs1, xs2);

    CHECK(std::get<0>(r).traversed_begin() == xs1.begin());
    CHECK(std::get<0>(r).begin() == xs1.end());
    CHECK(std::get<0>(r).end() == xs1.end());

    CHECK(std::get<1>(r).traversed_begin() == xs2.begin());
    CHECK(std::get<1>(r).begin() == xs2.end());
    CHECK(std::get<1>(r).end() == xs2.end());

    CHECK(xs1 == xs2_old);
    CHECK(xs2 == xs1_old);
}

TEST_CASE("algorithm/swap_ranges: different sizes")
{
    std::vector<int> xs1{1,3,5,7,9};
    std::vector<int> xs2{2,4,6,8};

    auto const xs1_old = xs1;
    auto const xs2_old = xs2;

    auto const n = std::min(xs1.size(), xs2.size());

    auto const r = sayan::swap_ranges(xs1, xs2);

    CHECK(std::get<0>(r).traversed_begin() == xs1.begin());
    CHECK(std::get<0>(r).begin() == xs1.begin() + n);
    CHECK(std::get<0>(r).end() == xs1.end());

    CHECK(std::get<1>(r).traversed_begin() == xs2.begin());
    CHECK(std::get<1>(r).begin() == xs2.begin() + n);
    CHECK(std::get<1>(r).end() == xs2.end());

    CHECK(::std::equal(xs1.begin(), xs1.begin() + n, xs2_old.begin(), xs2_old.begin() + n));
    CHECK(::std::equal(xs2.begin(), xs2.begin() + n, xs1_old.begin(), xs1_old.begin() + n));
    CHECK(::std::equal(xs1.begin() + n, xs1.end(), xs1_old.begin() + n, xs1_old.end()));
    CHECK(::std::equal(xs2.begin() + n, xs2.end(), xs2_old.begin() + n, xs2_old.end()));

    sayan::swap_ranges(xs2, xs1);

    CHECK(xs1 == xs1_old);
    CHECK(xs2 == xs2_old);
}

TEST_CASE("algorithm/rotate: empty parts")
{
    std::forward_list<int> xs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    auto const xs_old = xs;

    auto const cur = ::sayan::cursor(xs);

    auto const r1 = sayan::rotate(cur);

    CHECK(xs == xs_old);
    CHECK(r1.begin() == xs.end());

    auto c = cur;
    c.exhaust(sayan::front);

    auto const r2 = sayan::rotate(c);

    CHECK(xs == xs_old);
    CHECK(r2.begin() == xs.begin());
}

TEST_CASE("algorithm/rotate: equal parts")
{
    std::vector<int> xs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto xs_sayan = xs;

    auto const n = std::distance(xs.begin(), xs.end());
    auto const d = n / 2;
    CHECK(d == n - d);

    // std
    auto const r_std = std::rotate(xs.begin(), std::next(xs.begin(), d), xs.end());

    // sayan
    auto const r = ::sayan::rotate(::sayan::next(::sayan::cursor(xs_sayan), d));

    // Проверки
    CHECK(xs_sayan == xs);

    CHECK((r.traversed(sayan::front).begin() - xs_sayan.begin()) == 0);
    CHECK((r.begin() - xs_sayan.begin()) == (r_std - xs.begin()));
    CHECK((xs_sayan.end() - r.end()) == 0);
}

TEST_CASE("algorithm/rotate: first shorter")
{
    // Настройки
    std::vector<int> xs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    auto xs_sayan = xs;

    auto const n = std::distance(xs.begin(), xs.end());
    auto const d = n / 3;

    // std
    auto const r_std = std::rotate(xs.begin(), std::next(xs.begin(), d), xs.end());

    // sayan
    auto cur = ::sayan::cursor(xs_sayan);
    ::sayan::advance(cur, d);

    auto const r = ::sayan::rotate(cur);

    // Проверки
    CHECK(xs_sayan == xs);

    CHECK((r.traversed(sayan::front).begin() - xs_sayan.begin()) == 0);
    CHECK((r.begin() - xs_sayan.begin()) == (r_std - xs.begin()));
    CHECK((xs_sayan.end() - r.end()) == 0);
}

TEST_CASE("algorithm/rotate: first longer")
{
    // Настройки
    std::vector<int> xs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    auto xs_sayan = xs;

    auto const n = std::distance(xs.begin(), xs.end());
    auto const d = n - n / 3;

    // std
    auto const r_std = std::rotate(xs.begin(), std::next(xs.begin(), d), xs.end());

    // sayan
    auto cur = ::sayan::cursor(xs_sayan);
    ::sayan::advance(cur, d);

    auto const r = ::sayan::rotate(cur);

    // Проверки
    CHECK(xs_sayan == xs);

    CHECK((r.traversed(sayan::front).begin() - xs_sayan.begin()) == 0);
    CHECK((r.begin() - xs_sayan.begin()) == (r_std - xs.begin()));
    CHECK((xs_sayan.end() - r.end()) == 0);
}

TEST_CASE("algorithm/rotate_copy")
{
    // Настройки
    std::forward_list<int> const xs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    auto const n = std::distance(xs.begin(), xs.end());
    auto const d = n / 3;

    // std
    std::vector<int> out_std;
    std::rotate_copy(xs.begin(), std::next(xs.begin(), d), xs.end(),
                     std::back_inserter(out_std));

    // sayan
    auto cur = ::sayan::cursor(xs);
    ::sayan::advance(cur, d);

    std::vector<int> out_sayan;
    auto const r = ::sayan::rotate_copy(cur, sayan::back_inserter(out_sayan)).first;

    // Проверки
    CHECK(out_sayan == out_std);

    CHECK(r.traversed(sayan::front).begin() == xs.begin());
    CHECK(r.begin() == xs.end());
    CHECK(r.end() == xs.end());
}

TEST_CASE("algorithm/rotate_copy: to longer")
{
    // Настройки
    std::forward_list<int> const xs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    auto const n = std::distance(xs.begin(), xs.end());
    auto const d = n / 3;

    std::vector<int> dest_std(2*n, -1);
    auto dest_sayan = dest_std;

    // std
    auto const r_std = std::rotate_copy(xs.begin(), std::next(xs.begin(), d), xs.end(),
                                        dest_std.begin());

    // sayan
    auto cur = ::sayan::cursor(xs);
    ::sayan::advance(cur, d);
    auto const r = ::sayan::rotate_copy(cur, dest_sayan);

    // Проверки
    CHECK(dest_sayan == dest_std);

    CHECK(r.first.traversed(sayan::front).begin() == xs.begin());
    CHECK(r.first.begin() == xs.end());
    CHECK(r.first.end() == xs.end());

    CHECK(r.second.traversed(sayan::front).begin() == dest_sayan.begin());
    CHECK((r.second.begin() - dest_sayan.begin()) == (r_std - dest_std.begin()));
    CHECK(r.second.end() == dest_sayan.end());
}
