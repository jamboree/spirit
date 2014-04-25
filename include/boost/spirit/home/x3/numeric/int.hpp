/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_INT_APR_17_2006_0830AM)
#define BOOST_SPIRIT_X3_INT_APR_17_2006_0830AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/literal.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/support/numeric_utils/extract_int.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <cstdint>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename T
      , unsigned Radix = 10
      , unsigned MinDigits = 1
      , int MaxDigits = -1>
    struct int_parser : parser<int_parser<T, Radix, MinDigits, MaxDigits>>
    {
        // check template parameter 'Radix' for validity
        static_assert(
            (Radix == 2 || Radix == 8 || Radix == 10 || Radix == 16),
            "Error Unsupported Radix");

        typedef T attribute_type;
        static bool const has_attribute = true;
        static bool const caller_is_pass_through_unary = true;
        typedef extract_int<T, Radix, MinDigits, MaxDigits> extract;

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            x3::skip_over(first, last, context);
            return extract::call(first, last, attr);
        }
        
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, T val) const
        {
            x3::skip_over(first, last, context);
            Iterator it(first);
            T attr_;
            if (extract::call(it, last, attr_) && attr_ == val)
            {
                x3::traits::move_to(attr_, attr);
                first = it;
                return true;
            }
            return false;
        }
    };
    
    namespace extension
    {
        template <typename T>
        struct literal<T, typename enable_if<is_signed<T>>::type>
        {
            typedef int_parser<T> type;
        };
    }

#define BOOST_SPIRIT_X3_INT_PARSER(int_type, name)                              \
    typedef int_parser<int_type> name##type;                                    \
    name##type const name{};                                                    \
    /***/

    BOOST_SPIRIT_X3_INT_PARSER(long, long_)
    BOOST_SPIRIT_X3_INT_PARSER(short, short_)
    BOOST_SPIRIT_X3_INT_PARSER(int, int_)
    BOOST_SPIRIT_X3_INT_PARSER(long long, long_long)

    BOOST_SPIRIT_X3_INT_PARSER(int8_t, int8)
    BOOST_SPIRIT_X3_INT_PARSER(int16_t, int16)
    BOOST_SPIRIT_X3_INT_PARSER(int32_t, int32)
    BOOST_SPIRIT_X3_INT_PARSER(int64_t, int64)

#undef BOOST_SPIRIT_X3_INT_PARSER

}}}

#endif
