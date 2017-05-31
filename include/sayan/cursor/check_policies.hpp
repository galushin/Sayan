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
    };
}
//inline namespace v1
}
//namespace sayan

#endif
// Z_SAYAN_CURSOR_CHECK_POLICIES_HPP_INCLUDED
