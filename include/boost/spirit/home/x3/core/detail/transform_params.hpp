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
#include <boost/mpl/int.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/spirit/home/x3/support/utility/sfinae.hpp>
#include <boost/spirit/home/x3/support/utility/integer_sequence.hpp>


namespace boost { namespace spirit { namespace x3 { namespace detail
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(transform_params)

    template <typename Subject, typename Tuple, typename = void>
    struct transform_params
    {
        typedef Tuple type;
        typedef
            mpl::int_<has_transform_params<Subject>::value && std::tuple_size<Tuple>::value>
        invoke_tag;
    };
    
    template <typename Subject, typename... Ts>
    struct transform_params<Subject, std::tuple<Ts...>,
        typename disable_if_substitution_failure<decltype(Subject::transform_params::apply(declval<Ts&&>...))>::type>
    {
        struct type
        {
            typedef typename Subject::transform_params transform;
            
            type(Ts&& ...ts)
              : data(transform::apply(std::forward<Ts>(ts)...))
            {}

            decltype(transform::apply(declval<Ts&&>...)) data;
        };

        typedef mpl::int_<2> invoke_tag;
    };
}}}}


#endif

