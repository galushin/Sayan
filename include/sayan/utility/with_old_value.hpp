#ifndef Z_SAYAN_UTILITY_WITH_OLD_VALUE_HPP_INCLUDED
#define Z_SAYAN_UTILITY_WITH_OLD_VALUE_HPP_INCLUDED

/** @file sayan/utility/with_old_value.hpp
 @brief Обёртка, сохраняющее старое значение переменной.
*/

namespace sayan
{
inline namespace v1
{
    template <class T>
    class with_old_value
    {
    public:
        // Конструктор
        explicit with_old_value(T value)
         : value_(std::move(value))
         , old_value_(this->value_)
        {}

        // Доступ к значению
        // @{

        T const & value() const
        {
            return this->value_;
        }

        T & value()
        {
            return this->value_;
        }
        //@}

        // Операции со старым значением
        T const & old_value() const
        {
            return this->old_value_;
        }

        void commit()
        {
            this->old_value_ = this->value_;
        }

    private:
        T value_;
        T old_value_;
    };
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_UTILITY_WITH_OLD_VALUE_HPP_INCLUDED
