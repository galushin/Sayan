#ifndef Z_SAYAN_ALGORITHM_HPP_INCLUDED
#define Z_SAYAN_ALGORITHM_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>

#include <functional>

namespace sayan
{
inline namespace v1
{
    template <class InputCursor, class UnaryFunction>
    InputCursor
    for_each(InputCursor cur, UnaryFunction fun)
    {
        for(; !!cur; ++ cur)
        {
            fun(*cur);
        }

        return cur;
    }

    template <class InputCursor1, class InputCursor2,
              class BinaryPredicate = std::equal_to<>>
    std::pair<InputCursor1, InputCursor2>
    mismatch(InputCursor1 in1, InputCursor2 in2,
             BinaryPredicate bin_pred = BinaryPredicate{})
    {
        for(; !!in1 && !!in2 && bin_pred(*in1, *in2); ++in1, ++in2)
        {}

        return {std::move(in1), std::move(in2)};
    }

    template <class InputCursor1, class InputCursor2,
              class BinaryPredicate = std::equal_to<>>
    bool equal(InputCursor1 in1, InputCursor2 in2,
               BinaryPredicate bin_pred = BinaryPredicate{})
    {
        auto r = ::sayan::mismatch(std::move(in1), std::move(in2), std::move(bin_pred));

        return !r.first && !r.second;
    }
}
//namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_ALGORITHM_HPP_INCLUDED
