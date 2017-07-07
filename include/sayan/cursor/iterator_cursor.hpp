#ifndef Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED
#define Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>
#include <sayan/cursor/check_policies.hpp>
#include <sayan/utility/with_old_value.hpp>

namespace sayan
{
inline namespace v1
{
    /** @brief Курсор, заданный парой итераторов.
    @tparam Iterator тип итератора
    @tparam Sentinel тип ограничителя
    @tparam Chech стратегия проверки ошибок
    */
    template <class Iterator,
              class Sentinel = Iterator,
              class Check = cursor_checking_throw>
    class iterator_cursor_type
    {
        friend iterator_cursor_type cursor_hook(iterator_cursor_type cur, adl_tag)
        {
            cur.forget_all(typename std::iterator_traits<Iterator>::iterator_category{});

            return cur;
        }

    public:
        // Типы
        using reference = typename ::std::iterator_traits<Iterator>::reference;
        using difference_type = typename ::std::iterator_traits<Iterator>::difference_type;

        // Конструкторы
        /** @brief Конструктор
        @param first начало интервала
        @param last конец интервала
        @post <tt> this->begin() == first </tt>
        @post <tt> this->end() == last </tt>
        */
        explicit iterator_cursor_type(Iterator first, Sentinel last)
         : begin_(std::move(first))
         , end_(std::move(last))
        {}

        // Итераторы
        Iterator begin() const
        {
            return this->begin_.value();
        }

        Sentinel end() const
        {
            return this->end_.value();
        }

        Iterator traversed_begin() const
        {
            return this->begin_.old_value();
        }

        Sentinel traversed_end() const
        {
            return this->end_.old_value();
        }

        // Однопроходный курсор
        bool empty() const
        {
            return this->begin_.value() == this->end_.value();
        }

        reference operator[](sayan::front_fn) const
        {
            Check::ensure_not_empty(*this);

            return *this->begin_.value();
        }

        void drop(sayan::front_fn)
        {
            Check::ensure_not_empty(*this);

            ++this->begin_.value();
        }

        // Прямой курсор
        template <class = std::enable_if_t<sayan::is_forward_iterator<Iterator>::value>>
        iterator_cursor_type traversed(sayan::front_fn) const
        {
            return iterator_cursor_type(this->begin_.old_value(), this->begin_.value());
        }

        template <class = std::enable_if_t<sayan::is_forward_iterator<Iterator>::value>>
        void forget(sayan::front_fn)
        {
            this->begin_.commit();
        }

        template <class = std::enable_if_t<sayan::is_forward_iterator<Iterator>::value>>
        void exhaust(sayan::front_fn)
        {
            this->begin_.value() = this->end_.value();
        }

        template <class = std::enable_if_t<sayan::is_forward_iterator<Iterator>::value>>
        void splice(iterator_cursor_type other)
        {
            Check::ensure_equal(other.begin_.value(), this->end_.value());
            this->end_ = other.end_;
        }

        // Двунаправленный курсор
        template <class = std::enable_if_t<sayan::is_bidirectional_iterator<Iterator>::value>>
        reference operator[](sayan::back_fn) const
        {
            Check::ensure_not_empty(*this);

            auto i = this->end();
            -- i;
            return *i;
        }

        template <class = std::enable_if_t<sayan::is_bidirectional_iterator<Iterator>::value>>
        void drop(sayan::back_fn)
        {
            Check::ensure_not_empty(*this);

            --this->end_.value();
        }

        template <class = std::enable_if_t<sayan::is_bidirectional_iterator<Iterator>::value>>
        iterator_cursor_type traversed(sayan::back_fn) const
        {
            return iterator_cursor_type(this->end_.value(), this->end_.old_value());
        }

        template <class = std::enable_if_t<sayan::is_bidirectional_iterator<Iterator>::value>>
        void forget(sayan::back_fn)
        {
            this->end_.commit();
        }

        template <class = std::enable_if_t<sayan::is_bidirectional_iterator<Iterator>::value>>
        void exhaust(sayan::back_fn)
        {
            this->end_.value() = this->begin_.value();
        }

        // Курсор произвольного доступа
        template <class = std::enable_if_t<sayan::is_random_access_iterator<Iterator>::value>>
        reference operator[](difference_type index) const
        {
            Check::check_index(*this, index);

            return this->begin()[index];
        }

        template <class = std::enable_if_t<sayan::is_random_access_iterator<Iterator>::value>>
        void drop(sayan::front_fn, difference_type n)
        {
            Check::check_step(*this, n);

            this->begin_.value() += n;
        }

        template <class = std::enable_if_t<sayan::is_random_access_iterator<Iterator>::value>>
        void drop(sayan::back_fn, difference_type n)
        {
            Check::check_step(*this, n);

            this->end_.value() -= n;
        }

        template <class = std::enable_if_t<sayan::is_random_access_iterator<Iterator>::value>>
        difference_type size() const
        {
            return this->end() - this->begin();
        }

    private:
         void forget_all(std::input_iterator_tag)
        {}

        void forget_all(std::forward_iterator_tag)
        {
            this->forget(sayan::front);
        }

        void forget_all(std::bidirectional_iterator_tag)
        {
            this->forget(sayan::front);
            this->forget(sayan::back);
        }

    private:
        ::sayan::with_old_value<Iterator> begin_;
        ::sayan::with_old_value<Sentinel> end_;
    };
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED
