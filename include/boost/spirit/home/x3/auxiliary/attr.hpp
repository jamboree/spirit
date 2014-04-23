/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2013 Agustin Berge

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_SPIRIT_X3_ATTR_JUL_23_2008_0956AM
#define BOOST_SPIRIT_X3_ATTR_JUL_23_2008_0956AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>

namespace boost { namespace spirit { namespace x3
{
    struct attr_parser : parser<attr_parser>
    {
        template <typename T>
        struct caller_traits
        {
            typedef typename remove_reference<T>::type attribute_type;
            static bool const has_attribute =
                !is_same<unused_type, attribute_type>::value;
            static bool const handles_container =
                is_array<attribute_type>::value || // should array considered a container?
                traits::is_container<attribute_type>::value;
        };

        template <typename Iterator, typename Context, typename Attribute, typename T>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr_, T value) const
        {
            traits::move_to(value, attr_);
            return true;
        }
        
        template <typename Iterator, typename Context, typename Attribute, typename T, std::size_t N>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr_, T (&value)[N]) const
        {
            // $$$ Change to copy_to once we have it $$$
            traits::move_to(value + 0, value + N, attr_);
            return true;
        }
    };
    
    template <>
    struct get_info<attr_parser>
    {
        typedef std::string result_type;
        std::string operator()(attr_parser const& /*p*/) const
        {
            return "attr";
        }
    };

    attr_parser const attr{};
}}}

#endif
