/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_SKIP_JANUARY_26_2008_0422PM)
#define SPIRIT_SKIP_JANUARY_26_2008_0422PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct skip_directive : directive<skip_directive>
    {
        static bool const is_pass_through_unary = true;
        
        // skip[p] - reskip
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        typename disable_if<has_skipper<Context>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            auto const& skipper =
                detail::get_unused_skipper(x3::get<skipper_tag>(context));

            return subject.parse(
                first, last
              , make_context<skipper_tag>(skipper, context)
              , attr);
        }
        
        // skip[p] - as is
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        typename enable_if<has_skipper<Context>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return subject.parse(first, last, context, attr);
        }
        
        template <typename Skipper>
        static typename extension::as_parser<Skipper>::value_type
        transform_params(Skipper const& skipper)
        {
            return as_parser(skipper);
        }
        
        // skip(s)[p]
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename Skipper>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Skipper const& skipper) const
        {
            static_assert(traits::is_parser<Skipper>::value, "invalid skipper");
            return subject.parse(
                first, last
              , make_context<skipper_tag>(skipper, context)
              , attr);
        }
    };

    skip_directive const skip{};
}}}

#endif
