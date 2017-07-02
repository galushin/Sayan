#ifndef Z_SAYAN_TYPE_TRAITS_HPP_INCLUDED
#define Z_SAYAN_TYPE_TRAITS_HPP_INCLUDED

/** @file sayan/type_traits.hpp
 @brief Характеристики типов и простейшие мета-программирование
*/

#include <type_traits>

namespace sayan
{
namespace v1
{
    /** @brief Тип, объявляющий вложенный тип @c type равный @c T
    @tparam T тип
    */
    template <class T>
    struct declare_type
    {
        using type = T;
    };

    /// @cond false
    namespace details
    {
        template <class... Ts>
        struct declare_void
        {
            using type = void;
        };
    }
    // namespace details
    /// @endcond

    template <class... Ts>
    using void_t = typename details::declare_void<Ts...>::type;

    /// @cond false
    namespace details
    {
        template <class Default, class AlwaysVoid, template <class...> class Trait, class... Args>
        struct detector
        {
            using value_t = std::false_type;
            using type = Default;
        };

        template <class Default, template <class...> class Trait, class... Args>
        struct detector<Default, void_t<Trait<Args...>>, Trait, Args...>
        {
            using value_t = std::true_type;
            using type = Trait<Args...>;
        };
    }
    // namespace details
    /// @endcond

    struct nonesuch
    {
        nonesuch() = delete;
        ~nonesuch() = delete;

        nonesuch(nonesuch const &) = delete;
        void operator=(nonesuch const &) = delete;
    };

    template <template <class...> class Trait, class... Args>
    using is_detected = typename details::detector<nonesuch, void, Trait, Args...>::value_t;

    /// @cond false
    namespace details
    {
        template <class T, class AlwaysVoid = void>
        struct difference_type;

        template <class T>
        struct difference_type<T, void_t<typename T::difference_type>>
         : declare_type<typename T::difference_type>
        {};
    }
    // namespace details
    /// @endcond

    template <class T>
    struct difference_type
     : details::difference_type<T>
    {};

    template <class T>
    using difference_type_t = typename difference_type<T>::type;

    template <class T>
    using reference_t = decltype(*std::declval<T&>());

    template <class T, class...>
    struct declare_first_type
     : declare_type<T>
    {};

    template <class T, class... Others>
    using declare_first_type_t = typename declare_first_type<T, Others...>::type;

    // Замена для возможностей из С++17
    template <class T>
    using add_const_t = typename ::std::add_const<T>::type;

    template <class... Bs>
    struct conjunction;

    template <>
    struct conjunction<>
     : declare_type<std::true_type>
    {};

    template <class B1, class... Bs>
    struct conjunction<B1, Bs...>
     : std::conditional_t<B1::value == false, B1, conjunction<Bs...>>
    {};
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_TYPE_TRAITS_HPP_INCLUDED
