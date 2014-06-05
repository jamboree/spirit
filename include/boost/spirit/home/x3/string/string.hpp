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
#include <boost/spirit/home/x3/support/traits/string_traits.hpp>
#include <boost/spirit/home/support/char_encoding/ascii.hpp>
#include <boost/spirit/home/support/char_encoding/standard.hpp>
#include <boost/spirit/home/support/char_encoding/standard_wide.hpp>
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

        template <typename Iterator, typename Context, typename Attribute, typename String>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, String const& str) const
        {
            x3::skip_over(first, last, context);
            return detail::string_parse(str, first, last, attr);
        }
        
        // skipper delimited
        template <typename Iterator, typename Context, typename Attribute>
        typename enable_if<has_skipper<Context>, bool>::type
        parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            x3::skip_over(first, last, context);
            auto const& skipper = x3::get<skipper_tag>(context);
            Iterator it(first);
            for ( ; it != last; ++it)
            {
                Iterator i(it);
                if (skipper.parse(i, last, unused, unused))
                    break;
            }
            if (first == it)
                return false;
            traits::move_to(first, it, attr);
            first = it;
            return true;
        }
        
        // til eoi
        template <typename Iterator, typename Context, typename Attribute>
        typename disable_if<has_skipper<Context>, bool>::type
        parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            if (first == last)
                return false;
            traits::move_to(first, last, attr);
            first = last;
            return true;
        }
    };

    namespace standard
    {
        typedef string_parser<char_encoding::standard> string_type;
        string_type const string{};
    }
    
    namespace standard_wide
    {
        typedef string_parser<char_encoding::standard_wide> string_type;
        string_type const string{};
    }

    namespace extension
    {
        namespace detail
        {
            template <typename T>
            struct default_encoding;
            
            template <>
            struct default_encoding<char>
            {
                typedef char_encoding::standard type;
            };
            
            template <>
            struct default_encoding<wchar_t>
            {
                typedef char_encoding::standard_wide type;
            };
        }
        
        template <typename T>
        struct literal<T, typename enable_if<traits::is_string<T>>::type>
        {
            typedef unrefcv_t<typename traits::char_type_of<T>::type> char_type;
            typedef typename detail::default_encoding<char_type>::type encoding;
            typedef string_parser<encoding> type;
        };
    }

    using standard::string_type;
    using standard::string;
}}}

#endif
