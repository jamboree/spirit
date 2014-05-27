/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_TOKEN_HPP_INCLUDED
#define BOOST_SPIRIT_X3_TOKEN_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <tuple>
#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/metafunctions.hpp>


namespace boost { namespace spirit { namespace x3
{
    struct token_tag;
    
    struct tokval
    {
        template <typename Context>
        auto operator()(Context const& context) const
        {
            return x3::get<token_tag>(context).first;
        }
    };
    
    struct tokstr
    {
        template <typename Context>
        auto operator()(Context const& context) const
        {
            auto const& range = x3::get<token_tag>(context).second;
            typedef typename
                range_value<unrefcv_t<decltype(range)>>::type
            value_type;
            return std::basic_string<value_type>(range.begin(), range.end());
        }
    };

    struct token_directive : directive<token_directive>
    {
        static bool const is_pass_through_unary = true;
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename Lexer>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Lexer const& lexer) const
        {
            static_assert(traits::is_parser<Lexer>::value, "invalid lexer");

            x3::skip_over(first, last, context);
            Iterator it(first);
            typename traits::attribute_of<Lexer, Context>::type val{};
            if (lexer.parse(it, last, context, val))
            {
                auto token(std::make_pair(val, make_iterator_range(first, it)));
                auto token_context(make_context<token_tag>(token, context));
                if (subject.parse(it, last, token_context, attr))
                {
                    first = it;
                    return true;
                }
            }
            return false;
        }
    };
    
    token_directive const token{};
    tokval const _tokval{};
    tokstr const _tokstr{};
}}}


#endif

