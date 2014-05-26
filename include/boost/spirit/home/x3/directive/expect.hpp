/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_EXPECT_MARCH_16_2012_1024PM)
#define SPIRIT_EXPECT_MARCH_16_2012_1024PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/expectation.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct expect_directive : directive<expect_directive>
    {
        static bool const is_pass_through_unary = true;
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        typename disable_if<traits::handles_expectation<Subject>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            Iterator entry(first);
            if (!subject.parse(first, last, context, attr))
            {
                boost::throw_exception(
                    expectation_failure<Iterator>(entry, what(subject)));
            }
            return true;
        }
        
        template <typename Subject, typename... Ts, typename Iterator, typename Context, typename Attribute>
        typename enable_if<traits::handles_expectation<Subject>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            return subject.parse(first, last,
                make_context<expectation_tag>(unused, context), attr);
        }
    };

    expect_directive const expect{};
}}}

namespace boost { namespace spirit { namespace x3 { namespace traits
{
    template <typename Subject>
    struct handles_expectation<directive_parser<expect_directive, Subject>>
      : mpl::true_ {};
}}}}

#endif
