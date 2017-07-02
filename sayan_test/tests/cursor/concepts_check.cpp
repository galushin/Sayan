#include <sayan/cursor/sequence_to_cursor.hpp>

#include "../../simple_test.hpp"
#include <catch/catch.hpp>

TEST_CASE("cursors/concepts")
{
    std::istringstream is_src;
    std::ostringstream os_src;
    std::forward_list<int> fwd_src;
    std::list<int> bidir_src;
    std::vector<int> ra_src;

    auto is       = ::sayan::cursor(is_src);
    auto os       = ::sayan::cursor(os_src);
    auto fwd      = ::sayan::cursor(fwd_src);
    auto bidir    = ::sayan::cursor(bidir_src);
    auto ra       = ::sayan::cursor(ra_src);

    // Курсор ввода
    static_assert(sayan::is_input_cursor<decltype(is)>::value, "");
    static_assert(!sayan::is_input_cursor<decltype(os)>::value, "");
    static_assert(sayan::is_input_cursor<decltype(fwd)>::value, "");
    static_assert(sayan::is_input_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_input_cursor<decltype(ra)>::value, "");

    // Курсор вывода
    static_assert(!sayan::is_output_cursor<decltype(is), int>::value, "");
    static_assert(sayan::is_output_cursor<decltype(os), int>::value, "");
    static_assert(sayan::is_output_cursor<decltype(fwd), int>::value, "");
    static_assert(sayan::is_output_cursor<decltype(bidir), int>::value, "");
    static_assert(sayan::is_output_cursor<decltype(ra), int>::value, "");

    static_assert(!sayan::is_output_cursor<decltype(fwd), void*>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(bidir), void*>::value, "");
    static_assert(!sayan::is_output_cursor<decltype(ra), void*>::value, "");

    // Прямой курсор
    static_assert(!sayan::is_forward_cursor<decltype(is)>::value, "");
    static_assert(!sayan::is_forward_cursor<decltype(os)>::value, "");
    static_assert(sayan::is_forward_cursor<decltype(fwd)>::value, "");
    static_assert(sayan::is_forward_cursor<decltype(bidir)>::value, "");
    static_assert(sayan::is_forward_cursor<decltype(ra)>::value, "");

    // @todo Проверка BidirectionalCursor
    // @todo Проверка RandomAccessCursor
}
