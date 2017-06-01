#ifndef Z_SAYAN_ALGORITHM_HPP_INCLUDED
#define Z_SAYAN_ALGORITHM_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>
#include <sayan/cursor/range_to_cursor.hpp>

#include <functional>

namespace sayan
{
inline namespace v1
{
    struct for_each_fn
    {
        template <class InputRange, class UnaryFunction>
        safe_cursor_type_t<InputRange>
        operator()(InputRange && in, UnaryFunction fun) const
        {
            auto cur = sayan::cursor(std::forward<InputRange>(in));

            for(; !!cur; ++ cur)
            {
                fun(*cur);
            }

            return cur;
        }
    };

    struct find_if_fn
    {
        template <class InputRange, class UnaryPredicate>
        safe_cursor_type_t<InputRange>
        operator()(InputRange && in, UnaryPredicate pred) const
        {
            auto cur = ::sayan::cursor(std::forward<InputRange>(in));

            for(; !!cur && !pred(*cur); ++ cur)
            {}

            return cur;
        }
    };

    struct find_if_not_fn
    {
        template <class InputRange, class UnaryPredicate>
        safe_cursor_type_t<InputRange>
        operator()(InputRange && in, UnaryPredicate pred) const
        {
            auto cur = ::sayan::cursor(std::forward<InputRange>(in));

            using Ref = decltype(*cur);
            return ::sayan::find_if_fn{}(std::move(cur),
                                         [&pred](Ref x) { return !pred(x); });
        }
    };

    struct find_fn
    {
        template <class InputRange, class T>
        safe_cursor_type_t<InputRange>
        operator()(InputRange && in, T const & value) const
        {
            auto cur = sayan::cursor(std::forward<InputRange>(in));

            using Ref = decltype(*cur);
            return ::sayan::find_if_fn{}(std::move(cur),
                                         [&value](Ref x){ return x == value; });
        }
    };

    struct all_of_fn
    {
        template <class InputRange, class UnaryPredicate>
        bool operator()(InputRange && in, UnaryPredicate pred) const
        {
            return !::sayan::find_if_not_fn{}(std::forward<InputRange>(in),
                                              std::move(pred));
        }
    };

    struct any_of_fn
    {
        template <class InputRange, class UnaryPredicate>
        bool operator()(InputRange && in, UnaryPredicate pred) const
        {
            return !!::sayan::find_if_fn{}(std::forward<InputRange>(in),
                                           std::move(pred));
        }
    };

    struct none_of_fn
    {
        template <class InputRange, class UnaryPredicate>
        bool operator()(InputRange && in, UnaryPredicate pred) const
        {
            return !::sayan::any_of_fn{}(std::forward<InputRange>(in),
                                         std::move(pred));
        }
    };

    struct count_if_fn
    {
        template <class InputRange, class UnaryPredicate>
        difference_type_t<safe_cursor_type_t<InputRange>>
        operator()(InputRange && in, UnaryPredicate pred) const
        {
            auto result = difference_type_t<safe_cursor_type_t<InputRange>>{0};

            auto cur = sayan::cursor(std::forward<InputRange>(in));

            for(; !!cur; ++ cur)
            {
                if(pred(*cur))
                {
                    ++ result;
                }
            }

            return result;
        }
    };

    struct count_fn
    {
        template <class InputRange, class T>
        difference_type_t<safe_cursor_type_t<InputRange>>
        operator()(InputRange && in, T const & value) const
        {
            auto cur = ::sayan::cursor(std::forward<InputRange>(in));

            using Ref = decltype(*cur);
            return ::sayan::count_if_fn{}(std::move(cur),
                                          [&value](Ref x){return x == value;});
        }
    };

    struct mismatch_fn
    {
        template <class InputRange1, class InputRange2,
                  class BinaryPredicate = std::equal_to<>>
        std::pair<safe_cursor_type_t<InputRange1>, safe_cursor_type_t<InputRange2>>
        operator()(InputRange1 && in1, InputRange2 && in2,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur1 = sayan::cursor(std::forward<InputRange1>(in1));
            auto cur2 = sayan::cursor(std::forward<InputRange2>(in2));

            for(; !!cur1 && !!cur2 && bin_pred(*cur1, *cur2); ++cur1, ++cur2)
            {}

            return {std::move(cur1), std::move(cur2)};
        }
    };

    struct equal_fn
    {
        template <class InputRange1, class InputRange2,
                  class BinaryPredicate = std::equal_to<>>
        bool operator()(InputRange1 && in1, InputRange2 && in2,
                        BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto r = ::sayan::mismatch_fn{}(std::forward<InputRange1>(in1),
                                            std::forward<InputRange2>(in2),
                                            std::move(bin_pred));

            return !r.first && !r.second;
        }
    };

    struct is_partitioned_fn
    {
        template <class InputRange, class UnaryPredicate>
        bool operator()(InputRange && in, UnaryPredicate pred) const
        {
            auto cur = ::sayan::find_if_not_fn{}(std::forward<InputRange>(in),
                                                 pred);
            cur = ::sayan::find_if_fn{}(std::move(cur), pred);

            return !cur;
        }
    };

    struct includes_fn
    {
        template <class InputRange1, class InputRange2,
                  class Compare = std::less<>>
        bool operator()(InputRange1 && in1, InputRange2 && in2,
                        Compare cmp = Compare{}) const
        {
            auto cur1 = sayan::cursor(std::forward<InputRange1>(in1));
            auto cur2 = sayan::cursor(std::forward<InputRange2>(in2));

            for(; !!cur1 && !!cur2;)
            {
                if(cmp(*cur1, *cur2))
                {
                    ++ cur1;
                }
                else if(cmp(*cur2, *cur1))
                {
                    return false;
                }
                else
                {
                    ++ cur1;
                    ++ cur2;
                }
            }

            return !cur2;
        }
    };

    struct lexicographical_compare_fn
    {
        template <class InputRange1, class InputRange2,
                  class Compare = std::less<>>
        bool operator()(InputRange1 && in1, InputRange2 && in2,
                        Compare cmp = Compare{}) const
        {
            auto cur1 = sayan::cursor(std::forward<InputRange1>(in1));
            auto cur2 = sayan::cursor(std::forward<InputRange2>(in2));

            for(; !!cur1 && !!cur2; ++ cur1, ++ cur2)
            {
                if(cmp(*cur1, *cur2))
                {
                    return true;
                }
                else if(cmp(*cur2, *cur1))
                {
                    return false;
                }
            }

            return !!cur2;
        }
    };

    struct fill_fn
    {
        template <class OutputRange, class T>
        void operator()(OutputRange && out, T const & value) const
        {
            auto cur = sayan::cursor(std::forward<OutputRange>(out));

            for(; !!cur; ++ cur)
            {
                *cur = value;
            }
        }
    };

    struct generate_fn
    {
        template <class OutputRange, class Generator>
        void operator()(OutputRange && out, Generator gen) const
        {
            auto cur = sayan::cursor(std::forward<OutputRange>(out));

            for(; !!cur; ++ cur)
            {
                *cur = gen();
            }
        }
    };

    namespace
    {
        constexpr auto const & all_of = static_const<all_of_fn>;
        constexpr auto const & any_of = static_const<any_of_fn>;
        constexpr auto const & none_of = static_const<none_of_fn>;

        constexpr auto const & for_each = static_const<for_each_fn>;

        constexpr auto const & count = static_const<count_fn>;
        constexpr auto const & count_if = static_const<count_if_fn>;

        constexpr auto const & mismatch = static_const<mismatch_fn>;
        constexpr auto const & equal = static_const<equal_fn>;

        constexpr auto const & find = static_const<find_fn>;
        constexpr auto const & find_if = static_const<find_if_fn>;
        constexpr auto const & find_if_not = static_const<find_if_not_fn>;

        constexpr auto const & fill = static_const<fill_fn>;
        constexpr auto const & generate = static_const<generate_fn>;

        constexpr auto const & is_partitioned = static_const<is_partitioned_fn>;

        constexpr auto const & includes = static_const<includes_fn>;

        constexpr auto const & lexicographical_compare
            = static_const<lexicographical_compare_fn>;
    }
}
//namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_ALGORITHM_HPP_INCLUDED
