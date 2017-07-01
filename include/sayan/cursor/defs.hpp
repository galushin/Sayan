#ifndef Z_URAL_CURSOR_DEFS_HPP_INCLUDED
#define Z_URAL_CURSOR_DEFS_HPP_INCLUDED

#include <sayan/adl_tag.hpp>
#include <sayan/utility/static_const.hpp>
#include <sayan/utility/as_const.hpp>
#include <sayan/type_traits.hpp>

#include <utility>

namespace sayan
{
inline namespace v1
{
    struct front_fn {};
    struct back_fn {};

    namespace
    {
        constexpr auto const & front = static_const<front_fn>;
        constexpr auto const & back = static_const<back_fn>;
    }

    namespace concepts
    {
        /// @cond false
        namespace details
        {
            template <class... Ts>
            struct valid_types
             : declare_type<int>
            {};
        }
        // namespace details
        /// @endcond

        template <class... Ts>
        using valid_types = details::valid_types<Ts...>;

        template <class C, class... Args>
        using is_body_satisfied_impl = decltype(std::declval<C>().requires(std::declval<Args>()...));

        template <class Concept, class... Args>
        using is_body_satisfied_t = is_detected<is_body_satisfied_impl, Concept, Args...>;

        // @todo поддержка уточнения концепций: refines
        template <class Concept, class... Args>
        using satisfied_t = is_body_satisfied_t<Concept, Args...>;

        // @todo Выделить общую базовую концепцию
        struct OutputCursor
        {
            template <class T1, class T2>
            auto requires(T1 && x, T2 && y)
            -> valid_types<decltype(sayan::as_const(x).empty()),
                           decltype(x << std::forward<T2>(y)),
                           ::sayan::difference_type_t<std::decay_t<T1>>>;
        };

        struct InputCursor
        {
            template <class T>
            auto requires(T && x)
            -> valid_types<decltype(sayan::as_const(x).empty()),
                           decltype(x[::sayan::front]),
                           decltype(x.drop(::sayan::front)),
                           ::sayan::difference_type_t<std::decay_t<T>>>;
        };

        // @todo Определить ForwardCursor, BidirectionalCursor, RandomAccessCursor
    }
    // namespace concepts

    template <class C, class T>
    struct is_output_cursor
     : concepts::satisfied_t<concepts::OutputCursor, C, T>
    {};

    template <class T>
    struct is_input_cursor
     : concepts::satisfied_t<concepts::InputCursor, T>
    {};

    // @todo Уточнить
    template <class T>
    struct is_random_access_cursor
     : is_input_cursor<T>
    {};

    // @todo Не определять "лишние" функции в курсорах
    template <class Cursor, class = std::enable_if_t<is_input_cursor<Cursor>::value>>
    auto operator*(Cursor const & cur)
    -> decltype(cur[::sayan::front])
    {
        return cur[::sayan::front];
    }

    template <class Cursor, class = std::enable_if_t<is_input_cursor<Cursor>::value>>
    bool operator!(Cursor const & cur)
    {
        return cur.empty();
    }

    template <class Cursor, class = std::enable_if_t<is_input_cursor<Cursor>::value>>
    Cursor & operator++(Cursor & cur)
    {
        cur.drop(::sayan::front);
        return cur;
    }

    /** @brief Оператор << для курсоров: присваивает текущему элементу значение и переходит к
    следующему, используя оператор ++.
    @param cur курсор
    @param value выводимое значение
    @return @c cur
    */
    template <class Cursor, class T>
    auto operator<<(Cursor & cur, T && value)
    -> declare_first_type_t<Cursor &, decltype(*cur = std::forward<T>(value)), decltype(++cur)>
    {
        *cur = std::forward<T>(value);
        ++ cur;
        return cur;
    }

    template <class Cursor, class = std::enable_if_t<is_input_cursor<Cursor>::value>>
    Cursor cursor_hook(Cursor cur, adl_tag)
    {
        // @todo Убрать пройденные части, после этого можно будет не определять cursor_hook
        // в конкретных курсорах
        return cur;
    }

    template <class Cursor, class = std::enable_if_t<is_random_access_cursor<Cursor>::value>>
    Cursor &
    operator+=(Cursor & cur, difference_type_t<Cursor> n)
    {
        cur.drop(::sayan::front_fn{}, n);
        return cur;
    }
}
//inline namespace v1
}
//namespace sayan

#endif
// Z_URAL_CURSOR_DEFS_HPP_INCLUDED
