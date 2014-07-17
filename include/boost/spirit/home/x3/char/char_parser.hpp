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
    template <typename Base>
    struct char_parser : parser<char_parser<Base>>, Base
    {
        typedef typename Base::char_type attribute_type;
        static bool const has_attribute = true;
        static bool const caller_is_pass_through_unary = true;
        
        template <typename Iterator, typename Context, typename Attribute, typename... Ts>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            x3::skip_over(first, last, context);

            if (first != last && Base::test(*first, context, std::forward<Ts>(ts)...))
            {
                x3::traits::move_to(*first, attr);
                ++first;
                return true;
            }
            return false;
        }
    };
    
    template <typename Positive>
    struct negated_char : Positive
    {
        template <typename Char, typename Context, typename... Ts>
        static bool test(Char ch, Context const& ctx, Ts&&... ts)
        {
            return !Positive::test(ch, ctx, std::forward<Ts>(ts)...);
        }
    };
    
    template <typename Positive>
    struct negate_char
    {
        typedef negated_char<Positive> type;
    };

    template <typename Positive>
    struct negate_char<negated_char<Positive>>
    {
        typedef Positive type;
    };
    
    template <typename Base>
    inline char_parser<typename negate_char<Base>::type>
    operator~(char_parser<Base> const&)
    {
        return {};
    }
    
    template <typename Base, typename... Ts>
    inline caller<char_parser<typename negate_char<Base>::type>, Ts...>
    operator~(caller<char_parser<Base>, Ts...> const& c)
    {
        return {{}, c.params};
    }
}}}

#endif
