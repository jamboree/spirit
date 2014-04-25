/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2011 Jan Frederick Eick

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_UINT_APR_17_2006_0901AM)
#define BOOST_SPIRIT_X3_UINT_APR_17_2006_0901AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/literal.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/support/numeric_utils/extract_int.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <cstdint>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    template <
        typename T
      , unsigned Radix = 10
      , unsigned MinDigits = 1
      , int MaxDigits = -1>
    struct uint_parser : parser<uint_parser<T, Radix, MinDigits, MaxDigits>>
    {
        // check template parameter 'Radix' for validity
        static_assert(
            (Radix >= 2 && Radix <= 36),
            "Error Unsupported Radix");

        typedef T attribute_type;
        static bool const has_attribute = true;
        static bool const caller_is_pass_through_unary = true;
        typedef extract_uint<T, Radix, MinDigits, MaxDigits> extract;

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
        struct literal<T, typename enable_if<is_unsigned<T>>::type>
        {
            typedef uint_parser<T> type;
        };
    }
    
#define BOOST_SPIRIT_X3_UINT_PARSER(uint_type, name)                            \
    typedef uint_parser<uint_type> name##type;                                  \
    name##type const name{};                                                    \
    /***/

    BOOST_SPIRIT_X3_UINT_PARSER(unsigned long, ulong_)
    BOOST_SPIRIT_X3_UINT_PARSER(unsigned short, ushort_)
    BOOST_SPIRIT_X3_UINT_PARSER(unsigned int, uint_)
    BOOST_SPIRIT_X3_UINT_PARSER(unsigned long long, ulong_long)

    BOOST_SPIRIT_X3_UINT_PARSER(uint8_t, uint8)
    BOOST_SPIRIT_X3_UINT_PARSER(uint16_t, uint16)
    BOOST_SPIRIT_X3_UINT_PARSER(uint32_t, uint32)
    BOOST_SPIRIT_X3_UINT_PARSER(uint64_t, uint64)

#undef BOOST_SPIRIT_X3_UINT_PARSER

#define BOOST_SPIRIT_X3_UINT_PARSER(uint_type, radix, name)                     \
    typedef uint_parser<uint_type, radix> name##type;                           \
    name##type const name{};                                                    \
    /***/

    BOOST_SPIRIT_X3_UINT_PARSER(unsigned, 2, bin)
    BOOST_SPIRIT_X3_UINT_PARSER(unsigned, 8, oct)
    BOOST_SPIRIT_X3_UINT_PARSER(unsigned, 16, hex)

#undef BOOST_SPIRIT_X3_UINT_PARSER


}}}

#endif
