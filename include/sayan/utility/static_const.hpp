#ifndef Z_SAYAN_UTILITY_STATIC_CONST_HPP_INCLUDED
#define Z_SAYAN_UTILITY_STATIC_CONST_HPP_INCLUDED

/** @file utility/static_const
 @brief Шаблон глобальной константы времени компиляции, используемой для
 создания глобальных функциональных объектов, заменяющих глобальные свободные
 функции.
*/

namespace sayan
{
    template <class T>
    constexpr T const static_const{};
}
// namespace sayan

#endif
// Z_SAYAN_UTILITY_STATIC_CONST_HPP_INCLUDED
