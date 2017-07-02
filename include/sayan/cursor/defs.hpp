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
        template <class... Cs>
        struct refines
        {};

        /// @cond false
        namespace details
        {
            template <class... Ts>
            struct valid_types
             : declare_type<int>
            {};

            // Проверка того, что "тело" концепции удовлетворено
            template <class C, class... Args>
            using is_body_satisfied_impl
                = decltype(std::declval<C>().requires_(std::declval<Args>()...));

            template <class Concept, class... Args>
            using is_body_satisfied_t = is_detected<details::is_body_satisfied_impl, Concept, Args...>;

            // Уточняемые концепции
            struct get_refined
            {
                static refines<> get(void *);

                template <class... Cs>
                static refines<Cs...> get(refines<Cs...> *);
            };

            template <class T>
            using refined_concepts_t = decltype(get_refined::get(static_cast<T*>(nullptr)));

            // Проверка концепции целиком
            template <class Concept, class... Args>
            struct satisfied;

            template <class ConceptList, class... Args>
            struct is_refined_satisfied;

            template <class... Cs, class... Args>
            struct is_refined_satisfied<concepts::refines<Cs...>, Args...>
             : ::sayan::conjunction<typename satisfied<Cs, Args...>::type...>
            {};

            template <class ConceptList, class... Args>
            using is_refined_satisfied_t = typename is_refined_satisfied<ConceptList, Args...>::type;

            template <class Concept, class... Args>
            struct satisfied
            {
            private:
                using Body_t = is_body_satisfied_t<Concept, Args...>;
                using Refined = refined_concepts_t<Concept>;
                using Bases_t = is_refined_satisfied_t<Refined, Args...>;

            public:
                using type = std::integral_constant<bool, Body_t::value && Bases_t::value>;
            };
        }
        // namespace details
        /// @endcond

        template <class... Ts>
        using valid_types = details::valid_types<Ts...>;

        template <class Concept, class... Args>
        struct satisfied
         : details::satisfied<Concept, Args...>::type
        {};

        template <class Concept, class... Args>
        using satisfied_t = typename satisfied<Concept, Args...>::type;

        // @todo Выделить общую базовую концепцию
        struct OutputCursor
        {
            template <class T1, class T2>
            auto requires_(T1 && x, T2 && y)
            -> valid_types<decltype(sayan::as_const(x).empty()),
                           decltype(x << std::forward<T2>(y)),
                           ::sayan::difference_type_t<std::decay_t<T1>>>;
        };

        struct InputCursor
        {
            template <class T>
            auto requires_(T && x)
            -> valid_types<decltype(sayan::as_const(x).empty()),
                           decltype(x[::sayan::front]),
                           decltype(x.drop(::sayan::front)),
                           ::sayan::difference_type_t<std::decay_t<T>>>;
        };

        struct ForwardCursor
         : concepts::refines<InputCursor>
        {
            template <class T>
            auto requires_(T && x)
            -> valid_types<decltype(*x = *x),
                           decltype(sayan::as_const(x).traversed(sayan::front_fn{})),
                           decltype(x.exhaust(sayan::front_fn{})),
                           decltype(x.forget(sayan::front_fn{})),
                           decltype(x.splice(x))>;
        };

        // @todo Определить BidirectionalCursor, RandomAccessCursor
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

    template <class T>
    struct is_forward_cursor
     : concepts::satisfied_t<concepts::ForwardCursor, T>
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
