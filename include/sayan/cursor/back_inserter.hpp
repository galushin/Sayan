#ifndef Z_SAYAN_CURSOR_BACK_INSERTER_HPP_INCLUDED
#define Z_SAYAN_CURSOR_BACK_INSERTER_HPP_INCLUDED

/** @file sayan/cursor/back_inserter.hpp
 @brief Курсор вывода, добавляющий значение в конец заданного контейнера.
*/

#include <sayan/adl_tag.hpp>
#include <sayan/utility/static_const.hpp>

#include <utility>

namespace sayan
{
inline namespace v1
{
    /** @ingroup Cursors
    @brief Курсор вывода, добавляющий значение в конец заданного контейнера.
    @tparam Container тип контейнера, поддерживающего вставку в конец.
    */
    template <class Container>
    class back_insert_cursor
    {
        /** @brief Функция преобразования в курсор
        @param cur курсор
        @return @c cur
        */
        friend back_insert_cursor cursor_hook(back_insert_cursor cur, adl_tag)
        {
            return cur;
        }

    public:
        // Создание
        /** @brief Конструктор
        @param sink контейнер, в который будет осуществляться вставка
        @post Элементы, выводимые в курсор, будут вставлены в конец
        контейнера @c sink.
        */
        back_insert_cursor(Container & sink)
         : sink_(sink)
        {}

        // Курсор вывода
        /** @brief Проверка исчерпания курсора
        @return @b false
        */
        constexpr bool operator!() const
        {
            return false;
        }

        //@{
        /** @brief Запись в курсор
        @param x записываемые значения
        @post В конец связанного контейнера будет добавлен элемент @c x
        @return <tt> *this </tt>
        */
        back_insert_cursor &
        operator<<(typename Container::value_type const & x)
        {
            this->sink_.push_back(x);
            return *this;
        }

        back_insert_cursor &
        operator<<(typename Container::value_type && x)
        {
            this->sink_.push_back(std::move(x));
            return *this;
        }
        //@}

    private:
        Container & sink_;
    };

    /// @cond false
    namespace details
    {
        struct back_inserter_fn
        {
            template <class Container>
            constexpr ::sayan::back_insert_cursor<Container>
            operator()(Container & c) const
            {
                return ::sayan::back_insert_cursor<Container>(c);
            }
        };
    }
    // namespace details
    /// @endcond

    namespace
    {
        /** @brief Функциональный объект для создания курсора вставки в
        конец контейнера.
        */
        constexpr auto const & back_inserter
            = static_const<details::back_inserter_fn>;
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_BACK_INSERTER_HPP_INCLUDED
