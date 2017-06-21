#ifndef Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED
#define Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>
#include <sayan/cursor/check_policies.hpp>
#include <sayan/utility/with_old_value.hpp>

namespace sayan
{
inline namespace v1
{
    template <class Iterator,
              class Sentinel = Iterator,
              class Check = cursor_checking_throw>
    class iterator_cursor_type
    {
        friend iterator_cursor_type cursor_hook(iterator_cursor_type cur, adl_tag)
        {
            cur.forget(sayan::front_fn{});

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

        void drop(sayan::front_fn, difference_type n)
        {
            Check::check_step(*this, n);

            this->begin_.value() += n;
        }

        // Прямой курсор
        iterator_cursor_type traversed(sayan::front_fn) const
        {
            return iterator_cursor_type(this->begin_.old_value(), this->begin_.value());
        }

        void forget(sayan::front_fn)
        {
            this->begin_.commit();
        }

        void exhaust(sayan::front_fn)
        {
            this->begin_.value() = this->end_.value();
        }

        void splice(iterator_cursor_type other)
        {
            Check::ensure_equal(other.begin_.value(), this->end_.value());
            this->end_ = other.end_;
        }

        // Двунаправленный курсор
        reference operator[](sayan::back_fn) const
        {
            Check::ensure_not_empty(*this);

            auto i = this->end();
            -- i;
            return *i;
        }

        void drop(sayan::back_fn)
        {
            Check::ensure_not_empty(*this);

            --this->end_.value();
        }

        iterator_cursor_type traversed(sayan::back_fn) const
        {
            return iterator_cursor_type(this->end_.value(), this->end_.old_value());
        }

        // Курсор произвольного доступа
        reference operator[](difference_type index) const
        {
            Check::check_index(*this, index);

            return this->begin()[index];
        }

        difference_type size() const
        {
            return this->end() - this->begin();
        }

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
