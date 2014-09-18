/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_DETAIL_PACK_PARAMS_HPP_INCLUDED
#define BOOST_SPIRIT_X3_DETAIL_PACK_PARAMS_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <type_traits>
#include <boost/ref.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/declval.hpp>
#include <boost/type_traits/has_trivial_copy.hpp>
#include <boost/spirit/home/x3/core/eval.hpp>
#include <boost/spirit/home/x3/support/utility/sfinae.hpp>
#include <boost/spirit/home/x3/support/utility/integer_sequence.hpp>

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template<class T>
    struct mover
    {
        mover(T const& val) : val(val) {}
        
        mover(T&& val) : val(std::move(val)) {}
        
        mover(mover const& other) = default;
    
        mover(mover&& other) = default; 
    
        mover(mover& other) : val(std::move(other.val)) {}
    
        operator T const&() const
        {
            return val; 
        }
            
        T val;
    };
    
    template <typename T>
    struct wrap_param
      : std::conditional
        <
            std::is_move_constructible<T>::value
        && !(std::is_copy_constructible<T>::value
          && boost::has_trivial_copy_constructor<T>::value)
          , mover<T>
          , T
        >
    {};

    template <typename T, std::size_t N>
    struct wrap_param<T[N]>
    {
        typedef T const(&type)[N];
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
    struct pack_params;
    
    template <typename Subject, typename Enable, typename... Ts>
    struct transform_params
    {
        typedef bool no_transform;
    };
    
    template <typename Subject, typename... Ts>
    struct transform_params<Subject, typename disable_if_substitution_failure<
        decltype(declval<Subject const>().transform_params(declval<Ts>()...))>::type, Ts...>
    {
        typedef
            decltype(declval<Subject const>().transform_params(declval<Ts>()...))
        type;

        typedef bool has_transform;
    };
    
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
    struct args_pack
    {
        static auto make(wrap_param_t<Ts>... ts)
        {
            return [=](auto const& subject, auto&& ctx, auto&& parse)
            {
                return parse_unpacked(subject, parse,
                    x3::eval<unwrap_param_t<Ts>>(ts, ctx)...);
            };
        }
        
        typedef decltype(make(std::declval<Ts>()...)) type;
    };

    template <typename Subject, typename Enable, typename... Ts>
    struct pack_params
    {
        template <typename... As>
        static auto pack(Subject const& subject, As&&... as)
        {
            return args_pack<Ts...>::make(std::forward<As>(as)...);
        }
        
        typedef typename args_pack<Ts...>::type type;
    };
    
    template <typename T>
    struct result_pack
    {
        template <typename Subject, typename Context, typename Parse>
        bool operator()(Subject const&, Context&&, Parse&& parse) const
        {
            return parse(val);
        }
        
        T val;
    };
        
    template <typename Subject, typename... Ts>
    struct pack_params<Subject, typename disable_if_substitution_failure<
        decltype(declval<Subject const>().transform_params(declval<Ts>()...))>::type, Ts...>
    {
        typedef
            decltype(declval<Subject const>().transform_params(declval<Ts>()...))
        transform_type;

        typedef result_pack<transform_type> type;
        
        template <typename... As>
        static type pack(Subject const& subject, As&&... as)
        {
            return {subject.transform_params(std::forward<As>(as)...)};
        }
    };
    
    template <typename Subject, typename... Ts>
    using transform_params_t =
        typename transform_params<Subject, void, Ts...>::type;
}}}}

#endif
