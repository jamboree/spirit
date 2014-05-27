/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2013 Agustín Bergé
    opyright (c) 2014 Jamboree
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_SUBCONTEXT_APR_15_2013_0840AM)
#define BOOST_SPIRIT_X3_SUBCONTEXT_APR_15_2013_0840AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/utility/declval.hpp>
#include <boost/fusion/support/pair.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename... Pairs>
    struct subcontext;
    
    template <typename... IDs, typename... Ts>
    struct subcontext<IDs(Ts)...> : fusion::pair<IDs, Ts>...
    {
        subcontext(Ts... ts)
          : fusion::pair<IDs, Ts>(std::forward<Ts>(ts))...
        {}
        
        template <typename ID>
        static unused_type get_impl(unused_type)
        {
            return unused;
        }
        
        template <typename ID, typename T>
        static T get_impl(fusion::pair<ID, T> const& p)
        {
            return p.second;
        }
        
        template <typename ID>
        struct get_result
        {
            typedef decltype(declval<subcontext>().get(declval<ID>())) type;
        };

        template <typename ID>
        auto get(mpl::identity<ID>) const->decltype(get_impl<ID>(*this))
        {
            return get_impl<ID>(*this);
        }
    };
    
    template <typename... IDs, typename... Ts>
    inline subcontext<IDs(Ts)...> make_subcontext(Ts&&... ts)
    {
        return {std::forward<Ts>(ts)...};
    }
}}}

#endif
