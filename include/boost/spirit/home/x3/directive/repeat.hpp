/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_REPEAT_NOVEMBER_14_2008_1148AM)
#define SPIRIT_REPEAT_NOVEMBER_14_2008_1148AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace boost { namespace spirit { namespace x3
{
    // Infinite loop tag type
    struct inf_type {};
    inf_type const inf{};
    
    struct repeat_directive : directive<repeat_directive>
    {
        template <typename Subject>
        struct traits
        {
            static bool const handles_container = true;
            
            template <typename Context>
            using attribute = x3::traits::build_container<
                typename x3::traits::attribute_of<Subject, Context>::type>;
        };
        
        template <typename T>
        struct exact_iterator
        {
            typedef T type;
            bool got_max(T i) const { return i >= exact; }
            bool got_min(T i) const { return i >= exact; }
    
            T const exact;
        };
        
        template <typename T>
        struct finite_iterator
        {
            typedef T type;
            bool got_max(T i) const { return i >= max; }
            bool got_min(T i) const { return i >= min; }
    
            T const min;
            T const max;
        };

        template <typename T>
        struct infinite_iterator
        {
            typedef T type;
            bool got_max(T /*i*/) const { return false; }
            bool got_min(T i) const { return i >= min; }
    
            T const min;
        };
        
        // repeat(exact)[p]
        template <typename T>
        static typename enable_if<is_integral<T>, exact_iterator<T>>::type
        transform_params(T exact)
        {
            return {exact};
        }
        
        // repeat(min, max)[p]
        template <typename T>
        static typename enable_if<is_integral<T>, finite_iterator<T>>::type
        transform_params(T min, T max)
        {
            return {min, max};
        }
        
        // repeat(min, inf)[p]
        template <typename T>
        static typename enable_if<is_integral<T>, infinite_iterator<T>>::type
        transform_params(T min, inf_type)
        {
            return {min};
        }
        
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename LoopIter>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, LoopIter const& iter) const
        {
            typename LoopIter::type i(0);
            for (/**/; !iter.got_min(i); ++i)
            {
                if (!detail::parse_into_container(subject, first, last, context, attr))
                    return false;
            }
            // parse some more up to the maximum specified
            for (/**/; !iter.got_max(i); ++i)
            {
                if (!detail::parse_into_container(subject, first, last, context, attr))
                    break;
            }
            return true;
        }
        
        // repeat[p]
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            while (detail::parse_into_container(subject, first, last, context, attr));
            return true;
        }
    };

    repeat_directive const repeat{};
}}}

#endif
