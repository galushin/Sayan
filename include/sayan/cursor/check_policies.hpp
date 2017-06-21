#ifndef Z_SAYAN_CURSOR_CHECK_POLICIES_HPP_INCLUDED
#define Z_SAYAN_CURSOR_CHECK_POLICIES_HPP_INCLUDED

#include <stdexcept>

namespace sayan
{
inline namespace v1
{
    struct cursor_checking_throw
    {
    public:
        template <class Cursor>
        static void ensure_not_empty(Cursor const & cur)
        {
            if(cur.empty())
            {
                throw std::logic_error("cursor must be not empty");
            }
        }

        template <class Iterator, class Sentinel>
        static void ensure_equal(Iterator i, Sentinel s)
        {
            if(i != s)
            {
                throw std::logic_error("must be equal");
            }
        }

        template <class Cursor, class D>
        static void check_index(Cursor const & cur, D index)
        {
            if(index < D{0} || cur.size() <= index)
            {
                throw std::logic_error("index is out of bounds");
            }
        }

        template <class Cursor, class D>
        static void check_step(Cursor const & cur, D step)
        {
            if(step < D{0} || cur.size() < step)
            {
                throw std::logic_error("step is out of bounds");
            }
        }
    };
}
//inline namespace v1
}
//namespace sayan

#endif
// Z_SAYAN_CURSOR_CHECK_POLICIES_HPP_INCLUDED
