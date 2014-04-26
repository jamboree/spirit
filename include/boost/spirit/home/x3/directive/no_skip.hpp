/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2013 Agustin Berge

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_NO_SKIP_JAN_16_2010_0802PM)
#define SPIRIT_NO_SKIP_JAN_16_2010_0802PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace spirit { namespace x3
{
    // same as lexeme[], but does not pre-skip
    struct no_skip_directive : directive<no_skip_directive>
    {
        static bool const is_pass_through_unary = true;
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        typename enable_if<has_skipper<Context>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            auto const& skipper = x3::get<skipper_tag>(context);

            typedef unused_skipper<
                typename remove_reference<decltype(skipper)>::type>
            unused_skipper_type;
            unused_skipper_type unused_skipper(skipper);

            return subject.parse(
                first, last
              , make_context<skipper_tag>(unused_skipper, context)
              , attr);
        }
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        typename disable_if<has_skipper<Context>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return subject.parse(first, last, context, attr);
        }
    };

    no_skip_directive const no_skip{};
}}}

#endif
