/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_STRING_APR_18_2006_1125PM)
#define BOOST_SPIRIT_X3_STRING_APR_18_2006_1125PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/string/detail/string_parse.hpp>
#include <boost/spirit/home/x3/support/utility/utf8.hpp>
#include <boost/spirit/home/support/char_encoding/ascii.hpp>
#include <boost/spirit/home/support/char_encoding/standard.hpp>
#include <boost/spirit/home/support/char_encoding/standard_wide.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <string>

namespace boost { namespace spirit { namespace x3
{
    template <typename Encoding>
    struct string_parser : parser<string_parser<Encoding>>
    {
        typedef typename Encoding::char_type char_type;
        typedef std::basic_string<char_type> attribute_type;
        static bool const has_attribute = true;
        static bool const handles_container = true;
        static bool const caller_is_pass_through_unary = true;

        template <typename Iterator, typename Context, typename Attribute_, typename String>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr, String const& str) const
        {
            x3::skip_over(first, last, context);
            return detail::string_parse(str, first, last, attr);
        }
        
        // skipper delimited
        template <typename Iterator, typename Context, typename Attribute_>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr) const
        {
            x3::skip_over(first, last, context);
            auto const& skipper = get<skipper_tag>(context);
            Iterator it(first);
            for ( ; it != last; ++it)
            {
                Iterator i(it);
                if (skipper.parse(i, last, unused, unused))
                    break;
            }
            traits::move_to(first, it, attr);
            first = it;
            return true;
        }
    };

    namespace standard
    {
        typedef string_parser<char_encoding::standard> string_type;
        string_type const string{};
    }

    namespace extension
    {
        template <int N>
        struct literal<char[N]>
        {
            typedef string_parser<char_encoding::standard> type;
        };

        template <int N>
        struct literal<wchar_t[N]>
        {
            typedef string_parser<char_encoding::standard_wide> type;
        };
    }

    using standard::string_type;
    using standard::string;
}}}

#endif
