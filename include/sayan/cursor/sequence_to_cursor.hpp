#ifndef Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED
#define Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED

// Курсоры, которые создаются автоматически
#include <sayan/cursor/istream.hpp>
#include <sayan/cursor/iterator_cursor.hpp>
#include <sayan/cursor/ostream.hpp>

#include <sayan/utility/static_const.hpp>

namespace sayan
{
inline namespace v1
{
    /// @cond false
    namespace details
    {
        template <class Sequence>
        using cursor_type_t = decltype(cursor(std::declval<Sequence&&>()));

        struct cursor_fn
        {
        public:
            template <class Sequence>
            constexpr
            cursor_type_t<Sequence>
            operator()(Sequence && r) const
            noexcept(noexcept(cursor(std::forward<Sequence>(r))))
            {
                return cursor(std::forward<Sequence>(r));
            }

        };
    }
    // namespace datails
    // @endcond

    template <class Sequence>
    using cursor_type_t = details::cursor_type_t<Sequence>;

    template <class Sequence>
    using safe_cursor_type_t = cursor_type_t<Sequence>;

    namespace
    {
        constexpr auto const & cursor = static_const<details::cursor_fn>;
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED
