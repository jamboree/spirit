/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_GRAMMAR_JAN_19_2012_0454PM)
#define BOOST_SPIRIT_X3_GRAMMAR_JAN_19_2012_0454PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/nonterminal/detail/grammar.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/has_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/handles_container.hpp>

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
        typedef detail::rule_map<Elements...> rule_map;
        static bool const caller_is_pass_through_unary = true;

        grammar_parser(char const* name, Elements const&... elements)
          : name(name)
          , elements(elements...)
        {}

        template <typename Iterator, typename Context, typename Attribute, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            grammar_context<rule_map, Context> our_context(elements, context);
            return elements.front().parse(first, last, our_context, attr
                , std::forward<Ts>(ts)...);
        }

        char const* name;
        rule_map elements;
    };

    template <typename... Elements>
    inline grammar_parser<Elements...>
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

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename First, typename... Rest, typename Context>
    struct attribute_of<grammar_parser<First, Rest...>, Context>
      : attribute_of<First, Context>
    {};
    
    template <typename First, typename... Rest, typename Context>
    struct has_attribute<grammar_parser<First, Rest...>, Context>
      : has_attribute<First, Context>
    {};
    
    template <typename First, typename... Rest, typename Context>
    struct handles_container<grammar_parser<First, Rest...>, Context>
      : handles_container<First, Context>
    {};
}}}}

#endif
