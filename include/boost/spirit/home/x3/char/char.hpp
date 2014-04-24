/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CHAR_APRIL_16_2006_1051AM)
#define BOOST_SPIRIT_X3_CHAR_APRIL_16_2006_1051AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/literal.hpp>
#include <boost/spirit/home/x3/char/char_parser.hpp>
#include <boost/spirit/home/support/char_encoding/ascii.hpp>
#include <boost/spirit/home/support/char_encoding/standard.hpp>
#include <boost/spirit/home/support/char_encoding/standard_wide.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct any_char
    {
        template <typename Encoding, typename Char, typename Context>
        static bool test(Encoding, Char ch, Context const&)
        {
            typedef typename Encoding::char_type char_type;
            return ((sizeof(Char) <= sizeof(char_type)) || Encoding::ischar(ch));
        }
        
        template <typename Encoding, typename Char, typename Context, typename Char_>
        static bool test(Encoding e, Char ch, Context const& ctx, Char_ ch_)
        {
            typedef typename Encoding::char_type char_type;
            return test(e, ch, ctx) && ch == char_type(ch_);
        }
    };
    
    namespace standard
    {
        typedef char_parser<char_encoding::standard, any_char> char_type;
        char_type const char_{};
    }

    using standard::char_type;
    using standard::char_;

    namespace standard_wide
    {
        typedef char_parser<char_encoding::standard_wide, any_char> char_type;
        char_type const char_{};
    }

    namespace ascii
    {
        typedef char_parser<char_encoding::ascii, any_char> char_type;
        char_type const char_{};
    }

    namespace extension
    {
        template <>
        struct literal_delegate<char>
        {
            typedef standard::char_type type;
        };
        
        template <>
        struct literal_delegate<wchar_t>
        {
            typedef standard_wide::char_type type;
        };
    }
}}}

#endif
