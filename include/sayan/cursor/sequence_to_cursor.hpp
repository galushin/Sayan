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
    template <class Range, class = std::enable_if_t<!is_input_cursor<Range>::value>>
    auto cursor_hook(Range && r, adl_tag)
    -> iterator_cursor_type<decltype(std::forward<Range>(r).begin()),
                            decltype(std::forward<Range>(r).end())>
    {
        using Cursor = iterator_cursor_type<decltype(std::forward<Range>(r).begin()),
                                            decltype(std::forward<Range>(r).end())>;
        return Cursor{std::forward<Range>(r).begin(), std::forward<Range>(r).end()};
    }

    namespace details
    {
        using sayan::cursor_hook;

        struct cursor_fn
        {
            template <class Sequence>
            auto operator()(Sequence && seq) const
            noexcept(noexcept(cursor_hook(std::forward<Sequence>(seq), adl_tag{})))
            -> decltype(cursor_hook(std::forward<Sequence>(seq), adl_tag{}))
            {
                return cursor_hook(std::forward<Sequence>(seq), adl_tag{});
            }
        };
    }
    // namespace details

    namespace
    {
        constexpr auto const & cursor = static_const<details::cursor_fn>;
    }

    template <class Sequence>
    using cursor_type_t = decltype(cursor(std::declval<Sequence>()));

    template <class Sequence>
    using safe_cursor_type_t = cursor_type_t<Sequence>;

    //@{
    template <class Sequence>
    auto cursor_fwd(std::remove_reference_t<Sequence> & seq)
    noexcept(noexcept(::sayan::cursor(std::forward<Sequence>(seq))))
    -> decltype(::sayan::cursor(std::forward<Sequence>(seq)))
    {
        return ::sayan::cursor(std::forward<Sequence>(seq));
    }

    template <class Sequence>
    auto cursor_fwd(std::remove_reference_t<Sequence> && seq)
    noexcept(noexcept(::sayan::cursor(std::forward<Sequence>(seq))))
    -> decltype(::sayan::cursor(std::forward<Sequence>(seq)))
    {
        return ::sayan::cursor(std::forward<Sequence>(seq));
    }
    //@}

    struct size_fn
    {
        template <class InputSequence>
        difference_type_t<cursor_type_t<InputSequence>>
        operator()(InputSequence && seq) const
        {
            auto n = difference_type_t<cursor_type_t<InputSequence>>{0};

            for(auto cur = ::sayan::cursor_fwd<InputSequence>(seq); !!cur; ++ cur)
            {
                ++ n;
            }
            return n;
        }
    };

    struct advance_fn
    {
        template <class InputCursor>
        void operator()(InputCursor & cur, difference_type_t<InputCursor> n) const
        {
            for(; n > 0; -- n)
            {
                ++ cur;
            }
        }
    };

    struct cursor_swap_fn
    {
        template <class Cursor1, class Cursor2>
        void operator()(Cursor1 const & c1, Cursor2 const & c2) const
        {
            using std::swap;
            swap(*c1, *c2);
        }

        template <class Cursor1, class Cursor2, class P1, class P2>
        void operator()(Cursor1 const & c1, Cursor2 const & c2, P1 p1, P2 p2) const
        {
            using std::swap;
            swap(c1[p1], c2[p2]);
        }
    };

    struct next_fn
    {
        template <class Cursor>
        Cursor
        operator()(Cursor c, difference_type_t<Cursor> n = difference_type_t<Cursor>{1}) const
        {
            ::sayan::advance_fn{}(c, n);
            return c;
        }
    };

    struct cursor_from_parts_fn
    {
        template <class Cursor>
        Cursor operator()(Cursor pre, Cursor stem) const
        {
            pre = ::sayan::cursor(std::move(pre));
            pre.exhaust(::sayan::front);
            pre.splice(::sayan::cursor(std::move(stem)));
            return pre;
        }
    };

    namespace
    {
        constexpr auto const & size = static_const<size_fn>;
        constexpr auto const & advance = static_const<advance_fn>;
        constexpr auto const & next = static_const<next_fn>;
        constexpr auto const & cursor_swap = static_const<cursor_swap_fn>;
        constexpr auto const & cursor_from_parts = static_const<cursor_from_parts_fn>;
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED
