#ifndef Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED
#define Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED

#include <sayan/cursor/check_policies.hpp>

namespace sayan
{
inline namespace v1
{
    template <class Iterator,
              class Sentinel = Iterator,
              class Check = cursor_checking_throw>
    class iterator_cursor_type
    {
    public:
        // Типы
        using reference = typename ::std::iterator_traits<Iterator>::reference;

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
            return this->begin_ == this->end_;
        }

        reference operator[](sayan::front_fn) const
        {
            Check::ensure_not_empty(*this);

            return *this->begin_;
        }

        void drop(sayan::front_fn)
        {
            Check::ensure_not_empty(*this);

            ++this->begin_;
        }

        // Итераторы
        Iterator begin() const
        {
            return this->begin_;
        }

        Sentinel end() const
        {
            return this->end_;
        }

    private:
        Iterator begin_;
        Sentinel end_;
    };

    // @todo Запретить привязку к временным значениям?
    // @todo Использовать свободные функции begin и end?
    template <class Range>
    auto cursor(Range && r)
    -> iterator_cursor_type<decltype(std::forward<Range>(r).begin()),
                            decltype(std::forward<Range>(r).end())>
    {
        using Cursor = iterator_cursor_type<decltype(std::forward<Range>(r).begin()),
                                            decltype(std::forward<Range>(r).end())>;
        return Cursor{std::forward<Range>(r).begin(), std::forward<Range>(r).end()};
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_ITERATOR_CURSOR_HPP_INCLUDED
