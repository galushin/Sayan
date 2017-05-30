#ifndef Z_SAYAN_CURSOR_ISTREAM_HPP_INCLUDED
#define Z_SAYAN_CURSOR_ISTREAM_HPP_INCLUDED

#include <stdexcept>

namespace sayan
{
inline namespace v1
{
    struct front_fn {};

    namespace
    {
        constexpr auto const & front = front_fn{};
    }

    struct cursor_checking_throw
    {
    public:
        template <class Cursor>
        static void ensure_not_empty(Cursor const & cur)
        {
            if(cur.empty())
            {
                throw std::logic_error("cursor must be not empty");
            }
        }
    };

    template <class IStream, class Check = cursor_checking_throw>
    class istream_by_char_cursor
    {
    public:
        // Типы
        using value_type = typename std::decay_t<IStream>::char_type;
        using reference = value_type const &;

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
    cursor(IStream && is)
    {
        using Cursor = istream_by_char_cursor<IStream>;
        return Cursor(std::forward<IStream>(is));
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_ISTREAM_HPP_INCLUDED
