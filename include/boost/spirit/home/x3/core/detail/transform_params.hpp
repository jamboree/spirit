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
#include <boost/ref.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/declval.hpp>
#include <boost/spirit/home/x3/core/eval.hpp>
#include <boost/spirit/home/x3/support/utility/sfinae.hpp>
#include <boost/spirit/home/x3/support/utility/integer_sequence.hpp>

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename T, std::size_t N>
    struct array_wrapper
    {
        typedef T const(&const_reference)[N];
        
        array_wrapper(T const(&data)[N])
          : array_wrapper(data, make_index_sequence<N>())
        {}

        template<std::size_t... Ns>
        array_wrapper(T const(&data)[N], index_sequence<Ns...>)
          : data{data[Ns]...}
        {}
        
        operator const_reference() const
        {
            return data; 
        }

        T data[N];
    };
    
    template <typename T>
    struct wrap_param
    {
        typedef T type;
    };

    template <typename T, std::size_t N>
    struct wrap_param<T[N]>
    {
        typedef array_wrapper<T, N> type;
    };
    
    template <typename T>
    struct unwrap_param
    {
        typedef T const& type;
    };

    template <typename T>
    struct unwrap_param<reference_wrapper<T>>
    {
        typedef T& type;
    };
    
    template <typename T>
    struct unwrap_param<std::reference_wrapper<T>>
    {
        typedef T& type;
    };
    
    template <typename T>
    using wrap_param_t = typename wrap_param<T>::type;
    
    template <typename T>
    using unwrap_param_t = typename unwrap_param<T>::type;
    
    template <typename Subject, typename Enable, typename... Ts>
    struct transform_params;

    template <typename Subject, typename Parse, typename... As>
    static typename transform_params<Subject, void, As...>::has_transform
    parse_unpacked(Subject const& subject, Parse const& parse, As&&... as)
    {
        return parse(subject.transform_params(std::forward<As>(as)...));
    }
    
    template <typename Subject, typename Parse, typename... As>
    static typename transform_params<Subject, void, As...>::no_transform
    parse_unpacked(Subject const& subject, Parse const& parse, As&&... as)
    {
        return parse(std::forward<As>(as)...);
    }

    template <typename... Ts>
    struct arg_pack
    {
        static auto pack(wrap_param_t<Ts>... ts)
        {
            return [=](auto const& subject, auto&& ctx, auto&& parse)
            {
                return parse_unpacked(subject, parse,
                    x3::eval<unwrap_param_t<Ts>>(ts, ctx)...);
            };
        }
        
        typedef decltype(pack(std::declval<Ts>()...)) type;
    };

    template <typename Subject, typename Enable, typename... Ts>
    struct transform_params
    {
        template <typename... As>
        static auto pack(Subject const& subject, As&&... as)
        {
            return arg_pack<Ts...>::pack(std::forward<As>(as)...);
        }
        
        typedef typename arg_pack<Ts...>::type type;
        typedef bool no_transform;
    };
    
    template <typename Params>
    struct transform_pack
    {
        template <typename Subject, typename Context, typename Parse>
        bool operator()(Subject const&, Context&&, Parse&& parse) const
        {
            return parse(params);
        }
        
        Params params;
    };
        
    template <typename Subject, typename... Ts>
    struct transform_params<Subject, typename disable_if_substitution_failure<
        decltype(declval<Subject const>().transform_params(declval<Ts>()...))>::type, Ts...>
    {
        typedef
            decltype(declval<Subject const>().transform_params(declval<Ts>()...))
        result_type;

        typedef transform_pack<result_type> type;
        
        template <typename... As>
        static type pack(Subject const& subject, As&&... as)
        {
            return {subject.transform_params(std::forward<As>(as)...)};
        }
        
        typedef bool has_transform;
        typedef type sfinae_result;
    };
}}}}

#endif
