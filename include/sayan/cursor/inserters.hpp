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
        @post Элементы, выводимые в курсор, будут вставлены в конец контейнера @c sink.
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
            this->sink_.get().push_back(x);
            return *this;
        }

        back_insert_cursor &
        operator<<(typename Container::value_type && x)
        {
            this->sink_.get().push_back(std::move(x));
            return *this;
        }
        //@}

    private:
        std::reference_wrapper<Container> sink_;
    };

    /** @ingroup Cursors
    @brief Курсор вывода, добавляющий значение в начало заданного контейнера.
    @tparam Container тип контейнера, поддерживающего вставку в начало.
    */
    template <class Container>
    class front_insert_cursor
    {
        /** @brief Функция преобразования в курсор
        @param cur курсор
        @return @c cur
        */
        friend front_insert_cursor cursor_hook(front_insert_cursor cur, adl_tag)
        {
            return cur;
        }

    public:
        // Создание
        /** @brief Конструктор
        @param sink контейнер, в который будет осуществляться вставка
        @post Элементы, выводимые в курсор, будут вставлены в начло контейнера @c sink.
        */
        front_insert_cursor(Container & sink)
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
        @post В начало связанного контейнера будет добавлен элемент @c x
        @return <tt> *this </tt>
        */
        front_insert_cursor &
        operator<<(typename Container::value_type const & x)
        {
            this->sink_.get().push_front(x);
            return *this;
        }

        front_insert_cursor &
        operator<<(typename Container::value_type && x)
        {
            this->sink_.get().push_front(std::move(x));
            return *this;
        }
        //@}

    private:
        std::reference_wrapper<Container> sink_;
    };

    /** @brief Курсор вывода для вставки в заданное место контейнера.
    @tparam Container тип контейнера
    */
    template <class Container>
    class insert_cursor
    {
        /** @brief Функция преобразования в курсор
        @param cur курсор
        @return @c cur
        */
        friend insert_cursor cursor_hook(insert_cursor cur, adl_tag)
        {
            return cur;
        }

    public:
        // Типы
        /// @brief Тип значения
        using value_type = typename Container::value_type;

        // Конструкторы
        /** @brief Конструктор
        @param c контейнер
        @param pos итератор, перед которым должна осуществляться вставка.
        @pre @c pos должен определять позицию внутри контейнера @c c.
        */
        constexpr insert_cursor(Container & c, typename Container::const_iterator pos)
         : sink_(c)
         , pos_(std::move(pos))
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
        @param value значение, которое будет выведено
        @return <tt> *this </tt>
        */
        insert_cursor &
        operator<<(value_type const & value)
        {
            return this->put(value);
        }

        insert_cursor &
        operator<<(value_type && value)
        {
            return this->put(std::move(value));
        }
        //@}

    private:
        template <class T>
        insert_cursor & put(T && x)
        {
            this->pos_ = this->sink_.get().insert(this->pos_, std::forward<T>(x));
            ++ this->pos_;
            return *this;
        }

        std::reference_wrapper<Container> sink_;
        typename Container::const_iterator pos_;
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

        struct front_inserter_fn
        {
            template <class Container>
            constexpr ::sayan::front_insert_cursor<Container>
            operator()(Container & c) const
            {
                return ::sayan::front_insert_cursor<Container>(c);
            }
        };

        struct inserter_fn
        {
            template <class Container>
            constexpr ::sayan::insert_cursor<Container>
            operator()(Container & c, typename Container::const_iterator pos) const
            {
                return ::sayan::insert_cursor<Container>(c, pos);
            }
        };
    }
    // namespace details
    /// @endcond

    namespace
    {
        /// @brief Функциональный объект для создания курсора вставки в конец контейнера.
        constexpr auto const & back_inserter = static_const<details::back_inserter_fn>;

        /// @brief Функциональный объект для создания курсора вставки в начало контейнера.
        constexpr auto const & front_inserter = static_const<details::front_inserter_fn>;

        /// @brief Функциональный объект для создания курсора вставки перед заданной позицией
        constexpr auto const & inserter = static_const<details::inserter_fn>;
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_BACK_INSERTER_HPP_INCLUDED
