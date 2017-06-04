#ifndef Z_SAYAN_ALGORITHM_HPP_INCLUDED
#define Z_SAYAN_ALGORITHM_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>
#include <sayan/cursor/sequence_to_cursor.hpp>

#include <functional>
#include <experimental/functional>

namespace sayan
{
inline namespace v1
{
    struct for_each_fn
    {
        template <class InputSequence, class UnaryFunction>
        safe_cursor_type_t<InputSequence>
        operator()(InputSequence && in, UnaryFunction fun) const
        {
            auto cur = sayan::cursor_fwd<InputSequence>(in);

            for(; !!cur; ++ cur)
            {
                fun(*cur);
            }

            return cur;
        }
    };

    struct find_if_fn
    {
        template <class InputSequence, class UnaryPredicate>
        safe_cursor_type_t<InputSequence>
        operator()(InputSequence && in, UnaryPredicate pred) const
        {
            auto cur = ::sayan::cursor_fwd<InputSequence>(in);

            for(; !!cur && !pred(*cur); ++ cur)
            {}

            return cur;
        }
    };

    struct find_if_not_fn
    {
        template <class InputSequence, class UnaryPredicate>
        safe_cursor_type_t<InputSequence>
        operator()(InputSequence && in, UnaryPredicate pred) const
        {
            auto cur = ::sayan::cursor_fwd<InputSequence>(in);

            return ::sayan::find_if_fn{}(std::move(cur),
                                         std::experimental::not_fn(std::move(pred)));
        }
    };

    struct find_fn
    {
        template <class InputSequence, class T>
        safe_cursor_type_t<InputSequence>
        operator()(InputSequence && in, T const & value) const
        {
            auto cur = sayan::cursor_fwd<InputSequence>(in);

            using Ref = decltype(*cur);
            return ::sayan::find_if_fn{}(std::move(cur),
                                         [&value](Ref x){ return x == value; });
        }
    };

    struct all_of_fn
    {
        template <class InputSequence, class UnaryPredicate>
        bool operator()(InputSequence && in, UnaryPredicate pred) const
        {
            return !::sayan::find_if_not_fn{}(std::forward<InputSequence>(in),
                                              std::move(pred));
        }
    };

    struct any_of_fn
    {
        template <class InputSequence, class UnaryPredicate>
        bool operator()(InputSequence && in, UnaryPredicate pred) const
        {
            return !!::sayan::find_if_fn{}(std::forward<InputSequence>(in),
                                           std::move(pred));
        }
    };

    struct none_of_fn
    {
        template <class InputSequence, class UnaryPredicate>
        bool operator()(InputSequence && in, UnaryPredicate pred) const
        {
            return !::sayan::any_of_fn{}(std::forward<InputSequence>(in),
                                         std::move(pred));
        }
    };

    struct count_if_fn
    {
        template <class InputSequence, class UnaryPredicate>
        difference_type_t<safe_cursor_type_t<InputSequence>>
        operator()(InputSequence && in, UnaryPredicate pred) const
        {
            auto result = difference_type_t<safe_cursor_type_t<InputSequence>>{0};

            auto cur = sayan::cursor_fwd<InputSequence>(in);

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
        template <class InputSequence, class T>
        difference_type_t<safe_cursor_type_t<InputSequence>>
        operator()(InputSequence && in, T const & value) const
        {
            auto cur = ::sayan::cursor_fwd<InputSequence>(in);

            using Ref = decltype(*cur);
            return ::sayan::count_if_fn{}(std::move(cur),
                                          [&value](Ref x){return x == value;});
        }
    };

    struct mismatch_fn
    {
        template <class InputSequence1, class InputSequence2,
                  class BinaryPredicate = std::equal_to<>>
        std::pair<safe_cursor_type_t<InputSequence1>, safe_cursor_type_t<InputSequence2>>
        operator()(InputSequence1 && in1, InputSequence2 && in2,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur1 = sayan::cursor_fwd<InputSequence1>(in1);
            auto cur2 = sayan::cursor_fwd<InputSequence2>(in2);

            for(; !!cur1 && !!cur2 && bin_pred(*cur1, *cur2); ++cur1, ++cur2)
            {}

            return {std::move(cur1), std::move(cur2)};
        }
    };

    struct equal_fn
    {
        template <class InputSequence1, class InputSequence2,
                  class BinaryPredicate = std::equal_to<>>
        bool operator()(InputSequence1 && in1, InputSequence2 && in2,
                        BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto r = ::sayan::mismatch_fn{}(std::forward<InputSequence1>(in1),
                                            std::forward<InputSequence2>(in2),
                                            std::move(bin_pred));

            return !r.first && !r.second;
        }
    };

    struct is_partitioned_fn
    {
        template <class InputSequence, class UnaryPredicate>
        bool operator()(InputSequence && in, UnaryPredicate pred) const
        {
            auto cur = ::sayan::find_if_not_fn{}(std::forward<InputSequence>(in),
                                                 pred);
            cur = ::sayan::find_if_fn{}(std::move(cur), pred);

            return !cur;
        }
    };

    struct includes_fn
    {
        template <class InputSequence1, class InputSequence2,
                  class Compare = std::less<>>
        bool operator()(InputSequence1 && in1, InputSequence2 && in2,
                        Compare cmp = Compare{}) const
        {
            auto cur1 = sayan::cursor_fwd<InputSequence1>(in1);
            auto cur2 = sayan::cursor_fwd<InputSequence2>(in2);

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
        template <class InputSequence1, class InputSequence2,
                  class Compare = std::less<>>
        bool operator()(InputSequence1 && in1, InputSequence2 && in2,
                        Compare cmp = Compare{}) const
        {
            auto cur1 = sayan::cursor_fwd<InputSequence1>(in1);
            auto cur2 = sayan::cursor_fwd<InputSequence2>(in2);

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

    struct copy_fn
    {
        template <class InputSequence, class OutputSequence>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out) const
        {
            auto in_cur = sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in_cur && !!out_cur; ++ in_cur)
            {
                out_cur << *in_cur;
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    struct move_fn
    {
        template <class InputSequence, class OutputSequence>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out) const
        {
            auto in_cur = sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in_cur && !!out_cur; ++ in_cur)
            {
                out_cur << std::move(*in_cur);
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    struct transform_fn
    {
        template <class InputSequence, class OutputSequence, class UnaryFunction>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out,
                   UnaryFunction f) const
        {
            auto in_cur = sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in_cur && !!out_cur; ++ in_cur)
            {
                out_cur << f(*in_cur);
            }

            return {std::move(in_cur), std::move(out_cur)};
        }

        template <class InputSequence1, class InputSequence2,
                    class OutputSequence, class BinaryFunction>
        std::tuple<safe_cursor_type_t<InputSequence1>,
                   safe_cursor_type_t<InputSequence2>,
                   safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence1 && in1, InputSequence2 && in2,
                   OutputSequence && out, BinaryFunction fun) const
        {
            auto in1_cur = sayan::cursor_fwd<InputSequence1>(in1);
            auto in2_cur = sayan::cursor_fwd<InputSequence2>(in2);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in1_cur && !!in2_cur && !!out_cur; ++ in1_cur, ++ in2_cur)
            {
                out_cur << fun(*in1_cur, *in2_cur);
            }

            using Result = std::tuple<safe_cursor_type_t<InputSequence1>,
                                      safe_cursor_type_t<InputSequence2>,
                                      safe_cursor_type_t<OutputSequence>>;
            return Result{std::move(in1_cur), std::move(in2_cur), std::move(out_cur)};
        }
    };

    struct fill_fn
    {
        template <class OutputSequence, class T>
        void operator()(OutputSequence && out, T const & value) const
        {
            auto cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!cur; ++ cur)
            {
                *cur = value;
            }
        }
    };

    struct generate_fn
    {
        template <class OutputSequence, class Generator>
        void operator()(OutputSequence && out, Generator gen) const
        {
            auto cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!cur; ++ cur)
            {
                *cur = gen();
            }
        }
    };

    struct copy_if_fn
    {
        template <class InputSequence, class OutputSequence,
                  class UnaryPredicate>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in,
                   OutputSequence && out, UnaryPredicate pred) const
        {
            auto in_cur = ::sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = ::sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in_cur && !!out_cur; ++ in_cur)
            {
                if(pred(*in_cur))
                {
                    out_cur << *in_cur;
                }
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    struct remove_copy_if_fn
    {
        template <class InputSequence, class OutputSequence,
                  class UnaryPredicate>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in,
                   OutputSequence && out, UnaryPredicate pred) const
        {
            return ::sayan::copy_if_fn{}(std::forward<InputSequence>(in),
                                         std::forward<OutputSequence>(out),
                                         std::experimental::not_fn(std::move(pred)));
        }
    };

        struct remove_copy_fn
        {
            template <class InputSequence, class OutputSequence, class T>
            std::pair<safe_cursor_type_t<InputSequence>,
                      safe_cursor_type_t<OutputSequence>>
            operator()(InputSequence && in,
                       OutputSequence && out, T const  & value) const
            {
                auto in_cur = ::sayan::cursor_fwd<InputSequence>(in);
                using Ref = decltype(*in_cur);

                return remove_copy_if_fn{}(std::move(in_cur),
                                           std::forward<OutputSequence>(out),
                                           [&value](Ref x) { return x == value; });
            }
        };

    struct replace_copy_if_fn
    {
        template <class InputSequence, class OutputSequence,
                  class UnaryPredicate, class T>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out,
                   UnaryPredicate pred, T const & new_value) const
        {
            auto in_cur = ::sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = ::sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in_cur && !!out_cur; ++in_cur)
            {
                if(pred(*in_cur))
                {
                    out_cur << new_value;
                }
                else
                {
                    out_cur << *in_cur;
                }
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    struct replace_copy_fn
    {
        template <class InputSequence, class OutputSequence, class T>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out,
                   T const & old_value, T const & new_value) const
        {
            auto in_cur = ::sayan::cursor_fwd<InputSequence>(in);
            using Ref = decltype(*in_cur);

            auto impl = ::sayan::replace_copy_if_fn{};
            return impl(std::move(in_cur), std::forward<OutputSequence>(out),
                        [&old_value](Ref x) { return x == old_value; },
                        new_value);
        }
    };

    struct unique_copy_fn
    {
    public:
        template <class InputSequence, class OutputSequence,
                  class BinaryPredicate = std::equal_to<>>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, OutputSequence && out,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto in_cur = ::sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = ::sayan::cursor_fwd<OutputSequence>(out);

            if(!in_cur || !out_cur)
            {
                return {std::move(in_cur), std::move(out_cur)};
            }

            auto x = *in_cur;
            ++ in_cur;
            out_cur << x;

            for(; !!in_cur && !!out_cur; ++in_cur)
            {
                if(!bin_pred(x, *in_cur))
                {
                    x = *in_cur;
                    out_cur << x;
                }
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    struct partition_copy_fn
    {
        template <class InputSequence, class OutputSequence1,
                  class OutputSequence2, class UnaryPredicate>
        std::tuple<safe_cursor_type_t<InputSequence>,
                   safe_cursor_type_t<OutputSequence1>,
                   safe_cursor_type_t<OutputSequence2>>
        operator()(InputSequence && in, OutputSequence1 && out_true,
                   OutputSequence2 && out_false, UnaryPredicate pred) const
        {
            auto in_cur = sayan::cursor_fwd<InputSequence>(in);
            auto out_true_cur = sayan::cursor_fwd<OutputSequence1>(out_true);
            auto out_false_cur = sayan::cursor_fwd<OutputSequence2>(out_false);

            for(; !!in_cur && !!out_true_cur && !!out_false_cur; ++ in_cur)
            {
                if(pred(*in_cur))
                {
                    out_true_cur << *in_cur;
                }
                else
                {
                    out_false_cur << *in_cur;
                }
            }

            using Tuple = std::tuple<safe_cursor_type_t<InputSequence>,
                                     safe_cursor_type_t<OutputSequence1>,
                                     safe_cursor_type_t<OutputSequence2>>;
            return Tuple{std::move(in_cur), std::move(out_true_cur), std::move(out_false_cur)};
        }
    };

    struct merge_fn
    {
        template <class InputSequence1, class InputSequence2,
                    class OutputSequence, class Compare = std::less<>>
        std::tuple<safe_cursor_type_t<InputSequence1>,
                   safe_cursor_type_t<InputSequence2>,
                   safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence1 && in1, InputSequence2 && in2, OutputSequence && out,
                   Compare cmp = Compare{}) const
        {
            auto in1_cur = sayan::cursor_fwd<InputSequence1>(in1);
            auto in2_cur = sayan::cursor_fwd<InputSequence2>(in2);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in1_cur && !!in2_cur && !!out_cur; )
            {
                if(cmp(*in2_cur, *in1_cur))
                {
                    out_cur << *in2_cur;
                    ++ in2_cur;
                }
                else
                {
                    out_cur << *in1_cur;
                    ++ in1_cur;
                }
            }

            auto r1 = ::sayan::copy_fn{}(std::move(in1_cur), std::move(out_cur));
            auto r2 = ::sayan::copy_fn{}(std::move(in2_cur), std::move(r1.second));

            using Tuple = std::tuple<safe_cursor_type_t<InputSequence1>,
                                     safe_cursor_type_t<InputSequence2>,
                                     safe_cursor_type_t<OutputSequence>>;
            return Tuple{std::move(r1.first), std::move(r2.first), std::move(r2.second)};
        }
    };

    struct set_union_fn
    {
        template <class InputSequence1, class InputSequence2,
                    class OutputSequence, class Compare = std::less<>>
        std::tuple<safe_cursor_type_t<InputSequence1>,
                   safe_cursor_type_t<InputSequence2>,
                   safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence1 && in1, InputSequence2 && in2, OutputSequence && out,
                   Compare cmp = Compare{}) const
        {
            auto in1_cur = sayan::cursor_fwd<InputSequence1>(in1);
            auto in2_cur = sayan::cursor_fwd<InputSequence2>(in2);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in1_cur && !!in2_cur && !!out_cur; )
            {
                if(cmp(*in2_cur, *in1_cur))
                {
                    out_cur << *in2_cur;
                    ++ in2_cur;
                }
                else if(cmp(*in1_cur, *in2_cur))
                {
                    out_cur << *in1_cur;
                    ++ in1_cur;
                }
                else
                {
                    out_cur << *in1_cur;
                    ++ in1_cur;
                    ++ in2_cur;
                }
            }

            auto r1 = ::sayan::copy_fn{}(std::move(in1_cur), std::move(out_cur));
            auto r2 = ::sayan::copy_fn{}(std::move(in2_cur), std::move(r1.second));

            using Tuple = std::tuple<safe_cursor_type_t<InputSequence1>,
                                     safe_cursor_type_t<InputSequence2>,
                                     safe_cursor_type_t<OutputSequence>>;
            return Tuple{std::move(r1.first), std::move(r2.first), std::move(r2.second)};
        }
    };

    struct set_intersection_fn
    {
        template <class InputSequence1, class InputSequence2,
                    class OutputSequence, class Compare = std::less<>>
        std::tuple<safe_cursor_type_t<InputSequence1>,
                   safe_cursor_type_t<InputSequence2>,
                   safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence1 && in1, InputSequence2 && in2, OutputSequence && out,
                   Compare cmp = Compare{}) const
        {
            auto in1_cur = sayan::cursor_fwd<InputSequence1>(in1);
            auto in2_cur = sayan::cursor_fwd<InputSequence2>(in2);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in1_cur && !!in2_cur && !!out_cur; )
            {
                if(cmp(*in2_cur, *in1_cur))
                {
                    ++ in2_cur;
                }
                else if(cmp(*in1_cur, *in2_cur))
                {
                    ++ in1_cur;
                }
                else
                {
                    out_cur << *in1_cur;
                    ++ in1_cur;
                    ++ in2_cur;
                }
            }

            auto r1 = ::sayan::copy_fn{}(std::move(in1_cur), std::move(out_cur));
            auto r2 = ::sayan::copy_fn{}(std::move(in2_cur), std::move(r1.second));

            using Tuple = std::tuple<safe_cursor_type_t<InputSequence1>,
                                     safe_cursor_type_t<InputSequence2>,
                                     safe_cursor_type_t<OutputSequence>>;
            return Tuple{std::move(r1.first), std::move(r2.first), std::move(r2.second)};
        }
    };

    struct set_difference_fn
    {
        template <class InputSequence1, class InputSequence2,
                    class OutputSequence, class Compare = std::less<>>
        std::tuple<safe_cursor_type_t<InputSequence1>,
                   safe_cursor_type_t<InputSequence2>,
                   safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence1 && in1, InputSequence2 && in2, OutputSequence && out,
                   Compare cmp = Compare{}) const
        {
            auto in1_cur = sayan::cursor_fwd<InputSequence1>(in1);
            auto in2_cur = sayan::cursor_fwd<InputSequence2>(in2);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in1_cur && !!in2_cur && !!out_cur; )
            {
                if(cmp(*in2_cur, *in1_cur))
                {
                    ++ in2_cur;
                }
                else if(cmp(*in1_cur, *in2_cur))
                {
                    out_cur << *in1_cur;
                    ++ in1_cur;
                }
                else
                {
                    ++ in1_cur;
                    ++ in2_cur;
                }
            }

            auto r1 = ::sayan::copy_fn{}(std::move(in1_cur), std::move(out_cur));
            auto r2 = ::sayan::copy_fn{}(std::move(in2_cur), std::move(r1.second));

            using Tuple = std::tuple<safe_cursor_type_t<InputSequence1>,
                                     safe_cursor_type_t<InputSequence2>,
                                     safe_cursor_type_t<OutputSequence>>;
            return Tuple{std::move(r1.first), std::move(r2.first), std::move(r2.second)};
        }
    };

    struct set_symmetric_difference_fn
    {
        template <class InputSequence1, class InputSequence2,
                    class OutputSequence, class Compare = std::less<>>
        std::tuple<safe_cursor_type_t<InputSequence1>,
                   safe_cursor_type_t<InputSequence2>,
                   safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence1 && in1, InputSequence2 && in2, OutputSequence && out,
                   Compare cmp = Compare{}) const
        {
            auto in1_cur = sayan::cursor_fwd<InputSequence1>(in1);
            auto in2_cur = sayan::cursor_fwd<InputSequence2>(in2);
            auto out_cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in1_cur && !!in2_cur && !!out_cur; )
            {
                if(cmp(*in2_cur, *in1_cur))
                {
                    out_cur << *in2_cur;
                    ++ in2_cur;
                }
                else if(cmp(*in1_cur, *in2_cur))
                {
                    out_cur << *in1_cur;
                    ++ in1_cur;
                }
                else
                {
                    ++ in1_cur;
                    ++ in2_cur;
                }
            }

            auto r1 = ::sayan::copy_fn{}(std::move(in1_cur), std::move(out_cur));
            auto r2 = ::sayan::copy_fn{}(std::move(in2_cur), std::move(r1.second));

            using Tuple = std::tuple<safe_cursor_type_t<InputSequence1>,
                                     safe_cursor_type_t<InputSequence2>,
                                     safe_cursor_type_t<OutputSequence>>;
            return Tuple{std::move(r1.first), std::move(r2.first), std::move(r2.second)};
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

        constexpr auto const & copy = static_const<copy_fn>;
        constexpr auto const & copy_if = static_const<copy_if_fn>;

        constexpr auto const & move = static_const<move_fn>;

        constexpr auto const & transform = static_const<transform_fn>;

        constexpr auto const & fill = static_const<fill_fn>;
        constexpr auto const & generate = static_const<generate_fn>;

        constexpr auto const & remove_copy = static_const<remove_copy_fn>;
        constexpr auto const & remove_copy_if = static_const<remove_copy_if_fn>;

        constexpr auto const & replace_copy = static_const<replace_copy_fn>;
        constexpr auto const & replace_copy_if = static_const<replace_copy_if_fn>;

        constexpr auto const & unique_copy = static_const<unique_copy_fn>;

        constexpr auto const & is_partitioned = static_const<is_partitioned_fn>;
        constexpr auto const & partition_copy = static_const<partition_copy_fn>;

        constexpr auto const & includes = static_const<includes_fn>;
        constexpr auto const & set_union = static_const<set_union_fn>;
        constexpr auto const & set_intersection = static_const<set_intersection_fn>;
        constexpr auto const & set_difference = static_const<set_difference_fn>;
        constexpr auto const & set_symmetric_difference = static_const<set_symmetric_difference_fn>;

        constexpr auto const & lexicographical_compare
            = static_const<lexicographical_compare_fn>;

        constexpr auto const & merge = static_const<merge_fn>;
    }
}
//namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_ALGORITHM_HPP_INCLUDED
