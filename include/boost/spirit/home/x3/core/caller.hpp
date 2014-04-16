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

#include <boost/spirit/home/x3/core/detail/eval.hpp>
#include <boost/spirit/home/x3/core/detail/transform_params.hpp>
#include <boost/spirit/home/x3/support/context.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Subject, typename... Ts>
    struct caller : unary_parser<Subject, caller<Subject, Ts...>>
    {
        typedef unary_parser<Subject, caller<Subject, Ts...>> base_type;
        typedef detail::transform_params<Subject, std::tuple<Ts...>> transform;
        static bool const is_pass_through_unary = true;

        caller(Subject const& subject, Ts&& ...ts)
          : base_type(subject), params(std::forward<Ts>(ts)...) {}
          
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            typename transform::invoke_tag tag;
            make_index_sequence<sizeof...(Ts)> indices;
            return invoke_parse(tag, indices, first, last, context, attr);
        }
        
        // no transform_params
        template <std::size_t... Ns
            , typename Iterator, typename Context, typename Attribute>
        bool invoke_parse(mpl::int_<0>, index_sequence<Ns...>
          , Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return this->subject.parse(first, last, context, attr,
                detail::eval(std::get<Ns>(params), context)...);
        }

        // post-transform
        template <std::size_t... Ns
            , typename Iterator, typename Context, typename Attribute>
        bool invoke_parse(mpl::int_<1>, index_sequence<Ns...>
          , Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return this->subject.parse(first, last, context, attr,
                Subject::transform_params::apply(
                    detail::eval(std::get<Ns>(params), context)...));
        }

        // transformed
        template <std::size_t... Ns
            , typename Iterator, typename Context, typename Attribute>
        bool invoke_parse(mpl::int_<2>, index_sequence<Ns...>
          , Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return this->subject.parse(first, last, context, attr, params.data);
        }
        
        typename transform::type params;
    };
}}}


#endif

