#include <sayan/algorithm.hpp>

#include <catch/catch.hpp>

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
