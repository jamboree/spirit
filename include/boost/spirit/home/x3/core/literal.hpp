/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_LITERAL_HPP_INCLUDED
#define BOOST_SPIRIT_X3_LITERAL_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/caller.hpp>


namespace boost { namespace spirit { namespace x3 { namespace extension
{
    template <typename T>
    struct literal;
}}}}

namespace boost { namespace spirit { namespace x3
{
    struct lit_parser : parser<lit_parser>
    {
        typedef unused_type attribute_type;
        static bool const has_attribute = false;
        static bool const caller_is_pass_through_unary = true;

        template <typename T>
        static caller<typename extension::literal<unrefcv<T>>::type, unrefcv<T>>
        transform_params(T&& val)
        {
            return {{}, std::forward<T>(val)};
        }

        template <typename Iterator, typename Context, typename Attribute, typename Parser>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Parser const& p) const
        {
            static_assert(traits::is_parser<Parser>::value,
                "no parser defined for the literal");
            return p.parse(first, last, context, attr);
        }
    };

    lit_parser const lit{};
}}}

namespace boost { namespace spirit { namespace x3 { namespace extension
{
    template <typename T>
    struct as_parser<T, typename disable_if_substitution_failure<
        typename literal<T>::type>::type>
    {
        typedef
            caller<lit_parser, T>
        type;

        typedef type value_type;

        template <typename U>
        static type call(U&& val)
        {
            return {{}, std::forward<U>(val)};
        }
    };
}}}}


#endif
