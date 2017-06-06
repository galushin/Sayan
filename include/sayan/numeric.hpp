#ifndef Z_SAYAN_NUMERIC_HPP_INCLUDED
#define Z_SAYAN_NUMERIC_HPP_INCLUDED

#include <sayan/utility/static_const.hpp>
#include <sayan/cursor/sequence_to_cursor.hpp>

#include <functional>

namespace sayan
{
inline namespace v1
{
    struct iota_fn
    {
        template <class OutputSequence, class Incrementable>
        sayan::safe_cursor_type_t<OutputSequence>
        operator()(OutputSequence && out, Incrementable value) const
        {
            auto out_cur = ::sayan::cursor_fwd<OutputSequence>(out);

            for(; !!out_cur; ++ value)
            {
                out_cur << value;
            }

            return out_cur;
        }
    };

    struct accumulate_fn
    {
        template <class InputSequence, class T, class BinaryOperation = std::plus<>>
        T
        operator()(InputSequence && in, T init_value, BinaryOperation op = BinaryOperation{}) const
        {
            for(auto in_cur = ::sayan::cursor_fwd<InputSequence>(in); !!in_cur; ++ in_cur)
            {
                init_value = op(init_value, *in_cur);
            }

            return init_value;
        }
    };

    struct inner_product_fn
    {
        template <class InputSequence1, class InputSequence2, class T,
                  class BinaryOperation1 = std::plus<>, class BinaryOperation2 = std::multiplies<>>
        T
        operator()(InputSequence1 && in1, InputSequence2 && in2, T init_value,
                   BinaryOperation1 op1 = BinaryOperation1{},
                   BinaryOperation2 op2 = BinaryOperation2{}) const
        {
            auto in1_cur = ::sayan::cursor_fwd<InputSequence1>(in1);
            auto in2_cur = ::sayan::cursor_fwd<InputSequence2>(in2);

            for(; !!in1_cur && !!in2_cur; ++ in1_cur, ++in2_cur)
            {
                init_value = op1(init_value, op2(*in1_cur, *in2_cur));
            }

            return init_value;
        }
    };

    struct partial_sum_fn
    {
        template <class InputSequence, class OutputSequence, class BinaryOperation = std::plus<>>
        std::pair<safe_cursor_type_t<InputSequence>, safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out,
                   BinaryOperation op = BinaryOperation{}) const
        {
            auto in_cur = sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            if(!in_cur || !out_cur)
            {
                return {std::move(in_cur), std::move(out_cur)};
            }

            auto value = *in_cur;
            ++ in_cur;
            out_cur << value;

            for(; !!in_cur && !!out_cur; ++ in_cur)
            {
                value = op(std::move(value), *in_cur);
                out_cur << value;
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    struct adjacent_difference_fn
    {
        template <class InputSequence, class OutputSequence, class BinaryOperation = std::minus<>>
        std::pair<safe_cursor_type_t<InputSequence>, safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out,
                   BinaryOperation op = BinaryOperation{}) const
        {
            auto in_cur = sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            if(!in_cur || !out_cur)
            {
                return {std::move(in_cur), std::move(out_cur)};
            }

            auto prev_value = *in_cur;
            ++ in_cur;
            out_cur << prev_value;

            for(; !!in_cur && !!out_cur; ++ in_cur)
            {
                auto cur_value = *in_cur;
                out_cur << op(cur_value, std::move(prev_value));
                prev_value = std::move(cur_value);
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    namespace
    {
        constexpr auto const & iota = ::sayan::static_const<::sayan::iota_fn>;
        constexpr auto const & accumulate = ::sayan::static_const<::sayan::accumulate_fn>;
        constexpr auto const & inner_product = ::sayan::static_const<::sayan::inner_product_fn>;
        constexpr auto const & partial_sum = ::sayan::static_const<::sayan::partial_sum_fn>;
        constexpr auto const & adjacent_difference
            = sayan::static_const<::sayan::adjacent_difference_fn>;
    }
}
// namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_NUMERIC_HPP_INCLUDED
