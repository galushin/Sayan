#ifndef Z_SAYAN_CURSOR_OSTREAM_HPP_INCLUDED
#define Z_SAYAN_CURSOR_OSTREAM_HPP_INCLUDED

/** @file sayan/cursor/ostream.hpp
 @brief Курсор для потоков вывода
*/

#include <sayan/cursor/defs.hpp>

#include <utility>
#include <iosfwd>

namespace sayan
{
inline namespace v1
{
    /// @brief Тип, представляющий собой отсутствующий разделитель
    class empty_delimiter
    {
        template <class OStream>
        friend OStream &
        operator<<(OStream & os, empty_delimiter const &)
        {
            return os;
        }
    };

    /** @brief Курсор на основе потока вывода
    @tparam OStream тип потока вывода или ссылки на него
    @tparam Delimiter тип разделителя
    */
    template <class OStream, class Delimiter = empty_delimiter>
    class ostream_cursor_type
    {
    public:
        /** @brief Конструктор
        @param os поток, в который будет производится вывод
        @param delim разделитель, который будет помещаться между выводимыми
        значениями
        */
        explicit ostream_cursor_type(OStream && os,
                                     Delimiter delim = Delimiter{})
         : os_(std::forward<OStream>(os))
         , delim_(std::move(delim))
         , on_delim_(&ostream_cursor_type::write_delim_first)
        {}

        /** @brief Помещение очередного значения в поток
        @param value выводимое значение
        @return <tt> *this </tt>
        */
        template <class T>
        ostream_cursor_type & operator<<(T const & value)
        {
            (this->*on_delim_)();

            this->os_ << value;

            return *this;
        }

        //@{
        bool empty() const
        {
            return !this->os_;
        }

        bool operator!() const
        {
            return this->empty();
        }
        //@}

        /** @brief Передать поток вывода вызывающей стороне
        @post Если @c OStream -- не ссылочный тип, то после вызова курсор
        становится недействительным.
        */
        OStream release()
        {
            return std::move(os_);
        }

    private:
        void write_delim_first()
        {
            this->on_delim_ = &ostream_cursor_type::write_delim_others;
        }

        void write_delim_others()
        {
            this->os_ << this->delim_;
        }

        OStream os_;
        Delimiter delim_;
        void(ostream_cursor_type::*on_delim_)();
    };

    template <class OStream, class Delimiter>
    ostream_cursor_type<OStream, Delimiter>
    cursor_hook(ostream_cursor_type<OStream, Delimiter> cur, adl_tag)
    {
        return cur;
    }

    /** @brief Функция создания @c ostream_cursor_type
    @param os поток вывода
    @param delim разделитель
    */
    template <class OStream, class Delimiter = empty_delimiter>
    ostream_cursor_type<OStream, Delimiter>
    ostream_cursor(OStream && os, Delimiter delim = Delimiter{})
    {
        using R = ostream_cursor_type<OStream, Delimiter>;
        return R(std::forward<OStream>(os), std::move(delim));
    }

    /// @cond false
    namespace details
    {
        template <class T>
        struct is_based_on_ostream
        {
        private:
            template <class Char, class Traits>
            static std::true_type test(std::basic_ostream<Char, Traits> &);

            static std::false_type test(...);

        public:
            using type = decltype(test(std::declval<T&>()));
        };
    }
    /// @endcond

    template <class T>
    struct is_based_on_ostream
     : details::is_based_on_ostream<T>::type
    {};

    /** @brief Функция создания @c ostream_cursor_type.
    @tparam Тип потока вывода, должен быть производным от некоторого экземпляра
    шаблона класса @c basic_ostream.
    @param os поток вывода
    @param delim разделитель
    */
    template <class OStream,
              class = std::enable_if_t<is_based_on_ostream<OStream>::value>>
    ostream_cursor_type<OStream>
    cursor_hook(OStream && os, adl_tag)
    {
        return ostream_cursor_type<OStream>(std::forward<OStream>(os));
    }
}
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_OSTREAM_HPP_INCLUDED
