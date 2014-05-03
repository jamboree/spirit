/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013-2014 Agustin Berge
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_AUXILIARY_ANY_PARSER_APR_09_2014_1145PM)
#define BOOST_SPIRIT_X3_AUXILIARY_ANY_PARSER_APR_09_2014_1145PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/nonterminal/rule.hpp>
#include <boost/spirit/home/x3/auxiliary/reference.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <boost/spirit/home/x3/support/traits/is_parser.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Iterator, typename Attribute = unused_type>
    struct any_parser : parser<any_parser<Iterator, Attribute>>
    {
        typedef detail::decompose_attribute<Attribute> decompose_attr;
        typedef typename decompose_attr::result_type attribute_type;
        typedef typename decompose_attr::params_type params_type;
        typedef x3::rule_context<attribute_type, params_type> rule_context;
        typedef x3::context<rule_context_tag, rule_context> context_type;
        typedef x3::reference<any_parser const> reference;
        static bool const has_attribute =
            !is_same<attribute_type, unused_type>::value;
        static bool const handles_container =
            traits::is_container<attribute_type>::value;
        static bool const caller_is_pass_through_unary = true;

        template <typename Expr
            , typename enable_if<traits::is_parser<Expr>, bool>::type = true>
        any_parser(Expr const& expr)
          : f([p = as_parser(expr)](Iterator& first, Iterator const& last,
                context_type const& context, attribute_type& attr)
            {
                return p.parse(first, last, context, attr);
            })
        {}

        template <typename Context, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, attribute_type& attr, Ts&&... ts) const
        {
            static_assert(
                detail::check_args<params_type, Ts...>::value
              , "args/params not matched");
            
            params_type params(std::forward<Ts>(ts)...);
            rule_context r_context{boost::addressof(attr), &params};
            x3::skip_over(first, last, context);
            return f(first, last, context_type(r_context), attr);
        }

        template <typename Context, typename Attribute_, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr_, Ts&&... ts) const
        {
            attribute_type attr;
            if (parse(first, last, context, attr, std::forward<Ts>(ts)...))
            {
                traits::move_to(attr, attr_);
                return true;
            }
            return false;
        }

    private:
        
        std::function<bool(Iterator&, Iterator const&, context_type const&,
            attribute_type&)> f;
    };
}}}

#endif
