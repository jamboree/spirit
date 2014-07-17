/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    http://spirit.sourceforge.net/

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CONTEXT_JAN_4_2012_1215PM)
#define BOOST_SPIRIT_X3_CONTEXT_JAN_4_2012_1215PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename ID, typename T, typename Next = unused_type>
    struct context
    {
        context(T& val, Next const& next)
            : val(val), next(next) {}

        template <typename ID_, typename Unused = void>
        struct get_result
        {
            typedef typename Next::template get_result<ID_>::type type;
        };

        template <typename Unused>
        struct get_result<mpl::identity<ID>, Unused>
        {
            typedef T& type;
        };

        T& get(mpl::identity<ID>) const
        {
            return val;
        }

        template <typename ID_>
        typename Next::template get_result<ID_>::type
        get(ID_ id) const
        {
            return next.get(id);
        }
        
        Next const& get_next() const
        {
            return next;
        }

        T& val;
        Next const& next;
    };

    template <typename ID, typename T>
    struct context<ID, T, unused_type>
    {
        context(T& val)
            : val(val) {}

        context(T& val, unused_type)
            : val(val) {}

        template <typename ID_, typename Unused = void>
        struct get_result
        {
            typedef unused_type type;
        };

        template <typename Unused>
        struct get_result<mpl::identity<ID>, Unused>
        {
            typedef T& type;
        };

        T& get(mpl::identity<ID>) const
        {
            return val;
        }

        template <typename ID_>
        unused_type
        get(ID_) const
        {
            return unused;
        }
        
        unused_type get_next() const
        {
            return unused;
        }

        T& val;
    };
        
    template <typename Tag, typename Context>
    inline auto
    get(Context const& context) -> decltype(context.get(mpl::identity<Tag>()))
    {
        return context.get(mpl::identity<Tag>());
    }

    template <typename ID, typename T, typename Next>
    inline context<ID, T, Next> make_context(T& val, Next const& next)
    {
        return context<ID, T, Next>(val, next);
    }

    template <typename ID, typename T>
    inline context<ID, T> make_context(T& val)
    {
        return context<ID, T>(val);
    }
}}}

#endif
