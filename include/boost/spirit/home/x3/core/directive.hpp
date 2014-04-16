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


#include <tuple>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/detail/eval.hpp>
#include <boost/spirit/home/x3/support/utility/integer_sequence.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/handles_container.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Directive, typename Subject>
    struct directive_parser : unary_parser<Subject, directive_parser<Directive, Subject>>
    {
        typedef unary_parser<Subject, directive_parser<Directive, Subject>> base_type;
        static bool const is_pass_through_unary = Directive::is_pass_through_unary;

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
        template <typename Subject>
        struct traits : Directive::template traits<Subject> {};
        
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
                detail::eval(std::get<Ns>(params), context)...);
        }

        Directive directive;
        std::tuple<Ts...> params;
    };

    template <typename Derived>
    struct directive
    {
        static bool const is_pass_through_unary = false;
        
        template <typename Subject>
        struct traits {};

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

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    // attribute_of
    template <typename Directive, typename Subject, typename Context>
    struct attribute_of<directive_parser<Directive, Subject>, Context,
        typename disable_if_substitution_failure<
            typename Directive::template traits<Subject>::attribute_type>::type>
      : mpl::identity<typename Directive::template traits<Subject>::attribute_type>
    {};
    
    template <typename Directive, typename Subject, typename Context>
    struct attribute_of<directive_parser<Directive, Subject>, Context,
        typename disable_if_substitution_failure<
            typename Directive::template traits<Subject>::template attribute<Context>::type>::type>
      : mpl::identity<typename Directive::template traits<Subject>::template attribute<Context>::type>
    {};
    
    // has_attribute
    template <typename Directive, typename Subject, typename Context>
    struct has_attribute<directive_parser<Directive, Subject>, Context,
        typename disable_if_substitution_failure<
            mpl::bool_<Directive::template traits<Subject>::has_attribute>>::type>
      : mpl::bool_<Directive::template traits<Subject>::has_attribute> {};
      
    // handles_container
    template <typename Directive, typename Subject, typename Context>
    struct handles_container<directive_parser<Directive, Subject>, Context,
        typename disable_if_substitution_failure<
            mpl::bool_<Directive::template traits<Subject>::handles_container>>::type>
      : mpl::bool_<Directive::template traits<Subject>::handles_container> {};
}}}}


#endif

