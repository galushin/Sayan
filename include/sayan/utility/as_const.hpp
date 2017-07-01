#ifndef Z_SAYAN_UTILITY_AS_CONST_HPP_INCLUDED
#define Z_SAYAN_UTILITY_AS_CONST_HPP_INCLUDED

/** @file sayan/utility/as_const.hpp
 @brief Константный вид переменной. Замена для возможностей из С++17.
*/

#include <sayan/type_traits.hpp>

namespace sayan
{
inline namespace v1
{
    template <class T>
    constexpr
    sayan::add_const_t<T> &
    as_const(T & x) noexcept;

    template <class T>
    void as_const(const T&&) = delete;
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_UTILITY_AS_CONST_HPP_INCLUDED
