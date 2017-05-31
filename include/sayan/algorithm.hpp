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

    template <class InputCursor, class UnaryPredicate>
    InputCursor
    find_if(InputCursor cur, UnaryPredicate pred)
    {
        for(; !!cur && !pred(*cur); ++ cur)
        {}

        return cur;
    }

    template <class InputCursor, class UnaryPredicate>
    InputCursor
    find_if_not(InputCursor cur, UnaryPredicate pred)
    {
        using Ref = decltype(*cur);
        return ::sayan::find_if(std::move(cur),
                                [&pred](Ref x) { return !pred(x); });
    }

    template <class InputCursor, class T>
    InputCursor
    find(InputCursor cur, T const & value)
    {
        using Ref = decltype(*cur);
        return ::sayan::find_if(std::move(cur),
                                [&value](Ref x) { return x == value; });
    }

    template <class InputCursor, class UnaryPredicate>
    bool all_of(InputCursor cur, UnaryPredicate pred)
    {
        return !::sayan::find_if_not(std::move(cur), std::move(pred));
    }

    template <class InputCursor, class UnaryPredicate>
    bool any_of(InputCursor cur, UnaryPredicate pred)
    {
        return !!::sayan::find_if(std::move(cur), std::move(pred));
    }

    template <class InputCursor, class UnaryPredicate>
    bool none_of(InputCursor cur, UnaryPredicate pred)
    {
        return !::sayan::any_of(std::move(cur), std::move(pred));
    }

    template <class InputCursor, class UnaryPredicate>
    typename ::sayan::cursor_traits<InputCursor>::difference_type
    count_if(InputCursor cur, UnaryPredicate pred)
    {
        auto result = typename ::sayan::cursor_traits<InputCursor>::difference_type{0};

        for(; !!cur; ++ cur)
        {
            if(pred(*cur))
            {
                ++ result;
            }
        }

        return result;
    }

    template <class InputCursor, class T>
    typename ::sayan::cursor_traits<InputCursor>::difference_type
    count(InputCursor cur, T const & value)
    {
        using Ref = decltype(*cur);
        return ::sayan::count_if(std::move(cur),
                                 [&value](Ref x) { return x == value; });
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

    template <class InputCursor, class UnaryPredicate>
    bool is_partitioned(InputCursor cur, UnaryPredicate pred)
    {
        cur = ::sayan::find_if_not(std::move(cur), pred);
        cur = ::sayan::find_if(std::move(cur), pred);

        return !cur;
    }

    template <class InputCursor1, class InputCursor2,
              class Compare = std::less<>>
    bool includes(InputCursor1 in1, InputCursor2 in2, Compare cmp = Compare{})
    {
        for(; !!in1 && !!in2;)
        {
            if(cmp(*in1, *in2))
            {
                ++ in1;
            }
            else if(cmp(*in2, *in1))
            {
                return false;
            }
            else
            {
                ++ in1;
                ++ in2;
            }
        }

        return !in2;
    }

    template <class InputCursor1, class InputCursor2,
              class Compare = std::less<>>
    bool lexicographical_compare(InputCursor1 in1, InputCursor2 in2,
                                 Compare cmp = Compare{})
    {
        for(; !!in1 && !!in2; ++ in1, ++ in2)
        {
            if(cmp(*in1, *in2))
            {
                return true;
            }
            else if(cmp(*in2, *in1))
            {
                return false;
            }
        }

        return !!in2;
    }
}
//namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_ALGORITHM_HPP_INCLUDED
