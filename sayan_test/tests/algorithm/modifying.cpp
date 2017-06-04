#include <sayan/algorithm.hpp>

#include <sayan/cursor/back_inserter.hpp>

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

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK(std::get<0>(result).begin() == src.begin() + dest.size());
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
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

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK((std::get<0>(result).begin() - src.begin()) == dest.size() + to_erase);
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK(std::get<0>(result).begin() == src.end());
    CHECK(std::get<0>(result).end() == src.end());

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

    CHECK((std::get<0>(result).begin() - src.begin()) == dest_std.size() + to_erase);
    CHECK(std::get<0>(result).end() == src.end());

    CHECK(std::get<1>(result).begin() == dest.end());
    CHECK(std::get<1>(result).end() == dest.end());
}

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

TEST_CASE("algorithm/merge")
{
    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end());
    std::sort(src_2.begin(), src_2.end());

    std::string out_std;
    std::merge(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std));

    std::ostringstream os;
    sayan::merge(std::istringstream(src_1), std::istringstream(src_2), os);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/merge: custom predicate")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::merge(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::ostringstream os;
    sayan::merge(std::istringstream(src_1), std::istringstream(src_2), os, pred);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/merge: custom predicate, first input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::merge(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  out_std.begin(), pred);

    auto const r = sayan::merge(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/merge: custom predicate, second input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::merge(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  out_std.begin(), pred);

    auto const r = sayan::merge(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/merge: custom predicate, not enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::merge(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::string out(out_std.size() / 2, '*');
    auto const r = sayan::merge(src_1, src_2, out, pred);

    CHECK(out == out_std.substr(0, out.size()));

    CHECK(std::get<2>(r).begin() == out.end());
    CHECK(std::get<2>(r).end() == out.end());

    std::string out_2;
    std::merge(src_1.begin(), std::get<0>(r).begin(), src_2.begin(), std::get<1>(r).begin(),
               std::back_inserter(out_2), pred);

    CHECK(out == out_2);
}

TEST_CASE("algorithm/set_union")
{
    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end());
    std::sort(src_2.begin(), src_2.end());

    std::string out_std;
    std::set_union(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std));

    std::ostringstream os;
    sayan::set_union(std::istringstream(src_1), std::istringstream(src_2), os);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_union: custom predicate")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_union(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::ostringstream os;
    sayan::set_union(std::istringstream(src_1), std::istringstream(src_2), os, pred);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_union: custom predicate, first input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_union(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  out_std.begin(), pred);

    auto const r = sayan::set_union(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_union: custom predicate, second input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_union(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  out_std.begin(), pred);

    auto const r = sayan::set_union(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_union: custom predicate, not enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_union(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::string out(out_std.size() / 2, '*');
    auto const r = sayan::set_union(src_1, src_2, out, pred);

    CHECK(out == out_std.substr(0, out.size()));

    CHECK(std::get<2>(r).begin() == out.end());
    CHECK(std::get<2>(r).end() == out.end());

    std::string out_2;
    std::set_union(src_1.begin(), std::get<0>(r).begin(), src_2.begin(), std::get<1>(r).begin(),
               std::back_inserter(out_2), pred);

    CHECK(out == out_2);
}

TEST_CASE("algorithm/set_intersection")
{
    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end());
    std::sort(src_2.begin(), src_2.end());

    std::string out_std;
    std::set_intersection(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std));

    std::ostringstream os;
    sayan::set_intersection(std::istringstream(src_1), std::istringstream(src_2), os);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_intersection: custom predicate")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_intersection(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::ostringstream os;
    sayan::set_intersection(std::istringstream(src_1), std::istringstream(src_2), os, pred);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_intersection: custom predicate, first input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_intersection(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                             out_std.begin(), pred);

    auto const r = sayan::set_intersection(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_intersection: custom predicate, second input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_intersection(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  out_std.begin(), pred);

    auto const r = sayan::set_intersection(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_intersection: custom predicate, not enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_intersection(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::string out(out_std.size() / 2, '*');
    auto const r = sayan::set_intersection(src_1, src_2, out, pred);

    CHECK(out == out_std.substr(0, out.size()));

    CHECK(std::get<2>(r).begin() == out.end());
    CHECK(std::get<2>(r).end() == out.end());

    std::string out_2;
    std::set_intersection(src_1.begin(), std::get<0>(r).begin(),
                          src_2.begin(), std::get<1>(r).begin(),
                          std::back_inserter(out_2), pred);

    CHECK(out == out_2);
}

TEST_CASE("algorithm/set_difference")
{
    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end());
    std::sort(src_2.begin(), src_2.end());

    std::string out_std;
    std::set_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std));

    std::ostringstream os;
    sayan::set_difference(std::istringstream(src_1), std::istringstream(src_2), os);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_difference: custom predicate")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::ostringstream os;
    sayan::set_difference(std::istringstream(src_1), std::istringstream(src_2), os, pred);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_difference: custom predicate, first input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                             out_std.begin(), pred);

    auto const r = sayan::set_difference(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_difference: custom predicate, second input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  out_std.begin(), pred);

    auto const r = sayan::set_difference(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_difference: custom predicate, not enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::string out(out_std.size() / 2, '*');
    auto const r = sayan::set_difference(src_1, src_2, out, pred);

    CHECK(out == out_std.substr(0, out.size()));

    CHECK(std::get<2>(r).begin() == out.end());
    CHECK(std::get<2>(r).end() == out.end());

    std::string out_2;
    std::set_difference(src_1.begin(), std::get<0>(r).begin(),
                        src_2.begin(), std::get<1>(r).begin(),
                        std::back_inserter(out_2), pred);

    CHECK(out == out_2);
}

TEST_CASE("algorithm/set_symmetric_difference")
{
    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end());
    std::sort(src_2.begin(), src_2.end());

    std::string out_std;
    std::set_symmetric_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  std::back_inserter(out_std));

    std::ostringstream os;
    sayan::set_symmetric_difference(std::istringstream(src_1), std::istringstream(src_2), os);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_symmetric_difference: custom predicate")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_symmetric_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::ostringstream os;
    sayan::set_symmetric_difference(std::istringstream(src_1), std::istringstream(src_2), os, pred);

    CHECK(os.str() == out_std);
}

TEST_CASE("algorithm/set_symmetric_difference: custom predicate, first input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"1248"};
    std::string src_2{"12358"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_symmetric_difference(src_1.begin(), src_1.end(),
                                                     src_2.begin(), src_2.end(),
                                                     out_std.begin(), pred);

    auto const r = sayan::set_symmetric_difference(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_symmetric_difference: custom predicate, second input is shorter, enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std(src_1.size() + src_2.size() + 10, '*');
    auto out = out_std;

    auto const r_std = std::set_symmetric_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                                  out_std.begin(), pred);

    auto const r = sayan::set_symmetric_difference(src_1, src_2, out, pred);

    CHECK(out == out_std);

    CHECK(std::get<0>(r).begin() == src_1.end());
    CHECK(std::get<0>(r).end() == src_1.end());

    CHECK(std::get<1>(r).begin() == src_2.end());
    CHECK(std::get<1>(r).end() == src_2.end());

    CHECK((std::get<2>(r).begin() - out.begin()) == r_std - out_std.begin());
    CHECK(std::get<2>(r).end() == out.end());
}

TEST_CASE("algorithm/set_symmetric_difference: custom predicate, not enough space")
{
    auto const pred = std::greater<>{};

    std::string src_1{"12358"};
    std::string src_2{"1248"};

    std::sort(src_1.begin(), src_1.end(), pred);
    std::sort(src_2.begin(), src_2.end(), pred);

    std::string out_std;
    std::set_symmetric_difference(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
               std::back_inserter(out_std), pred);

    std::string out(out_std.size() / 2, '*');
    auto const r = sayan::set_symmetric_difference(src_1, src_2, out, pred);

    CHECK(out == out_std.substr(0, out.size()));

    CHECK(std::get<2>(r).begin() == out.end());
    CHECK(std::get<2>(r).end() == out.end());

    std::string out_2;
    std::set_symmetric_difference(src_1.begin(), std::get<0>(r).begin(),
                        src_2.begin(), std::get<1>(r).begin(),
                        std::back_inserter(out_2), pred);

    CHECK(out == out_2);
}
