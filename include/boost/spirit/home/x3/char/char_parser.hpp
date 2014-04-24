/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CHAR_PARSER_APR_16_2006_0906AM)
#define BOOST_SPIRIT_X3_CHAR_PARSER_APR_16_2006_0906AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    // The base char_parser
    ///////////////////////////////////////////////////////////////////////////
    template <typename Encoding, typename Classifier>
    struct char_parser : parser<char_parser<Encoding, Classifier>>
    {
        typedef typename Encoding::char_type attribute_type;
        static bool const has_attribute = true;
        static bool const caller_is_pass_through_unary = true;
        
        template <typename Iterator, typename Context, typename Attribute, typename... T>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, T&&... t) const
        {
            x3::skip_over(first, last, context);

            if (first != last && Classifier::test(Encoding(), *first, context, std::forward<T>(t)...))
            {
                x3::traits::move_to(*first, attr);
                ++first;
                return true;
            }
            return false;
        }
    };
    
    template <typename Positive>
    struct not_char
    {
        struct type
        {
            template <typename Encoding, typename Char, typename Context, typename... T>
            static bool test(Encoding e, Char ch, Context const& ctx, T... t)
            {
                return !Positive::test(e, ch, ctx, t...);
            }
        };
    };

    template <typename Positive>
    struct not_char<not_char<Positive>>
    {
        typedef Positive type;
    };
    
    template <typename Encoding, typename Classifier>
    inline char_parser<Encoding, typename not_char<Classifier>::type>
    operator~(char_parser<Encoding, Classifier> const&)
    {
        return {};
    }
    
    template <typename Encoding, typename Classifier, typename T>
    inline caller<char_parser<Encoding, typename not_char<Classifier>::type>, T> 
    operator~(caller<char_parser<Encoding, Classifier>, T> const& c)
    {
        return {{}, c.params};
    }
}}}

#endif
