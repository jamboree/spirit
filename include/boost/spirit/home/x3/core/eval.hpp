/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_EVAL_HPP_INCLUDED
#define BOOST_SPIRIT_X3_EVAL_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/traits/is_parser.hpp>
#include <boost/spirit/home/x3/support/utility/unrefcv.hpp>
#include <boost/spirit/home/x3/support/utility/is_callable.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename T, typename Context, typename = void>
    struct result_of_eval
    {
        typedef T type;
        
        static T&& apply(T&& val, Context const&)
        {
            return std::forward<T>(val);
        }
    };

    template <typename F, typename Context>
    struct result_of_eval<F, Context,
        typename enable_if_c<!traits::is_parser<unrefcv_t<F>>::value
            && is_callable<F(Context const&)>::value, void>::type>
    {
        typedef decltype(declval<F>()(declval<Context const&>())) type;
        
        static type apply(F&& f, Context const& ctx)
        {
            return f(ctx);
        }
    };
    
    template <typename T, typename Context>
    inline typename result_of_eval<T, Context>::type
    eval(T&& val, Context const& ctx)
    {
        return result_of_eval<T, Context>::apply(std::forward<T>(val), ctx);
    }

    template <typename T, typename Context>
    using eval_t = typename result_of_eval<T, Context>::type;
}}}

#endif
