/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_REGEX_HPP_INCLUDED
#define BOOST_SPIRIT_X3_REGEX_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <boost/spirit/home/x3/support/traits/string_traits.hpp>
#include <boost/regex.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Char, typename Traits = regex_traits<Char>>
    struct regex_parser : parser<regex_parser<Char, Traits>>
    {
        typedef basic_regex<Char, Traits> regex;
        typedef typename regex::flag_type flag_type;
        typedef typename Traits::string_type attribute_type;
        static bool const has_attribute = true;
        static bool const handles_container = true;
        static bool const caller_is_pass_through_unary = true;

        explicit regex_parser(flag_type flag = regex_constants::normal)
          : flag(flag)
        {}
        
        template <typename String>
        typename enable_if<traits::is_string<String>, regex>::type
        transform_params(String const& str) const
        {
            return regex(str, flag);
        }
        
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, regex const& e) const
        {
            x3::skip_over(first, last, context);
            match_results<Iterator> m;
            if (regex_search(first, last, m, e, regex_constants::match_continuous))
            {
                auto const& m0 = m[0];
                traits::move_to(m0.first, m0.second, attr);
                first = m0.second;
                return true;
            }
            return false;
        }
        
        flag_type flag;
    };
    
    typedef regex_parser<char> regex_type;
    typedef regex_parser<wchar_t> wregex_type;
    
    regex_type const regex{};
    wregex_type const wregex{};
}}}


#endif

        
