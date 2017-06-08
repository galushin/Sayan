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
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED
