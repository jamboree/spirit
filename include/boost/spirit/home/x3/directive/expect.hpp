/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_EXPECT_MARCH_16_2012_1024PM)
#define SPIRIT_EXPECT_MARCH_16_2012_1024PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>

namespace boost { namespace spirit { namespace x3
{
    template <typename Iterator>
    struct expectation_failure : std::runtime_error
    {
        expectation_failure(Iterator first, Iterator last, std::string const& what)
          : std::runtime_error("boost::spirit::x3::expectation_failure")
          , first(first), last(last), what_(what)
        {}
        ~expectation_failure() throw() {}

        Iterator first;
        Iterator last;
        std::string what_;
    };

    struct expect_directive : directive<expect_directive>
    {
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            bool r = subject.parse(first, last, context, attr);

            if (!r)
            {
                boost::throw_exception(
                    expectation_failure<Iterator>(
                        first, last, what(subject)));
            }
            return r;
        }
    };

    expect_directive const expect{};
}}}

#endif
