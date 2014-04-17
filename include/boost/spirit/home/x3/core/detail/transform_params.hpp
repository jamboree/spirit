/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_DETAIL_TRANSFORM_PARAMS_HPP_INCLUDED
#define BOOST_SPIRIT_X3_DETAIL_TRANSFORM_PARAMS_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <tuple>
#include <boost/mpl/bool.hpp>
#include <boost/spirit/home/x3/support/utility/sfinae.hpp>


namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Subject, typename Tuple, typename = void>
    struct transform_params
    {
        typedef Tuple type;
        typedef mpl::false_ is_transformed;
        typedef bool no;
    };
    
    template <typename Subject, typename... Ts>
    struct transform_params<Subject, std::tuple<Ts...>,
        typename disable_if_substitution_failure<decltype(Subject::transform_params(declval<Ts&&>...))>::type>
    {
        struct type
        {
            type(Ts&& ...ts)
              : data(Subject::transform_params(std::forward<Ts>(ts)...))
            {}

            decltype(Subject::transform_params(declval<Ts&&>...)) data;
        };

        typedef mpl::true_ is_transformed;
        typedef bool yes;
    };
}}}}


#endif

