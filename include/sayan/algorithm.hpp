#ifndef Z_SAYAN_ALGORITHM_HPP_INCLUDED
#define Z_SAYAN_ALGORITHM_HPP_INCLUDED

#include <sayan/cursor/defs.hpp>
#include <sayan/cursor/sequence_to_cursor.hpp>
#include <sayan/cursor/reverse.hpp>

#include <cassert>
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
        template <class InputSequence, class T, class BinaryPredicate = std::equal_to<>>
        safe_cursor_type_t<InputSequence>
        operator()(InputSequence && in, T const & value,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur = sayan::cursor_fwd<InputSequence>(in);

            using Ref = decltype(*cur);
            return ::sayan::find_if_fn{}(std::move(cur),
                                         [&value, &bin_pred](Ref x){ return bin_pred(x, value); });
        }
    };

    struct find_first_of_fn
    {
        template <class InputSequence, class ForwardSequence,
                  class BinaryPredicate = std::equal_to<>>
        safe_cursor_type_t<InputSequence>
        operator()(InputSequence && in, ForwardSequence && s,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto in_cur = ::sayan::cursor_fwd<InputSequence>(in);
            auto const s_cur = ::sayan::cursor_fwd<ForwardSequence>(s);

            for(; !!in_cur; ++in_cur)
            {
                if(!!::sayan::find_fn{}(s_cur, *in_cur, bin_pred))
                {
                    return in_cur;
                }
            }

            return in_cur;
        }
    };

    struct adjacent_find_fn
    {
        template <class ForwardSequence, class BinaryPredicate = std::equal_to<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto c1 = ::sayan::cursor_fwd<ForwardSequence>(seq);

            if(!c1)
            {
                return c1;
            }

            auto c2 = c1;
            ++ c2;

            for(; !!c2; ++c2)
            {
                if(bin_pred(*c1, *c2))
                {
                    return c1;
                }

                c1 = c2;
            }

            return c2;
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
        template <class InputSequence, class T, class BinaryPredicate = std::equal_to<>>
        difference_type_t<safe_cursor_type_t<InputSequence>>
        operator()(InputSequence && in, T const & value,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur = ::sayan::cursor_fwd<InputSequence>(in);

            using Ref = decltype(*cur);
            return ::sayan::count_if_fn{}(std::move(cur),
                                          [&value, &bin_pred](Ref x){return bin_pred(x, value);});
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

    struct search_fn
    {
        template <class ForwardSequence1, class ForwardSequence2,
                  class BinaryPredicate = std::equal_to<>>
        safe_cursor_type_t<ForwardSequence1>
        operator()(ForwardSequence1 && where, ForwardSequence2 && what,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur = sayan::cursor_fwd<ForwardSequence1>(where);
            auto const s_cur = sayan::cursor_fwd<ForwardSequence2>(what);

            for(; !!cur; ++ cur)
            {
                auto t = ::sayan::mismatch_fn{}(s_cur, cur, bin_pred);

                if(!::std::get<0>(t))
                {
                    break;
                }
            }

            return cur;
        }
    };

    struct search_n_fn
    {
        template <class ForwardSequence, class Size, class T,
                  class BinaryPredicate = std::equal_to<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, Size const n, T const & value,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur = sayan::cursor_fwd<ForwardSequence>(seq);
            auto k = n;

            auto i = cur;
            for(; !!i; ++ i)
            {
                if(k == 0)
                {
                    return cur;
                }

                if(!bin_pred(*i, value))
                {
                    cur = i;
                    ++ cur;
                    k = n;
                }
                else
                {
                    -- k;
                }
            }

            return i;
        }
    };

    struct find_end_fn
    {
        template <class ForwardSequence1, class ForwardSequence2,
                  class BinaryPredicate = std::equal_to<>>
        safe_cursor_type_t<ForwardSequence1>
        operator()(ForwardSequence1 && where, ForwardSequence2 && what,
                   BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur = sayan::cursor_fwd<ForwardSequence1>(where);
            auto const s_cur = sayan::cursor_fwd<ForwardSequence2>(what);

            if(!s_cur)
            {
                cur.exhaust(::sayan::front_fn{});

                return cur;
            }

            auto result = ::sayan::search_fn{}(std::move(cur), s_cur);

            if(!result)
            {
                return result;
            }

            cur = result;
            ++ cur;

            for (;;)
            {
                auto pre = cur.traversed(sayan::front);

                cur = ::sayan::search_fn{}(std::move(cur), s_cur);

                if(!cur)
                {
                    break;
                }

                pre.splice(cur.traversed(sayan::front));
                pre.exhaust(sayan::front);
                pre.splice(std::move(cur));
                cur = std::move(pre);

                result = cur;
                ++ cur;
            }

            return result;
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

    struct fill_n_fn
    {
        template <class OutputSequence, class Size, class T>
        safe_cursor_type_t<OutputSequence>
        operator()(OutputSequence && out, Size n, T const & value) const
        {
            auto cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!cur && n > 0; -- n)
            {
                cur << value;
            }

            return cur;
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

    struct generate_n_fn
    {
        template <class OutputSequence, class Size, class Generator>
        safe_cursor_type_t<OutputSequence>
        operator()(OutputSequence && out, Size n, Generator gen) const
        {
            auto cur = sayan::cursor_fwd<OutputSequence>(out);

            for(; !!cur && n > 0; -- n)
            {
                cur << gen();
            }

            return cur;
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

    struct copy_n_fn
    {
        template <class InputSequence, class Size, class OutputSequence>
        std::pair<safe_cursor_type_t<InputSequence>,
                  safe_cursor_type_t<OutputSequence>>
        operator()(InputSequence && in, Size n, OutputSequence && out) const
        {
            auto in_cur = ::sayan::cursor_fwd<InputSequence>(in);
            auto out_cur = ::sayan::cursor_fwd<OutputSequence>(out);

            for(; !!in_cur && !!out_cur && n > 0; ++ in_cur)
            {
                out_cur << *in_cur;

                -- n;
            }

            return {std::move(in_cur), std::move(out_cur)};
        }
    };

    struct copy_backward_fn
    {
        template <class BidirectionalSequence1, class BidirectionalSequence2>
        std::pair<safe_cursor_type_t<BidirectionalSequence1>,
                  safe_cursor_type_t<BidirectionalSequence2>>
        operator()(BidirectionalSequence1 && in, BidirectionalSequence2 && out) const
        {
            auto r = ::sayan::copy_fn{}(::sayan::make_reverse_cursor(std::forward<BidirectionalSequence1>(in)),
                                        ::sayan::make_reverse_cursor(std::forward<BidirectionalSequence2>(out)));
            return {std::move(r.first).base(), std::move(r.second).base()};
        }
    };

    struct move_backward_fn
    {
        template <class BidirectionalSequence1, class BidirectionalSequence2>
        std::pair<safe_cursor_type_t<BidirectionalSequence1>,
                  safe_cursor_type_t<BidirectionalSequence2>>
        operator()(BidirectionalSequence1 && in, BidirectionalSequence2 && out) const
        {
            auto r = ::sayan::move_fn{}(::sayan::make_reverse_cursor(std::forward<BidirectionalSequence1>(in)),
                                        ::sayan::make_reverse_cursor(std::forward<BidirectionalSequence2>(out)));
            return {std::move(r.first).base(), std::move(r.second).base()};
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

    struct remove_if_fn
    {
        template <class ForwardSequence, class UnaryPredicate>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, UnaryPredicate pred) const
        {
            auto out = find_if_fn{}(std::forward<ForwardSequence>(seq), pred);

            if(!out)
            {
                return out;
            }

            auto cur = out;
            ++ cur;

            for(; !!cur; ++ cur)
            {
                if(!pred(*cur))
                {
                    out << std::move(*cur);
                }
            }

            return out;
        }
    };

    struct remove_fn
    {
        template <class ForwardSequence, class T>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, T const & value) const
        {
            auto cur = sayan::cursor_fwd<ForwardSequence>(seq);
            using Ref = decltype(*cur);
            return ::sayan::remove_if_fn{}(std::move(cur),
                                           [&value](Ref x){ return x == value; });
        }
    };

    struct replace_if_fn
    {
        template <class ForwardSequence, class UnaryPredicate, class T>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, UnaryPredicate pred, T const & new_value) const
        {
            auto cur = ::sayan::cursor_fwd<ForwardSequence>(seq);
            for(; !!cur; ++ cur)
            {
                if(pred(*cur))
                {
                    *cur = new_value;
                }
            }
            return cur;
        }
    };

    struct replace_fn
    {
        template <class ForwardSequence, class T>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, T const & old_value, T const & new_value) const
        {
            auto cur = ::sayan::cursor_fwd<ForwardSequence>(seq);
            using Ref = decltype(*cur);
            return ::sayan::replace_if_fn{}(std::move(cur),
                                            [&old_value](Ref x) { return x == old_value; },
                                            new_value);
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

    struct swap_ranges_fn
    {
        template <class ForwardSequence1, class ForwardSequence2>
        std::pair<safe_cursor_type_t<ForwardSequence1>, safe_cursor_type_t<ForwardSequence2>>
        operator()(ForwardSequence1 && s1, ForwardSequence2 && s2) const
        {
            auto cur1 = ::sayan::cursor_fwd<ForwardSequence1>(s1);
            auto cur2 = ::sayan::cursor_fwd<ForwardSequence2>(s2);

            for(;!!cur1 && !!cur2; ++ cur1, ++ cur2)
            {
                ::sayan::cursor_swap(cur1, cur2);
            }

            return {std::move(cur1), std::move(cur2)};
        }
    };

    struct reverse_copy_fn
    {
        template <class BidirectionalSequence, class OutputSequence>
        std::pair<safe_cursor_type_t<BidirectionalSequence>, safe_cursor_type_t<OutputSequence>>
        operator()(BidirectionalSequence && in_seq, OutputSequence && out_seq) const
        {
            auto in = ::sayan::cursor_fwd<BidirectionalSequence>(in_seq);

            auto r = ::sayan::copy_fn{}(::sayan::make_reverse_cursor(std::move(in)),
                                        ::std::forward<OutputSequence>(out_seq));

            return {std::move(r.first).base(), std::move(r.second)};
        }
    };

    struct reverse_fn
    {
        template <class BidirectionalSequence>
        void
        operator()(BidirectionalSequence && seq) const
        {
            auto cur = ::sayan::cursor_fwd<BidirectionalSequence>(seq);

            for(; !!cur;)
            {
                auto p = ::sayan::next(cur);

                if(!p)
                {
                    break;
                }

                ::sayan::cursor_swap(cur, p, sayan::front, sayan::back);

                p.drop(sayan::back);

                cur = p;
            }
        }
    };

    struct rotate_fn
    {
    public:
        template <class ForwardCursor>
        ForwardCursor
        operator()(ForwardCursor cur) const
        {
            auto cur1 = cur.traversed(::sayan::front_fn{});

            if(!cur1)
            {
                cur.exhaust(::sayan::front_fn{});
                return cur;
            }

            if(!cur)
            {
                return cur1;
            }

            auto cur2 = cur;
            cur2.forget(sayan::front_fn{});

            return this->rotate_forward_nontrivial(std::move(cur1), std::move(cur2));
        }

    private:
        template <class ForwardCursor>
        void rotate_forward_step(ForwardCursor & cur1, ForwardCursor & cur2) const
        {
            do
            {
                ::sayan::cursor_swap(cur1, cur2);
                ++ cur1;
                ++ cur2;

                if(!cur1)
                {
                    cur1.splice(cur2.traversed(::sayan::front));
                    cur2.forget(::sayan::front);
                }
            }
            while(!!cur2);
            cur2 = std::move(cur2).traversed(::sayan::front);
        }

        template <class ForwardCursor>
        ForwardCursor rotate_forward_nontrivial(ForwardCursor cur1, ForwardCursor cur2) const
        {
            this->rotate_forward_step(cur1, cur2);

            auto result = cur1;
            result.splice(cur2);

            for(; !!cur2;)
            {
                this->rotate_forward_step(cur1, cur2);
            }

            return result;
        }
    };

    struct rotate_copy_fn
    {
        template <class ForwardCursor, class OutputSequence>
        std::pair<ForwardCursor, safe_cursor_type_t<OutputSequence>>
        operator()(ForwardCursor in, OutputSequence && out) const
        {
            auto out_cur = ::sayan::cursor_fwd<OutputSequence>(out);
            auto in2 = in.traversed(sayan::front);

            for(; !!in && !!out_cur; ++ in)
            {
                out_cur << *in;
            }

            for(; !!in2 && !!out_cur; ++ in2)
            {
                out_cur << *in2;
            }

            return {std::move(in), std::move(out_cur)};
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

    struct unique_fn
    {
        template <class ForwardSequence, class BinaryPredicate = std::equal_to<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto out = adjacent_find_fn{}(std::forward<ForwardSequence>(seq), bin_pred);

            if(!out)
            {
                return out;
            }

            auto cur = out;
            ++ cur;
            assert(!!out);
            ++ cur;

            for(; !!cur; ++ cur)
            {
                if(!bin_pred(*out, *cur))
                {
                    ++ out;
                    *out = std::move(*cur);
                }
            }

            ++ out;
            return out;
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

    struct partition_fn
    {
        template <class ForwardSequence, class UnaryPredicate>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, UnaryPredicate pred) const
        {
            auto cur_false = ::sayan::find_if_not_fn{}(std::forward<ForwardSequence>(seq), pred);

            if(!cur_false)
            {
                return cur_false;
            }

            for(auto cur = sayan::next(cur_false); !!cur; ++ cur)
            {
                if(pred(*cur))
                {
                    ::sayan::cursor_swap(cur_false, cur);
                    ++cur_false;
                }
            }

            return cur_false;
        }
    };

    struct stable_partition_fn
    {
    public:
        template <class BidirectionalSequence, class UnaryPredicate>
        safe_cursor_type_t<BidirectionalSequence>
        operator()(BidirectionalSequence && seq, UnaryPredicate pred) const
        {
            auto cur = ::sayan::cursor_fwd<BidirectionalSequence>(seq);

            if(!cur)
            {
                return cur;
            }

            return this->impl_not_empty(std::move(cur), ::sayan::size(cur), std::move(pred));
        }

    private:
        template <class BiCursor, class N, class UnaryPredicate>
        BiCursor
        impl_not_empty(BiCursor cur, N n, UnaryPredicate pred) const
        {
            if(n == N(1))
            {
                if(pred(*cur))
                {
                    ++ cur;
                }
                return cur;
            }

            auto const n_half = n / 2;

            ::sayan::advance(cur, n_half);

            // Разбиваем половины
            auto const r1 = this->impl_not_empty(cur.traversed(::sayan::front), n_half, pred);
            auto const r2 = this->impl_not_empty(::sayan::cursor(cur), n - n_half, pred);

            // Вращаем
            auto r = cursor_from_parts(r1, r2.traversed(::sayan::front));

            r = ::sayan::rotate_fn{}(r);

            // Формируем результат
            auto result = r1.traversed(::sayan::front);
            result.splice(r.traversed(::sayan::front));
            result.exhaust(::sayan::front);
            result.splice(r);
            result.splice(r2);

            return result;
        }
    };

    struct partition_point_fn
    {
        template <class ForwardSequence, class UnaryPredicate>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, UnaryPredicate pred) const
        {
            return ::sayan::find_if_not_fn{}(std::forward<ForwardSequence>(seq), std::move(pred));
        }
    };

    struct is_sorted_until_fn
    {
        template <class ForwardSequence, class Compare = std::less<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::cursor_fwd<ForwardSequence>(seq);
            using Ref = decltype(*cur);

            cur = ::sayan::adjacent_find_fn{}(std::move(cur),
                                              [&cmp](Ref x, Ref y) { return cmp(y, x); });

            if(!!cur)
            {
                ++ cur;
            }

            return cur;
        }
    };

    struct is_sorted_fn
    {
        template <class ForwardSequence, class Compare = std::less<>>
        bool operator()(ForwardSequence && seq, Compare cmp = Compare{}) const
        {
            return !::sayan::is_sorted_until_fn{}(std::forward<ForwardSequence>(seq),
                                                  std::move(cmp));
        }
    };

    struct lower_bound_fn
    {
        template <class ForwardSequence, class T, class Compare = std::less<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, T const & value, Compare cmp = Compare{}) const
        {
            auto const pred = [&value, &cmp](auto const & x) { return cmp(x, value); };

            return ::sayan::partition_point_fn{}(std::forward<ForwardSequence>(seq), pred);
        }
    };

    struct upper_bound_fn
    {
        template <class ForwardSequence, class T, class Compare = std::less<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, T const & value, Compare cmp = Compare{}) const
        {
            auto const pred = [&value, &cmp](auto const & x) { return !cmp(value, x); };

            return ::sayan::partition_point_fn{}(std::forward<ForwardSequence>(seq), pred);
        }
    };

    struct equal_range_fn
    {
        template <class ForwardSequence, class T, class Compare = std::less<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, T const & value, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::upper_bound_fn{}(std::forward<ForwardSequence>(seq), value, cmp);
            cur = cur.traversed(sayan::front);
            return ::sayan::lower_bound_fn{}(std::move(cur), value, std::move(cmp));
        }
    };

    struct binary_search_fn
    {
        template <class ForwardSequence, class T, class Compare = std::less<>>
        bool operator()(ForwardSequence && seq, T const & value, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::lower_bound_fn{}(std::forward<ForwardSequence>(seq), value,
                                                 std::move(cmp));
            return !!cur && !cmp(value, *cur);
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

    struct inplace_merge_fn
    {
    public:
        template <class BidirectionalCursor, class Compare = std::less<>>
        void operator()(BidirectionalCursor cur, Compare cmp = Compare{}) const
        {
            if(!cur)
            {
                return;
            }

            auto c1 = cur.traversed(sayan::front);

            if(!c1)
            {
                return;
            }

            auto const n1 = ::sayan::size(c1);

            auto c2 = ::sayan::cursor(std::move(cur));
            auto const n2 = ::sayan::size(c2);

            return this->impl(c1, n1, c2, n2, std::move(cmp));
        }

    private:
        template <class Cursor, class N, class Compare>
        void impl(Cursor c1, N n1, Cursor c2, N n2, Compare cmp) const
        {
            if(n1 == 1 && n2 == 1)
            {
                if(cmp(*c2, *c1))
                {
                    ::sayan::cursor_swap(c2, c1);
                    return;
                }
            }

            // Находим точки разбиения
            if(n1 > n2)
            {
                auto const n1_half = n1 / 2;
                ::sayan::advance(c1, n1_half);

                c2 = ::sayan::lower_bound_fn{}(c2, *c1, cmp);
            }
            else
            {
                auto const n2_half = n2 / 2;
                ::sayan::advance(c2, n2_half);

                c1 = ::sayan::lower_bound_fn{}(c1, *c2, cmp);
            }

            auto r = rotate_fn{}(cursor_from_parts(c1, c2.traversed(::sayan::front)));

            (*this)(cursor_from_parts(c1.traversed(::sayan::front), r.traversed(::sayan::front)), cmp);
            (*this)(cursor_from_parts(r, c2), cmp);
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

    namespace details
    {
        template <class IntType>
        IntType heap_parent(IntType index)
        {
            return (index - 1) / 2;
        }

        template <class RandomAccessCursor, class Size, class Compare>
        void heap_sink(RandomAccessCursor cur, Size pos, Size n, Compare cmp)
        {
            for(;;)
            {
                auto c = 2*pos+1;

                if(c >= n)
                {
                    return;
                }

                auto const c2 = c+1;

                if(c2 < n && !cmp(cur[c2], cur[c]))
                {
                    c = c2;
                }

                if(cmp(cur[c], cur[pos]))
                {
                    return;
                }

                ::sayan::cursor_swap(cur, cur, pos, c);
                pos = c;
            }
        }
    }
    // namespace details

    struct is_heap_until_fn
    {
        template <class RandomAccessSequence, class Compare = std::less<>>
        safe_cursor_type_t<RandomAccessSequence>
        operator()(RandomAccessSequence && seq, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::cursor_fwd<RandomAccessSequence>(seq);

            if(!cur)
            {
                return cur;
            }

            auto const n = ::sayan::size(cur);

            auto i = 1 + 0*n;
            for(; i < n; ++ i)
            {
                if(cmp(cur[details::heap_parent(i)], cur[i]))
                {
                    break;
                }
            }

            cur += i;
            return cur;
        }
    };

    struct is_heap_fn
    {
        template <class RandomAccessSequence, class Compare = std::less<>>
        bool operator()(RandomAccessSequence && seq, Compare cmp = Compare{}) const
        {
            return !is_heap_until_fn{}(std::forward<RandomAccessSequence>(seq), std::move(cmp));
        }
    };

    struct push_heap_fn
    {
        template <class RandomAccessSequence, class Compare = std::less<>>
        void operator()(RandomAccessSequence && seq, Compare cmp = Compare{}) const
        {
            auto const cur = ::sayan::cursor_fwd<RandomAccessSequence>(seq);

            if(!cur)
            {
                return;
            }

            auto pos = ::sayan::size(cur) - 1;

            for(; pos != 0; )
            {
                auto const parent = details::heap_parent(pos);

                if(cmp(cur[parent], cur[pos]))
                {
                    ::sayan::cursor_swap(cur, cur, parent, pos);
                }

                pos = parent;
            }
        }
    };

    struct pop_heap_fn
    {
        template <class RandomAccessSequence, class Compare = std::less<>>
        void operator()(RandomAccessSequence && seq, Compare cmp = Compare{}) const
        {
            auto const cur = ::sayan::cursor_fwd<RandomAccessSequence>(seq);

            if(!cur)
            {
                return;
            }

            auto const n = ::sayan::size(cur);
            if(n == 1)
            {
                return;
            }

            ::sayan::cursor_swap(cur, cur, 0*n, n-1);

            details::heap_sink(cur, 0*n, n-1, cmp);
        }
    };

    struct make_heap_fn
    {
        template <class RandomAccessSequence, class Compare = std::less<>>
        void operator()(RandomAccessSequence && seq, Compare cmp = Compare{}) const
        {
            auto const cur = ::sayan::cursor_fwd<RandomAccessSequence>(seq);

            if(!cur)
            {
                return;
            }

            auto const n = ::sayan::size(cur);

            for(auto i = details::heap_parent(n+1); i > 0; -- i)
            {
                details::heap_sink(cur, i-1, n, cmp);
            }
        }
    };

    struct sort_heap_fn
    {
        template <class RandomAccessSequence, class Compare = std::less<>>
        void operator()(RandomAccessSequence && seq, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::cursor_fwd<RandomAccessSequence>(seq);

            for(; !!cur; cur.drop(sayan::back_fn{}))
            {
                ::sayan::pop_heap_fn{}(cur, cmp);
            }
        }
    };

    struct partial_sort_copy_fn
    {
        template <class InputSequence, class RandomAccessSequence, class Compare = std::less<>>
        std::pair<safe_cursor_type_t<InputSequence>, safe_cursor_type_t<RandomAccessSequence>>
        operator()(InputSequence && in_seq, RandomAccessSequence && out_seq,
                   Compare cmp = Compare{}) const
        {
            auto in = ::sayan::cursor_fwd<InputSequence>(in_seq);
            auto out = ::sayan::cursor_fwd<RandomAccessSequence>(out_seq);

            if(!out || !in)
            {
                return {std::move(in), std::move(out)};
            }

            std::tie(in, out) = sayan::copy_fn{}(std::move(in), std::move(out));

            auto h = out.traversed(::sayan::front);
            auto const n = ::sayan::size(h);
            ::sayan::make_heap_fn{}(h, cmp);

            for(; !!in; ++ in)
            {
                if(cmp(*in, *h))
                {
                    *h = *in;
                    details::heap_sink(h, 0*n, n, cmp);
                }
            }

            ::sayan::sort_heap_fn{}(std::move(h), cmp);

            return {std::move(in), std::move(out)};
        }
    };

    struct partial_sort_fn
    {
        template <class RandomAccessCursor, class Compare = std::less<>>
        void operator()(RandomAccessCursor cur, Compare cmp = Compare{}) const
        {
            auto h = cur.traversed(sayan::front);

            if(!h)
            {
                return;
            }

            auto const n = ::sayan::size(h);

            ::sayan::make_heap_fn{}(h, cmp);

            for(; !!cur; ++ cur)
            {
                if(cmp(*cur, *h))
                {
                    ::sayan::cursor_swap(cur, h);
                    details::heap_sink(h, 0*n, n, cmp);
                }
            }

            ::sayan::sort_heap_fn{}(std::move(h), std::move(cmp));
        }
    };

    struct sort_fn
    {
        template <class RandomAccessSequence, class Compare = std::less<>>
        void operator()(RandomAccessSequence && seq, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::cursor_fwd<RandomAccessSequence>(seq);

            cur.exhaust(sayan::front);

            ::sayan::partial_sort_fn{}(std::move(cur), std::move(cmp));
        }
    };

    struct min_element_fn
    {
        template <class ForwardSequence, class Compare = std::less<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, Compare cmp = Compare{}) const
        {
            auto result = ::sayan::cursor_fwd<ForwardSequence>(seq);

            if(!result)
            {
                return result;
            }

            auto cur = result;
            ++ cur;

            for(; !!cur; ++ cur)
            {
                if(cmp(*cur, *result))
                {
                    result = cur;
                }
            }

            return result;
        }
    };

    struct max_element_fn
    {
        template <class ForwardSequence, class Compare = std::less<>>
        safe_cursor_type_t<ForwardSequence>
        operator()(ForwardSequence && seq, Compare cmp = Compare{}) const
        {
            auto result = ::sayan::cursor_fwd<ForwardSequence>(seq);

            if(!result)
            {
                return result;
            }

            auto cur = result;
            ++ cur;

            for(; !!cur; ++ cur)
            {
                if(!cmp(*cur, *result))
                {
                    result = cur;
                }
            }

            return result;
        }
    };

    struct minmax_element_fn
    {
        template <class ForwardSequence, class Compare = std::less<>>
        std::pair<safe_cursor_type_t<ForwardSequence>, safe_cursor_type_t<ForwardSequence>>
        operator()(ForwardSequence && seq, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::cursor_fwd<ForwardSequence>(seq);
            return {sayan::min_element_fn{}(cur, cmp), sayan::max_element_fn{}(cur, cmp)};
        }
    };

    struct is_permutation_fn
    {
        template <class ForwardSequence1, class ForwardSequence2,
                  class BinaryPredicate = std::equal_to<>>
        bool operator()(ForwardSequence1 && in1, ForwardSequence2 && in2,
                        BinaryPredicate bin_pred = BinaryPredicate{}) const
        {
            auto cur1 = ::sayan::cursor_fwd<ForwardSequence1>(in1);
            auto cur2 = ::sayan::cursor_fwd<ForwardSequence2>(in2);

            std::tie(cur1, cur2)
                = ::sayan::mismatch_fn{}(std::move(cur1), std::move(cur2), bin_pred);

            if(::sayan::size_fn{}(cur1) != ::sayan::size_fn{}(cur2))
            {
                return false;
            }

            for(auto i = cur1; !!i; ++ i)
            {
                auto const n1 = ::sayan::count_fn{}(cur1, *i, bin_pred);
                auto const n2 = ::sayan::count_fn{}(cur2, *i, bin_pred);

                if(n1 != n2)
                {
                    return false;
                }
            }

            return true;
        }
    };

    struct next_permutation_fn
    {
        template <class BidirectionalSequence, class Compare = std::less<>>
        bool operator()(BidirectionalSequence && seq, Compare cmp = Compare{}) const
        {
            auto cur = ::sayan::make_reverse_cursor(std::forward<BidirectionalSequence>(seq));

            if(!cur)
            {
                return false;
            }

            auto j = is_sorted_until_fn{}(cur, cmp);

            if(!j)
            {
                reverse_fn{}(cur.base());
                return false;
            }

            using Ref = decltype(*cur);
            auto i = find_if_fn{}(cur, [&](Ref x) { return cmp(*j, x); });

            ::sayan::cursor_swap(j, i);

            ::sayan::reverse_fn{}(j.traversed(sayan::front_fn{}).base());

            return true;
        }
    };

    struct prev_permutation_fn
    {
        template <class BidirectionalSequence, class Compare = std::less<>>
        bool operator()(BidirectionalSequence && seq, Compare cmp = Compare{}) const
        {
            return ::sayan::next_permutation_fn{}(std::forward<BidirectionalSequence>(seq),
                                                  std::experimental::not_fn(std::move(cmp)));
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

        constexpr auto const & find_end = static_const<find_end_fn>;
        constexpr auto const & find_first_of = static_const<find_first_of_fn>;

        constexpr auto const & adjacent_find = static_const<adjacent_find_fn>;

        constexpr auto const & search = static_const<search_fn>;
        constexpr auto const & search_n = static_const<search_n_fn>;

        constexpr auto const & copy = static_const<copy_fn>;
        constexpr auto const & copy_if = static_const<copy_if_fn>;
        constexpr auto const & copy_n = static_const<copy_n_fn>;

        constexpr auto const & move = static_const<move_fn>;

        constexpr auto const & copy_backward = static_const<copy_backward_fn>;
        constexpr auto const & move_backward = static_const<move_backward_fn>;

        constexpr auto const & transform = static_const<transform_fn>;

        constexpr auto const & fill = static_const<fill_fn>;
        constexpr auto const & fill_n = static_const<fill_n_fn>;
        constexpr auto const & generate = static_const<generate_fn>;
        constexpr auto const & generate_n = static_const<generate_n_fn>;

        constexpr auto const & remove = static_const<remove_fn>;
        constexpr auto const & remove_if = static_const<remove_if_fn>;
        constexpr auto const & remove_copy = static_const<remove_copy_fn>;
        constexpr auto const & remove_copy_if = static_const<remove_copy_if_fn>;

        constexpr auto const & replace = static_const<replace_fn>;
        constexpr auto const & replace_if = static_const<replace_if_fn>;
        constexpr auto const & replace_copy = static_const<replace_copy_fn>;
        constexpr auto const & replace_copy_if = static_const<replace_copy_if_fn>;

        constexpr auto const & swap_ranges = static_const<swap_ranges_fn>;

        constexpr auto const & reverse = static_const<reverse_fn>;
        constexpr auto const & reverse_copy = static_const<reverse_copy_fn>;

        constexpr auto const & rotate = static_const<rotate_fn>;
        constexpr auto const & rotate_copy = static_const<rotate_copy_fn>;

        constexpr auto const & unique = static_const<unique_fn>;
        constexpr auto const & unique_copy = static_const<unique_copy_fn>;

        constexpr auto const & is_partitioned = static_const<is_partitioned_fn>;
        constexpr auto const & partition = static_const<partition_fn>;
        constexpr auto const & partition_copy = static_const<partition_copy_fn>;
        constexpr auto const & partition_point = static_const<partition_point_fn>;
        constexpr auto const & stable_partition = static_const<stable_partition_fn>;

        constexpr auto const & is_sorted = static_const<is_sorted_fn>;
        constexpr auto const & is_sorted_until = static_const<is_sorted_until_fn>;

        constexpr auto const & sort = static_const<sort_fn>;

        constexpr auto const & partial_sort = static_const<partial_sort_fn>;
        constexpr auto const & partial_sort_copy = static_const<partial_sort_copy_fn>;

        constexpr auto const & lower_bound = static_const<lower_bound_fn>;
        constexpr auto const & upper_bound = static_const<upper_bound_fn>;
        constexpr auto const & equal_range = static_const<equal_range_fn>;
        constexpr auto const & binary_search = static_const<binary_search_fn>;

        constexpr auto const & merge = static_const<merge_fn>;
        constexpr auto const & inplace_merge = static_const<inplace_merge_fn>;

        constexpr auto const & includes = static_const<includes_fn>;
        constexpr auto const & set_union = static_const<set_union_fn>;
        constexpr auto const & set_intersection = static_const<set_intersection_fn>;
        constexpr auto const & set_difference = static_const<set_difference_fn>;
        constexpr auto const & set_symmetric_difference = static_const<set_symmetric_difference_fn>;

        constexpr auto const & is_heap = static_const<is_heap_fn>;
        constexpr auto const & is_heap_until = static_const<is_heap_until_fn>;
        constexpr auto const & push_heap = static_const<push_heap_fn>;
        constexpr auto const & pop_heap = static_const<pop_heap_fn>;
        constexpr auto const & make_heap = static_const<make_heap_fn>;
        constexpr auto const & sort_heap = static_const<sort_heap_fn>;

        constexpr auto const & min_element = static_const<min_element_fn>;
        constexpr auto const & max_element = static_const<max_element_fn>;
        constexpr auto const & minmax_element = static_const<minmax_element_fn>;

        constexpr auto const & lexicographical_compare
            = static_const<lexicographical_compare_fn>;

        constexpr auto const & is_permutation = static_const<is_permutation_fn>;
        constexpr auto const & next_permutation = static_const<next_permutation_fn>;
        constexpr auto const & prev_permutation = static_const<prev_permutation_fn>;
    }
}
//namespace v1
}
// namespace sayan

#endif
// Z_SAYAN_ALGORITHM_HPP_INCLUDED
