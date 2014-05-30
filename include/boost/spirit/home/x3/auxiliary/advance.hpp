/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree
    Copyright (c) 2011 Aaron Graham

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_ADVANCE_HPP_INCLUDED
#define BOOST_SPIRIT_X3_ADVANCE_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/iterator/iterator_traits.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct advance_parser : parser<advance_parser>
    {
        typedef unused_type attribute_type;
        static bool const has_attribute = false;
        static bool const caller_is_pass_through_unary = true;

        template <typename Iterator, typename Context, typename Attribute, typename T>
        bool parse(Iterator& first, Iterator const& last
          , Context const& /*context*/, Attribute& /*attr*/, T dist) const
        {
            // This series of checks is designed to fail parsing on negative
            // values, without generating a "expression always evaluates true"
            // warning on unsigned types.
            if (dist == T(0)) return true;
            if (dist < T(1)) return false;

            typename iterator_category<Iterator>::type tag;
            return advance(tag, dist, first, last);
        }
        
        template <typename T, typename Iterator>
        bool advance(std::forward_iterator_tag, T dist
            , Iterator& first, Iterator const& last) const
        {
            Iterator it(first);
            do
            {
                if (it == last)
                    return false;
                ++it;
            } while (--dist);
            first = it;
            return true;
        }

        template <typename T, typename Iterator>
        bool advance(std::random_access_iterator_tag, T dist
            , Iterator& first, Iterator const& last) const
        {
            if (last - first < dist)
                return false;

            first += dist;
            return true;
        }
    };

    advance_parser const advance{};
}}}

#endif
