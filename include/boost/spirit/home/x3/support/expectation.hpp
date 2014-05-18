/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_EXPECTATION_HPP_INCLUDED
#define BOOST_SPIRIT_X3_EXPECTATION_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/throw_exception.hpp>
#include <stdexcept>


namespace boost { namespace spirit { namespace x3
{
    struct expectation_tag;

    template <typename Iterator>
    struct expectation_failure : std::runtime_error
    {
        expectation_failure(Iterator pos, std::string const& info)
          : std::runtime_error("boost::spirit::x3::expectation_failure")
          , pos(pos), info(info), rethrow_count()
        {}
        
        ~expectation_failure() throw() {}

        Iterator pos;
        std::string info;
        unsigned rethrow_count;
    };
}}}


#endif

