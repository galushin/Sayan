#ifndef Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED
#define Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED

// Курсоры, которые создаются автоматически
#include <sayan/cursor/istream.hpp>
#include <sayan/cursor/iterator_cursor.hpp>
#include <sayan/cursor/ostream.hpp>

#include <sayan/cursor/defs.hpp>
#include <sayan/cursor/check_policies.hpp>

namespace sayan
{
inline namespace v1
{
    template <class Range, class = std::enable_if_t<!is_cursor<Range>::value>>
    auto cursor_hook(Range && r, adl_tag)
    -> iterator_cursor_type<decltype(std::forward<Range>(r).begin()),
                            decltype(std::forward<Range>(r).end())>
    {
        using Cursor = iterator_cursor_type<decltype(std::forward<Range>(r).begin()),
                                            decltype(std::forward<Range>(r).end())>;
        return Cursor{std::forward<Range>(r).begin(), std::forward<Range>(r).end()};
    }

    template <class Sequence>
    auto cursor(Sequence && seq)
    {
        using sayan::cursor_hook;
        return cursor_hook(std::forward<Sequence>(seq), adl_tag{});
    }

    template <class Sequence>
    using cursor_type_t = decltype(cursor(std::declval<Sequence&&>()));

    template <class Sequence>
    using safe_cursor_type_t = cursor_type_t<Sequence>;
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED
