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
    template <typename Encoding, typename Class>
    struct char_parser : parser<char_parser<Encoding, Class>>
    {
        typedef typename Class::template test<Encoding> test;
        typedef typename Encoding::char_type attribute_type;
        static bool const has_attribute = true;
        static bool const caller_is_pass_through_unary = true;
        
        template <typename Test = test, typename... Ts>
        static auto transform_params(Ts&&... ts)->
            decltype(Test::transform_params(std::forward<Ts>(ts)...))
        {
            return Test::transform_params(std::forward<Ts>(ts)...);
        }
        
        template <typename Iterator, typename Context, typename Attribute, typename... Ts>
        bool parse(
            Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            x3::skip_over(first, last, context);

            if (first != last && test::check(*first, context, std::forward<Ts>(ts)...))
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
            template <typename Encoding>
            struct test : Positive::template test<Encoding>
            {
                typedef typename Positive::template test<Encoding> pos;
                
                template <typename Char, typename Context, typename... Ts>
                static bool check(Char ch, Context const& ctx, Ts&&... ts)
                {
                    return !pos::check(ch, ctx, std::forward<Ts>(ts)...);
                }
            };
        };
    };

    template <typename Positive>
    struct not_char<not_char<Positive>>
    {
        typedef Positive type;
    };
    
    template <typename Encoding, typename Class>
    inline char_parser<Encoding, typename not_char<Class>::type>
    operator~(char_parser<Encoding, Class> const&)
    {
        return {};
    }
    
    template <typename Encoding, typename Class, typename... Ts>
    inline caller<char_parser<Encoding, typename not_char<Class>::type>, Ts...> 
    operator~(caller<char_parser<Encoding, Class>, Ts...> const& c)
    {
        return {mpl::false_(), {}, c.params};
    }
}}}

#endif
