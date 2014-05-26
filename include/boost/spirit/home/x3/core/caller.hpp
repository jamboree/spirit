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

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/detail/eval.hpp>
#include <boost/spirit/home/x3/core/detail/transform_params.hpp>
#include <boost/spirit/home/x3/support/expectation.hpp>
#include <boost/spirit/home/x3/support/utility/integer_sequence.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/handles_container.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Subject, typename... Ts>
    struct caller : unary_parser<Subject, caller<Subject, Ts...>>
    {
        typedef unary_parser<Subject, caller<Subject, Ts...>> base_type;
        typedef detail::transform_params<Subject, void, Ts...> transform;
        typedef typename transform::tag transform_tag;
        static bool const is_pass_through_unary =
            Subject::caller_is_pass_through_unary;
        static bool const handles_container = Subject::handles_container;

        template <typename... As>
        caller(Subject const& subject, As&&... as)
          : caller(transform_tag(), subject, std::forward<As>(as)...) {}
          
        template <typename... As>
        caller(mpl::true_, Subject const& subject, As&&... as)
          : base_type(subject)
          , params(subject.transform_params(std::forward<As>(as)...))
        {}
        
        template <typename... As>
        caller(mpl::false_, Subject const& subject, As&&... as)
          : base_type(subject), params(std::forward<As>(as)...) {}
          
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            transform_tag tag;
            make_index_sequence<sizeof...(Ts)> indices;
            return parse_impl(tag, indices, first, last, context, attr);
        }

        // transformed
        template <std::size_t... Ns
          , typename Iterator, typename Context, typename Attribute>
        bool parse_impl(mpl::true_, index_sequence<Ns...>
          , Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return invoke_parse(first, last, context, attr, params);
        }
        
        // not transformed
        template <std::size_t... Ns
          , typename Iterator, typename Context, typename Attribute>
        bool parse_impl(mpl::false_, index_sequence<Ns...>
          , Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return parse_unpacked(first, last, context, attr,
                detail::eval<typename detail::unwrap_param<Ts>::type>(
                    std::get<Ns>(params), context)...);
        }
        
        template <typename Iterator, typename Context
          , typename Attribute, typename... As>
        typename enable_if_c<detail::transform_params<Subject, void, As...>
            ::tag::value, bool>::type
        parse_unpacked(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, As&&... as) const
        {
            return invoke_parse(first, last, context, attr,
                this->subject.transform_params(std::forward<As>(as)...));
        }
        
        template <typename Iterator, typename Context
          , typename Attribute, typename... As>
        typename enable_if_c<!detail::transform_params<Subject, void, As...>
            ::tag::value, bool>::type
        parse_unpacked(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, As&&... as) const
        {
            return invoke_parse(
                first, last, context, attr, std::forward<As>(as)...);
        }

        template <
            typename HandlesExpectation = traits::handles_expectation<Subject>
          , typename Iterator, typename Unused, typename Next
          , typename Attribute, typename... As>
        typename enable_if_c<!HandlesExpectation::value, bool>::type
        invoke_parse(Iterator& first, Iterator const& last
          , x3::context<expectation_tag, Unused, Next> const& context
          , Attribute& attr, As&&... as) const
        {
            Iterator entry(first);
            if (!this->subject.parse(
                first, last, context.next, attr, std::forward<As>(as)...))
                boost::throw_exception(expectation_failure<Iterator>(
                    entry, what(this->subject, std::forward<As>(as)...)));
            return true;
        }

        template <typename Iterator, typename Context
            , typename Attribute, typename... As>
        bool invoke_parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, As&&... as) const
        {
            return this->subject.parse(
                first, last, context, attr, std::forward<As>(as)...);
        }
        
        typename transform::type params;
    };
}}}

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    // attribute_of
    template <typename Subject, typename... Ts, typename Context>
    struct attribute_of<caller<Subject, Ts...>, Context,
        typename disable_if_substitution_failure<
            typename Subject::template caller_traits<x3::detail::
                result_of_eval<Ts, Context>...>::attribute_type>::type>
      : mpl::identity<typename Subject::template caller_traits<x3::detail::
            result_of_eval<Ts, Context>...>::attribute_type> {};
    
    template <typename Subject, typename... Ts, typename Context>
    struct attribute_of<caller<Subject, Ts...>, Context,
        typename disable_if_substitution_failure<
            typename Subject::template caller_traits<
                x3::detail::result_of_eval<Ts, Context>...>::
                    template attribute<Context>::type>::type>
      : Subject::template caller_traits<x3::detail::
            result_of_eval<Ts, Context>...>::template attribute<Context> {};
    
    // has_attribute
    template <typename Subject, typename... Ts, typename Context>
    struct has_attribute<caller<Subject, Ts...>, Context,
        typename disable_if_substitution_failure<
            mpl::bool_<Subject::template caller_traits<x3::detail::
                result_of_eval<Ts, Context>...>::has_attribute>>::type>
      : mpl::bool_<Subject::template caller_traits<x3::detail::
            result_of_eval<Ts, Context>...>::has_attribute> {};
      
    // handles_container
    template <typename Subject, typename... Ts, typename Context>
    struct handles_container<caller<Subject, Ts...>, Context,
        typename disable_if_substitution_failure<
            mpl::bool_<Subject::template caller_traits<x3::detail::
                result_of_eval<Ts, Context>...>::handles_container>>::type>
      : mpl::bool_<Subject::template caller_traits<x3::detail::
            result_of_eval<Ts, Context>...>::handles_container> {};
    
    // handles_expectation
    template <typename Subject, typename... Ts>
    struct handles_expectation<caller<Subject, Ts...>> : mpl::true_ {};
}}}}


#endif

