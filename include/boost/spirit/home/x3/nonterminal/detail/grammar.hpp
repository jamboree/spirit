/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_DETAIL_GRAMMAR_HPP_INCLUDED
#define BOOST_SPIRIT_X3_DETAIL_GRAMMAR_HPP_INCLUDED


#include <boost/spirit/home/x3/nonterminal/rule.hpp>


namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename First, typename... Rest>
    struct rule_map : First, Rest...
    {
        rule_map(First const& f, Rest const&... rs)
          : First(f), Rest(rs)...
        {}
        
        template <typename ID, typename RHS, typename Attribute, typename Params, bool P>
        static rule_definition<ID, RHS, Attribute, Params, P> const&
        get_impl(rule_definition<ID, RHS, Attribute, Params, P> const& def)
        {
            return def;
        }
        
        template <typename ID, typename RHS, typename Attribute, typename Params, bool P>
        static mpl::true_ has_impl(rule_definition<ID, RHS, Attribute, Params, P> const&);
        
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
        
        First const& front() const
        {
            return *this;
        }
    };
}}}}


#endif

