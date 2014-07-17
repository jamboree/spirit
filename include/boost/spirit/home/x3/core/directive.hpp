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
#include <boost/spirit/home/x3/core/eval.hpp>
#include <boost/spirit/home/x3/core/detail/pack_params.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/handles_container.hpp>

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Directive, typename = void>
    struct caller_is_pass_through_unary
      : mpl::bool_<Directive::is_pass_through_unary> {};
      
    template <typename Directive>
    struct caller_is_pass_through_unary<Directive,
        typename disable_if_substitution_failure<
            mpl::bool_<Directive::caller_is_pass_through_unary>>::type>
      : mpl::bool_<Directive::caller_is_pass_through_unary> {};
}}}}

namespace boost { namespace spirit { namespace x3
{
    template <typename Directive, typename Subject>
    struct directive_parser
      : unary_parser<Subject, directive_parser<Directive, Subject>>
    {
        typedef
            unary_parser<Subject, directive_parser<Directive, Subject>>
        base_type;
        static bool const is_pass_through_unary =
            Directive::is_pass_through_unary;
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
        typedef detail::pack_params<Directive, void, Ts...> pack_params;
        typedef typename pack_params::type pack_type;
        
        static bool const is_pass_through_unary =
            detail::caller_is_pass_through_unary<Directive>::value;
        
        template <typename... As>
        directive_caller(Directive const& directive, As&&... as)
          : directive(directive)
          , pack(pack_params::pack(directive, std::forward<As>(as)...))
        {}

        directive_caller(Directive const& directive, pack_type const& pack)
          : directive(directive), pack(pack) {}

        template <typename Subject>
        directive_parser<directive_caller
            , typename extension::as_parser<Subject>::value_type>
        operator[](Subject const& subject) const
        {
            return {*this, as_parser(subject)};
        }
        
        template <typename Subject, typename Iterator
            , typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return pack(directive, context, [&](auto&&... as)
            {
                return this->directive.parse(subject, first, last, context,
                    attr, static_cast<decltype(as)>(as)...);
            });
        }

        Directive directive;
        pack_type pack;
    };

    template <typename Derived>
    struct directive
    {
        static bool const is_pass_through_unary = false;

        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }

        template <typename Subject>
        directive_parser<Derived
            , typename extension::as_parser<Subject>::value_type>
        operator[](Subject const& subject) const
        {
            return {derived(), as_parser(subject)};
        }
        
        template <typename... Ts>
        directive_caller<Derived, unrefcv_t<Ts>...>
        operator()(Ts&&... ts) const
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
      : mpl::identity<typename Directive::
            template traits<Subject>::attribute_type>
    {};
    
    template <typename Directive, typename Subject, typename Context>
    struct attribute_of<directive_parser<Directive, Subject>, Context,
        typename disable_if_substitution_failure<
            typename Directive::template traits<Subject>::
                template attribute<Context>::type>::type>
      : Directive::template traits<Subject>::template attribute<Context>
    {};
    
    // has_attribute
    template <typename Directive, typename Subject, typename Context>
    struct has_attribute<directive_parser<Directive, Subject>, Context,
        typename disable_if_substitution_failure<mpl::bool_<Directive::
            template traits<Subject>::has_attribute>>::type>
      : mpl::bool_<Directive::template traits<Subject>::has_attribute> {};
      
    // handles_container
    template <typename Directive, typename Subject, typename Context>
    struct handles_container<directive_parser<Directive, Subject>, Context,
        typename disable_if_substitution_failure<mpl::bool_<Directive::
            template traits<Subject>::handles_container>>::type>
      : mpl::bool_<Directive::template traits<Subject>::handles_container> {};
    
    // Caller
    //--------------------------------------------------------------------------
    namespace detail
    {
        template <typename Sig>
        struct caller_type;
        
        template <typename R, typename... Ts>
        struct caller_type<R(Ts...)>
        {
            typedef R type;
        };
        
        // attribute_of
        template <typename DirectiveCall, typename Subject, typename Context, typename Enable = void>
        struct directive_caller_attribute_of
          : attribute_of<directive_parser<typename
                caller_type<DirectiveCall>::type, Subject>, Context> {};

        template <typename Directive, typename... Ts, typename Subject, typename Context>
        struct directive_caller_attribute_of<Directive(Ts...)
            , Subject, Context, typename disable_if_substitution_failure<
                typename Directive::template caller_traits<Subject, Ts...>::
                    attribute_type>::type>
          : mpl::identity<typename Directive::template caller_traits<Subject
                , Ts...>::attribute_type> {};

        template <typename Directive, typename... Ts, typename Subject, typename Context>
        struct directive_caller_attribute_of<Directive(Ts...)
            , Subject, Context, typename disable_if_substitution_failure<
                typename Directive::template caller_traits<Subject, Ts...>::
                    template attribute<Context>::type>::type>
          : Directive::template caller_traits<Subject, Ts...>::
                template attribute<Context> {};

        // has_attribute
        template <typename DirectiveCall, typename Subject, typename Context, typename Enable = void>
        struct directive_caller_has_attribute
          : has_attribute<directive_parser<typename
                caller_type<DirectiveCall>::type, Subject>, Context> {};

        template <typename Directive, typename... Ts, typename Subject, typename Context>
        struct directive_caller_has_attribute<Directive(Ts...)
            , Subject, Context, typename disable_if_substitution_failure<
                mpl::bool_<Directive::template caller_traits<Subject, Ts...>::
                    has_attribute>>::type>
          : mpl::bool_<Directive::template caller_traits<Ts...>::
                has_attribute> {};

        // handles_container
        template <typename DirectiveCall, typename Subject, typename Context, typename Enable = void>
        struct directive_caller_handles_container
          : handles_container<directive_parser<typename
                caller_type<DirectiveCall>::type, Subject>, Context> {};

        template <typename Directive, typename... Ts, typename Subject, typename Context>
        struct directive_caller_handles_container<Directive(Ts...)
            , Subject, Context, typename disable_if_substitution_failure<
                mpl::bool_<Directive::template caller_traits<Subject, Ts...>::
                    handles_container>>::type>
          : mpl::bool_<Directive::template caller_traits<Subject, Ts...>::
                handles_container> {};
    }

    template <typename Directive, typename... Ts, typename Subject, typename Context>
    struct attribute_of<directive_parser<directive_caller<Directive, Ts...>
        , Subject>, Context>
      : detail::directive_caller_attribute_of<
            Directive(eval_t<Ts, Context>...)
          , Subject, Context> {};

    template <typename Directive, typename... Ts, typename Subject, typename Context>
    struct has_attribute<directive_parser<directive_caller<Directive, Ts...>
        , Subject>, Context>
      : detail::directive_caller_has_attribute<
            Directive(eval_t<Ts, Context>...)
          , Subject, Context> {};

    template <typename Directive, typename... Ts, typename Subject, typename Context>
    struct handles_container<directive_parser<directive_caller<Directive, Ts...>
        , Subject>, Context>
      : detail::directive_caller_handles_container<
            Directive(eval_t<Ts, Context>...)
          , Subject, Context> {};
}}}}

#endif
