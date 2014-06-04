/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_UNTIL_HPP_INCLUDED
#define BOOST_SPIRIT_X3_UNTIL_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct until_directive : directive<until_directive>
    {
        template <typename Subject>
        struct traits
        {
            static bool const handles_container = true;
            
            template <typename Context>
            using attribute = x3::traits::build_container<
                typename x3::traits::attribute_of<Subject, Context>::type>;
        };
        
        template <typename Terminator>
        static typename extension::as_parser<Terminator>::value_type
        transform_params(Terminator const& terminator)
        {
            return as_parser(terminator);
        }
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename Terminator>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Terminator const& terminator) const
        {
            static_assert(traits::is_parser<Terminator>::value, "invalid terminator");
            
            Iterator it(first);
            while (!terminator.parse(it, last, context, unused))
            {
                if (!detail::parse_into_container(subject, it, last, context, attr))
                    return false;
            }
            first = it;
            return true;
        }
    };

    until_directive const until{};
}}}

#endif
