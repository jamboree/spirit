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
        template <typename Encoding>
        struct test
        {
            typedef typename Encoding::char_type char_type;
            typedef std::pair<char_type, char_type> range;
            
            template <typename Char, typename Context>
            static bool check(Char ch, Context const&)
            {
                return ((sizeof(Char) <= sizeof(char_type)) || Encoding::ischar(ch));
            }
            
            template <typename Char, typename Context, typename Char_>
            static bool check(Char ch, Context const& ctx, Char_ ch_)
            {
                return check(ch, ctx) && ch == char_type(ch_);
            }
            
            template <typename Char>
            static range transform_params(Char a, Char b)
            {
                return {char_type(a), char_type(b)};
            }
            
            template <typename Char, typename Context>
            static bool check(Char ch, Context const& ctx, range const& r)
            {
                return check(ch, ctx) && !(ch < r.first) && !(r.second < ch);
            }
        };
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
        struct literal<char>
        {
            typedef standard::char_type type;
        };
        
        template <>
        struct literal<wchar_t>
        {
            typedef standard_wide::char_type type;
        };
    }
}}}

#endif
