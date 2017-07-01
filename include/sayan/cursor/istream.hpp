#ifndef Z_SAYAN_CURSOR_ISTREAM_HPP_INCLUDED
#define Z_SAYAN_CURSOR_ISTREAM_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>
#include <sayan/cursor/check_policies.hpp>

namespace sayan
{
inline namespace v1
{
    template <class IStream,
              class Check = cursor_checking_throw,
              class Distance = std::ptrdiff_t>
    class istream_by_char_cursor
    {
    public:
        // Типы
        using value_type = typename std::decay_t<IStream>::char_type;
        using reference = value_type const &;
        using difference_type = Distance;

        // Создание
        explicit istream_by_char_cursor(IStream && is)
         : is_(std::forward<IStream>(is))
        {
            this->read();
        }

        // Курсор ввода
        bool empty() const
        {
            return !this->is_;
        }

        reference operator[](front_fn) const
        {
            Check::ensure_not_empty(*this);

            return this->reader_;
        }

        void drop(front_fn)
        {
            this->read();
        }

    private:
        void read()
        {
            Check::ensure_not_empty(*this);

            this->reader_ = this->is_.get();
        }

        IStream is_;
        value_type reader_;
    };

    /// @cond false
    namespace details
    {
        template <class T>
        struct is_based_on_istream
        {
        private:
            template <class Char, class Traits>
            static std::true_type test(std::basic_istream<Char, Traits> &);

            static std::false_type test(...);

        public:
            using type = decltype(test(std::declval<T&>()));
        };
    }
    /// @endcond

    template <class T>
    struct is_based_on_istream
     : details::is_based_on_istream<T>::type
    {};

    template <class IStream, class = std::enable_if_t<sayan::is_based_on_istream<IStream>::value>>
    istream_by_char_cursor<IStream>
    cursor_hook(IStream && is, adl_tag)
    {
        using Cursor = istream_by_char_cursor<IStream>;
        return Cursor(std::forward<IStream>(is));
    }

    template <class T, class IStream,
              class Check = cursor_checking_throw,
              class Distance = std::ptrdiff_t>
    class istream_cursor_type
    {
    public:
        // Типы
        using difference_type = Distance;

        // Конструкторы
        explicit istream_cursor_type(IStream && is)
         : is_(std::forward<IStream>(is))
         {
             this->read();
         }

        // Курсор ввода
        bool empty() const
        {
            return !this->is_;
        }

        T const &
        operator[](front_fn) const
        {
            Check::ensure_not_empty(*this);

            return this->reader_;
        }

        void drop(front_fn)
        {
            this->read();
        }

    private:
        void read()
        {
            Check::ensure_not_empty(*this);

            this->is_ >> this->reader_;
        }

        IStream is_;
        T reader_;
    };

    template <class T, class IStream>
    istream_cursor_type<T, IStream>
    make_istream_cursor(IStream && is)
    {
        return istream_cursor_type<T, IStream>{std::forward<IStream>(is)};
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_ISTREAM_HPP_INCLUDED
