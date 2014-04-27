/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_GRAMMAR_JAN_19_2012_0454PM)
#define BOOST_SPIRIT_X3_GRAMMAR_JAN_19_2012_0454PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/nonterminal/grammar.hpp>
#include <boost/fusion/support/pair.hpp>

#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/config.hpp>


namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename First, typename... Rest>
    struct rule_map : First, Rest...
    {
        typedef typename First::second_type start_rule;

        rule_map(First&& f, Rest&&... rs)
          : First(std::move(f)), Rest(std::move(rs))...
        {}
        
        template <typename ID, typename T>
        static T const& get_impl(fusion::pair<ID, T> const& pair)
        {
            return pair.second;
        }
        
        template <typename ID, typename T>
        static mpl::true_ has_impl(fusion::pair<ID, T> const&);
        
        template <typename ID>
        static mpl::false_ has_impl(...);
        
        template <typename ID>
        auto get(mpl::identity<ID>) const->decltype(get_impl<ID>(*this))
        {
            return get_impl<ID>(*this);
        }
        
        template <typename ID>
        auto has(mpl::identity<ID>) const->decltype(has_impl<ID>(*this))
        {
            return decltype(has_impl<ID>(*this))();
        }
        
        start_rule const& start() const
        {
            return First::second;
        }
    };
}}}}

namespace boost { namespace spirit { namespace x3
{
    template <typename Elements, typename Next>
    struct grammar_context
    {
        Elements const& elements;
        Next const& next;
        
        grammar_context(Elements const& elements, Next const& next)
            : elements(elements), next(next) {}

        template <typename ID>
        struct get_result
        {
            typedef decltype(declval<grammar_context>().get(declval<ID>())) type;
        };

        template <typename ID>
        auto get_impl(ID id, mpl::true_) const->decltype(elements.get(id))
        {
            return elements.get(id);
        }

        template <typename ID>
        auto get_impl(ID id, mpl::false_) const->decltype(next.get(id))
        {
            return next.get(id);
        }

        template <typename ID>
        auto get(ID id) const->decltype(this->get_impl(id, elements.has(id)))
        {
            return get_impl(id, elements.has(id));
        }
    };

    template <typename... Elements>
    struct grammar_parser : parser<grammar_parser<Elements...>>
    {
        typedef
            detail::rule_map<fusion::pair<typename Elements::id, Elements>...>
        rule_map;
        typedef typename rule_map::start_rule start_rule;
        typedef typename start_rule::attribute_type attribute_type;
        static bool const has_attribute = start_rule::has_attribute;
        static bool const handles_container = start_rule::handles_container;
        static bool const caller_is_pass_through_unary = true;

        grammar_parser(char const* name, Elements const&... elements)
          : name(name)
          , elements(fusion::pair<typename Elements::id, Elements>(elements)...)
        {}

        template <typename Iterator, typename Context, typename Attribute, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            grammar_context<rule_map, Context> our_context(elements, context);
            return elements.start().parse(first, last, our_context, attr
                , std::forward<Ts>(ts)...);
        }

        char const* name;
        rule_map elements;
    };

    template <typename... Elements>
    grammar_parser<Elements...>
    grammar(char const* name, Elements const&... elements)
    {
        return {name, elements...};
    }

    template <typename...  Elements>
    struct get_info<grammar_parser<Elements...>>
    {
        typedef std::string result_type;
        std::string operator()(grammar_parser<Elements...> const& p) const
        {
            return p.name;
        }
    };
}}}

#endif
