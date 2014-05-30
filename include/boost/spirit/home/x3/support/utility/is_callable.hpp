/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_IS_CALLABLE_HPP_INCLUDED
#define BOOST_SPIRIT_X3_IS_CALLABLE_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/utility/result_of.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/spirit/home/x3/support/utility/sfinae.hpp>

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Sig, typename Enable = void>
    struct is_callable_impl
      : false_type
    {};

    template <typename F, typename... A>
    struct is_callable_impl<F(A...), typename disable_if_substitution_failure<
        typename result_of<F(A...)>::type>::type>
      : true_type
    {};
}}}}

namespace boost { namespace spirit { namespace x3
{
    template <typename Sig>
    struct is_callable;

    template <typename F, typename... A>
    struct is_callable<F(A...)>
      : detail::is_callable_impl<F(A...)>
    {};
}}}

#endif
