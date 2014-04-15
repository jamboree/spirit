/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_DIRECTIVE_HPP_INCLUDED
#define BOOST_SPIRIT_X3_DIRECTIVE_HPP_INCLUDED


#if defined(_MSC_VER)
#pragma once
#endif


#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/eval.hpp>
#include <boost/spirit/home/x3/support/utility/integer_sequence.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Directive, typename Subject>
    struct directive_parser : unary_parser<Subject, directive_parser<Directive, Subject>>
    {
        typedef unary_parser<Subject, directive_parser<Directive, Subject>> base_type;
        static bool const is_pass_through_unary = true;
        static bool const handles_container = Subject::handles_container;

        directive_parser(Directive const& directive, Subject const& subject)
          : base_type(subject), directive(directive) {}
          
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return directive.parse(this->subject, first, last, context, attr);
        }
        
        Directive directive;
    };
    
    template <typename Directive, typename... Ts>
    struct directive_caller
    {
        directive_caller(Directive const& directive, Ts&& ...ts)
          : directive(directive), params(std::forward<Ts>(ts)...) {}

        template <typename Subject>
        directive_parser<directive_caller, typename extension::as_parser<Subject>::value_type>
        operator[](Subject const& subject) const
        {
            return {*this, subject};
        }
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            make_index_sequence<sizeof...(Ts)> indices;
            return invoke_parse(indices, subject, first, last, context, attr);
        }
        
        template <std::size_t... Ns, typename Subject
            , typename Iterator, typename Context, typename Attribute>
        bool invoke_parse(index_sequence<Ns...>, Subject const& subject
          , Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return directive.parse(subject, first, last, context, attr,
                eval(std::get<Ns>(params), context)...);
        }

        Directive directive;
        std::tuple<Ts...> params;
    };

    template <typename Derived>
    struct directive
    {
        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }

        template <typename Subject>
        directive_parser<Derived, typename extension::as_parser<Subject>::value_type>
        operator[](Subject const& subject) const
        {
            return {derived(), subject};
        }
        
        template <typename... Ts>
        directive_caller<Derived, Ts...>
        operator()(Ts&& ...ts) const
        {
            return {derived(), std::forward<Ts>(ts)...};
        }
    };
}}}


#endif

