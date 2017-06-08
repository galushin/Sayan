#ifndef Z_URAL_CURSOR_DEFS_HPP_INCLUDED
#define Z_URAL_CURSOR_DEFS_HPP_INCLUDED

#include <sayan/adl_tag.hpp>
#include <sayan/utility/static_const.hpp>

#include <type_traits>
#include <utility>

namespace sayan
{
inline namespace v1
{
    struct front_fn {};

    namespace
    {
        constexpr auto const & front = static_const<front_fn>;
    }

    /// @cond false
    namespace details
    {
        template <class T>
        struct is_cursor
        {
        private:
            template <class... Args>
            struct true_type_const
            {
                using type = std::true_type;
            };

            template <class... Args>
            using true_type_const_t = typename true_type_const<Args...>::type;

            template <class U>
            static auto test(U * p)
            -> true_type_const_t<decltype(p->empty()),
                                 decltype((*p)[::sayan::front]),
                                 decltype(p->drop(::sayan::front))>;

            template <class U>
            static std::false_type test(...);

        public:
            using type = decltype(is_cursor::test<T>(nullptr));
        };
    }
    // namespace details
    /// @endcond

    template <class T>
    struct is_cursor
     : ::sayan::details::is_cursor<std::decay_t<T>>::type
    {};

    template <class Cursor, class = std::enable_if_t<is_cursor<Cursor>::value>>
    auto operator*(Cursor const & cur)
    -> decltype(cur[::sayan::front])
    {
        return cur[::sayan::front];
    }

    template <class Cursor, class = std::enable_if_t<is_cursor<Cursor>::value>>
    bool operator!(Cursor const & cur)
    {
        return cur.empty();
    }

    template <class Cursor, class = std::enable_if_t<is_cursor<Cursor>::value>>
    Cursor & operator++(Cursor & cur)
    {
        cur.drop(::sayan::front);
        return cur;
    }

    template <class Cursor, class T, class = std::enable_if_t<is_cursor<Cursor>::value>>
    Cursor & operator<<(Cursor & cur, T && value)
    {
        *cur = std::forward<T>(value);
        ++ cur;
        return cur;
    }

    template <class Cursor, class = std::enable_if_t<is_cursor<Cursor>::value>>
    Cursor cursor_hook(Cursor cur, adl_tag)
    {
        return cur;
    }

    template <class T>
    struct cursor_traits
    {
        using difference_type = typename T::difference_type;
    };

    template <class T>
    using difference_type_t = typename cursor_traits<T>::difference_type;
}
//inline namespace v1
}
//namespace sayan

#endif
// Z_URAL_CURSOR_DEFS_HPP_INCLUDED
