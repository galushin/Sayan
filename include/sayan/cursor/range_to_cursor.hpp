#ifndef Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED
#define Z_SAYAN_CURSOR_CURSOR_FN_HPP_INCLUDED

// Курсоры, которые создаются автоматически
#include <sayan/cursor/istream.hpp>
#include <sayan/cursor/ostream.hpp>

#include <sayan/utility/static_const.hpp>

namespace sayan
{
inline namespace v1
{
    /// @cond false
    namespace details
    {
        template <class Range>
        using cursor_type_t = decltype(cursor(std::declval<Range&&>()));

        struct cursor_fn
        {
        public:
            template <class Range>
            constexpr
            cursor_type_t<Range>
            operator()(Range && r) const
            noexcept(noexcept(cursor(std::forward<Range>(r))))
            {
                return cursor(std::forward<Range>(r));
            }

        };
    }
    // namespace datails
    // @endcond

    template <class Range>
    using cursor_type_t = details::cursor_type_t<Range>;

    template <class Range>
    using safe_cursor_type_t = cursor_type_t<Range>;

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
