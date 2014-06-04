/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree
    Copyright (c) 2009 Chris Hoeppler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_CONFIX_HPP_INCLUDED
#define BOOST_SPIRIT_X3_CONFIX_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/directive.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct confix_directive : directive<confix_directive>
    {
        static bool const is_pass_through_unary = true;
        
        template <typename Prefix, typename Suffix>
        static std::pair<
            typename extension::as_parser<Prefix>::value_type
          , typename extension::as_parser<Suffix>::value_type>
        transform_params(Prefix const& prefix, Suffix const& suffix)
        {
            return {as_parser(prefix), as_parser(suffix)};
        }
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename Prefix, typename Suffix>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, std::pair<Prefix, Suffix> const& enclose) const
        {
            Iterator it(first);

            if (enclose.first.parse(it, last, context, unused) &&
                subject.parse(it, last, context, attr) &&
                enclose.second.parse(it, last, context, unused))
            {
                first = it;
                return true;
            }
            return false;
        }
    };

    confix_directive const confix{};
}}}

#endif
