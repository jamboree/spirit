/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_CALLER_HPP_INCLUDED
#define BOOST_SPIRIT_X3_CALLER_HPP_INCLUDED


#if defined(_MSC_VER)
#pragma once
#endif


#include <tuple>
#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/eval.hpp>
#include <boost/spirit/home/x3/support/utility/integer_sequence.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Subject, typename... Ts>
    struct caller : unary_parser<Subject, caller<Subject, Ts...>>
    {
        typedef unary_parser<Subject, caller<Subject, Ts...>> base_type;
        static bool const is_pass_through_unary = true;

        caller(Subject const& subject, Ts&& ...ts)
          : base_type(subject), params(std::forward<Ts>(ts)...) {}
          
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            make_index_sequence<sizeof...(Ts)> indices;
            return invoke_parse(indices, first, last, context, attr);
        }
        
        template <std::size_t... Ns
            , typename Iterator, typename Context, typename Attribute>
        bool invoke_parse(index_sequence<Ns...>
          , Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return this->subject.parse(first, last, context, attr,
                eval(std::get<Ns>(params), context)...);
        }

        std::tuple<Ts...> params;
    };
}}}


#endif

