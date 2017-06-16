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
        struct reverse_indicies_fn
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
    public:
        // Типы
        using reference = typename Cursor::reference;

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
            return this->base_[::sayan::details::reverse_indicies_fn{}(std::move(f))];
        }

        template <class F>
        void drop(F f)
        {
            this->base_.drop(::sayan::details::reverse_indicies_fn{}(std::move(f)));
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
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_REVERSE_HPP_INCLUDED
