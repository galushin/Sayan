#ifndef Z_SAYAN_CURSOR_REVERSE_HPP_INCLUDED
#define Z_SAYAN_CURSOR_REVERSE_HPP_INCLUDED

#include <sayan/cursor/sequence_to_cursor.hpp>

namespace sayan
{
inline namespace v1
{
    /// @cond false
    namespace details
    {
        struct reverse_index_fn
        {
            ::sayan::back_fn operator()(::sayan::front_fn) const
            {
                return {};
            };

            ::sayan::front_fn operator()(::sayan::back_fn) const
            {
                return {};
            };
        };
    }
    // @encond

    // namespace details
    template <class Cursor>
    class reverse_cursor
    {
        friend reverse_cursor cursor_hook(reverse_cursor cur, adl_tag)
        {
            cur.base_ = ::sayan::cursor(std::move(cur.base_));
            return cur;
        }

    public:
        // Типы
        using reference = sayan::reference_t<Cursor>;
        using difference_type = sayan::difference_type_t<Cursor>;

        // Создание
        explicit reverse_cursor(Cursor cur)
         : base_(std::move(cur))
        {}

        // Курсор
        bool empty() const
        {
            return this->base_.empty();
        }

        template <class F>
        reference operator[](F f) const
        {
            return this->base_[::sayan::details::reverse_index_fn{}(std::move(f))];
        }

        template <class F>
        void drop(F f)
        {
            this->base_.drop(::sayan::details::reverse_index_fn{}(std::move(f)));
        }

        template <class F>
        auto traversed(F f) const
        {
            auto b = this->base_.traversed(::sayan::details::reverse_index_fn{}(std::move(f)));
            return reverse_cursor(std::move(b));
        }

        template <class F>
        void forget(F f)
        {
            this->base_.forget(::sayan::details::reverse_index_fn{}(std::move(f)));
        }

        template <class F>
        void exhaust(F f)
        {
            this->base_.exhaust(::sayan::details::reverse_index_fn{}(std::move(f)));
        }

        difference_type size() const
        {
            return this->base().size();
        }

        void splice(reverse_cursor other)
        {
            other.base_.splice(std::move(this->base_));
            this->base_ = std::move(other.base_);
        }

        reference operator[](difference_type index) const
        {
            return this->base()[this->size() - index - 1];
        }

        template <class F>
        void drop(F f, difference_type n)
        {
            this->base_.drop(::sayan::details::reverse_index_fn{}(std::move(f)), n);
        }

        // Адаптор курсора
        Cursor const & base() const &
        {
            return this->base_;
        }

    private:
        Cursor base_;
    };

    template <class BidirectionalSequence>
    reverse_cursor<cursor_type_t<BidirectionalSequence>>
    make_reverse_cursor(BidirectionalSequence && seq)
    {
        using R = reverse_cursor<cursor_type_t<BidirectionalSequence>>;
        return R{::sayan::cursor_fwd<BidirectionalSequence>(seq)};

    }

    template <class BidirectionalCursor>
    BidirectionalCursor
    make_reverse_cursor(reverse_cursor<BidirectionalCursor> const & cur)
    {
        return cur.base();
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_REVERSE_HPP_INCLUDED
