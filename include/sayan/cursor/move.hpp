#ifndef Z_SAYAN_CURSOR_MOVE_HPP_INCLUDED
#define Z_SAYAN_CURSOR_MOVE_HPP_INCLUDED

#include <sayan/cursor/sequence_to_cursor.hpp>

namespace sayan
{
inline namespace v1
{
    /** @brief Адаптор курсора, ведующий себя так же как базовый курсор, за исключением того, что
    при разыменовании преобразует значение, которое вернул базовый курсор, во временное значение.
    @tparam Cursor тип базового курсора
    */
    template <class Cursor>
    class move_cursor
    {
        friend move_cursor cursor_hook(move_cursor cur, adl_tag)
        {
            cur.base_ = ::sayan::cursor(std::move(cur.base_));
            return cur;
        }

        using base_reference = ::sayan::reference_t<Cursor>;

    public:
        // Типы
        /// @brief Тип расстояния
        using difference_type = sayan::difference_type_t<Cursor>;

        /// @brief Тип ссылки
        using reference
            = std::conditional_t<std::is_reference<base_reference>::value,
                                 std::remove_reference_t<base_reference> &&,
                                 base_reference>;

        // Конструктор
        /** @brief Конструктор
        @param cur курсор
        @post <tt> this->base() == cur </tt>
        */
        explicit move_cursor(Cursor cur)
         : base_(std::move(cur))
        {}

        // Адаптор курсора
        Cursor const & base() const
        {
            return this->base_;
        }

        // Курсор
        bool empty() const
        {
            return this->base().empty();
        }

        template <class F>
        reference operator[](F f) const
        {
            return static_cast<reference>(this->base()[std::move(f)]);
        }

        template <class F>
        void drop(F f)
        {
            this->base_.drop(std::move(f));
        }

        template <class F>
        auto traversed(F f)
        -> move_cursor<decltype(this->base().traversed(std::move(f)))>
        {
            using R = move_cursor<decltype(this->base().traversed(std::move(f)))>;
            return R{this->base().traversed(std::move(f))};
        }

        template <class F>
        void forget(F f)
        {
            this->base_.forget(std::move(f));
        }

        template <class F>
        void exhaust(F f)
        {
            this->base_.exhaust(std::move(f));
        }

        void splice(move_cursor other)
        {
            this->base_.splice(std::move(other.base_));
        }

        // Курсор производного доступа
        /** @brief Количество элементов
        @return <tt> this->base().size() </tt>
        */
        difference_type size() const
        {
            return this->base().size();
        }

        template <class F>
        void drop(F f, difference_type n)
        {
            this->base_.drop(std::move(f), std::move(n));
        }

    private:
        Cursor base_;
    };

    template <class InputSequence>
    move_cursor<safe_cursor_type_t<InputSequence>>
    make_move_cursor(InputSequence && seq)
    {
        using Cur = move_cursor<safe_cursor_type_t<InputSequence>>;
        return Cur(::sayan::cursor_fwd<InputSequence>(seq));
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_CURSOR_MOVE_HPP_INCLUDED
