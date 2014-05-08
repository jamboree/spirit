/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011-2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_SEEK_HPP_INCLUDED
#define BOOST_SPIRIT_X3_SEEK_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/directive.hpp>


namespace boost { namespace spirit { namespace x3
{
    struct seek_directive : directive<seek_directive>
    {
        static bool const is_pass_through_unary = true;

        // seek[p]
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            Iterator it(first);
            for ( ; !subject.parse(it, last, context, attr); ++it)
            {
                if (it == last)
                    return false;
            }
            first = it;
            return true;
        }
        
        template <typename Stepper>
        static typename extension::as_parser<Stepper>::value_type
        transform_params(Stepper const& stepper)
        {
            return as_parser(stepper);
        }
        
        // seek(s)[p]
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename Stepper>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Stepper const& stepper) const
        {
            static_assert(traits::is_parser<Stepper>::value, "invalid stepper");
            
            Iterator it(first);
            while (!subject.parse(it, last, context, attr))
            {
                if (!stepper.parse(it, last, context, unused))
                    return false;
            }
            first = it;
            return true;
        }
    };
    
    seek_directive const seek{};
}}}


#endif

