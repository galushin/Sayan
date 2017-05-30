#ifndef Z_SAYAN_ALGORITHM_HPP_INCLUDED
#define Z_SAYAN_ALGORITHM_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>

#include <functional>

namespace sayan
{
inline namespace v1
{
    template <class InputCursor1, class InputCursor2,
              class BinaryPredicate = std::equal_to<>>
    bool equal(InputCursor1 in1, InputCursor2 in2,
               BinaryPredicate bin_pred = BinaryPredicate{})
    {
        for(; !!in1 && !!in2; ++in1, ++in2)
        {
            if(!bin_pred(*in1, *in2))
            {
                return false;
            }
        }

        return in1.empty() && in2.empty();
    }
}
//namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_ALGORITHM_HPP_INCLUDED
