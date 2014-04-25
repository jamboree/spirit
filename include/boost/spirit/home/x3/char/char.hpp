/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_CHAR_APRIL_16_2006_1051AM)
#define BOOST_SPIRIT_X3_CHAR_APRIL_16_2006_1051AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/literal.hpp>
#include <boost/spirit/home/x3/char/char_parser.hpp>
#include <boost/spirit/home/x3/char/detail/char_set.hpp>
#include <boost/spirit/home/x3/char/detail/cast_char.hpp>
#include <boost/spirit/home/x3/support/traits/string_traits.hpp>
#include <boost/spirit/home/support/char_encoding/ascii.hpp>
#include <boost/spirit/home/support/char_encoding/standard.hpp>
#include <boost/spirit/home/support/char_encoding/standard_wide.hpp>
#include <boost/array.hpp>


namespace boost { namespace spirit { namespace x3
{
    namespace detail
    {
        template <typename T>
        struct is_multi_string : traits::is_string<T> {};
        
        template <typename T, std::size_t N>
        struct is_multi_string<T[N]>
          : mpl::bool_<traits::is_char<T>::value && (N > 2)> {};
    }
    
    template <typename Encoding>
    struct any_char
    {
        typedef typename Encoding::char_type char_type;
        typedef std::pair<char_type, char_type> char_range;
        typedef detail::char_set<char_type> char_set;
        typedef array<char_type, 2> small_set;
        
        // char_
        template <typename Char, typename Context>
        static bool test(Char ch, Context const&)
        {
            return ((sizeof(Char) <= sizeof(char_type)) || Encoding::ischar(ch));
        }

        template <typename Char>
        static typename enable_if<traits::is_char<Char>, char_type>::type
        transform_params(Char ch)
        {
            return char_type(ch);
        }

        // char_('a')
        template <typename Char, typename Context>
        static bool test(Char ch, Context const& ctx, char_type ch_)
        {
            return test(ch, ctx) && char_type(ch) == ch_;
        }
        
        template <typename Char>
        static small_set transform_params(Char(&s)[2])
        {
            return {char_type(s[0]), char_type(s[1])};
        }
        
        // char_("a")
        template <typename Char, typename Context>
        static bool test(Char ch, Context const& ctx, small_set const& s)
        {
            char_type ch_ = char_type(ch);
            return test(ch, ctx) && (s[0] == ch_ || s[1] == ch_);
        }

        template <typename Char>
        static char_range transform_params(Char a, Char b)
        {
            return {char_type(a), char_type(b)};
        }
        
        // char_('a', 'z')
        template <typename Char, typename Context>
        static bool test(Char ch, Context const& ctx, char_range const& r)
        {
            char_type ch_ = char_type(ch);
            return test(ch, ctx) && !(ch_ < r.first) && !(r.second < ch_);
        }

        template <typename String>
        static typename
            enable_if<detail::is_multi_string<String>, char_set>::type
        transform_params(String const& str)
        {
            using detail::cast_char;
            
            typedef typename
                remove_const<
                    typename traits::char_type_of<String>::type
                >::type
            in_type;
            
            static_assert(sizeof(char_type) >= sizeof(in_type),
                "cannot convert string");
                
            char_set chset;
            auto definition = traits::get_c_string(str);
            in_type ch = *definition++;
            while (ch)
            {
                in_type next = *definition++;
                if (next == '-')
                {
                    next = *definition++;
                    if (next == 0)
                    {
                        chset.add(cast_char<char_type>(ch));
                        chset.add('-');
                        break;
                    }
                    chset.add(
                        cast_char<char_type>(ch),
                        cast_char<char_type>(next)
                    );
                }
                else
                {
                    chset.add(cast_char<char_type>(ch));
                }
                ch = next;
            }
            return std::move(chset);
        }

        // char_("a-z")
        template <typename Char, typename Context>
        static bool test(Char ch, Context const& ctx, char_set const& chset)
        {
            return test(ch, ctx) && chset.test(char_type(ch));
        }
    };
    
    namespace standard
    {
        typedef char_parser<any_char<char_encoding::standard>> char_type;
        char_type const char_{};
    }

    using standard::char_type;
    using standard::char_;

    namespace standard_wide
    {
        typedef char_parser<any_char<char_encoding::standard_wide>> char_type;
        char_type const char_{};
    }

    namespace ascii
    {
        typedef char_parser<any_char<char_encoding::ascii>> char_type;
        char_type const char_{};
    }

    namespace extension
    {
        template <>
        struct literal<char>
        {
            typedef standard::char_type type;
        };
        
        template <>
        struct literal<wchar_t>
        {
            typedef standard_wide::char_type type;
        };
    }
}}}

#endif
