/*=============================================================================
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_X3_WITH_MAY_02_2014_0749AM)
#define SPIRIT_X3_WITH_MAY_02_2014_0749AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    // with directive injects a value into the context prior to parsing.
    ///////////////////////////////////////////////////////////////////////////
    template <typename ID>
    struct with_directive : directive<with_directive<ID>>
    {
        static bool const is_pass_through_unary = true;

        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename T>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, T&& val) const
        {
            static bool const copy =
                is_reference<ID>::value && is_reference<T>::value;
            typename
                mpl::if_c<copy, unrefcv_t<T>, T&&>::type
            val_(std::forward<T>(val));
            auto with_context(make_context<unrefcv_t<ID>>(val_, context));
            return subject.parse(first, last, with_context, attr);
        }
    };

    // with<ID>(val): const
    // with<ID&>(val): mutable
    template <typename ID, typename T>
    inline directive_caller<with_directive<ID>, unrefcv_t<T>>
    with(T&& val)
    {
        return {{}, std::forward<T>(val)};
    }
}}}

#endif
