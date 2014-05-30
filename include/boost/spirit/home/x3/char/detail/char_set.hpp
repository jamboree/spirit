/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_DETAIL_CHAR_SET_HPP_INCLUDED
#define BOOST_SPIRIT_X3_DETAIL_CHAR_SET_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <bitset>
#include <boost/utility/enable_if.hpp>
#include <boost/icl/interval_set.hpp>

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Char, typename = void>
    struct char_set
    {
        void add(Char ch)
        {
            set.add(ch);
        }
        
        void add(Char from, Char to)
        {
            set.add(icl::interval<Char>::closed(from, to));
        }
        
        bool test(Char ch) const
        {
            return set.find(ch) != set.end();
        }
        
    private:
        
        icl::interval_set<Char> set;
    };
    
    template <typename Char>
    struct char_set<Char, typename enable_if_c<sizeof(Char) == 1>::type>
    {
        typedef unsigned char index;
        
        void add(Char ch)
        {
            set.set(index(ch));
        }
        
        void add(Char from, Char to)
        {
            for ( ; from <= to; ++from)
                set.set(index(from));
        }
        
        bool test(Char ch) const
        {
            return set.test(index(ch));
        }
        
    private:
        
        std::bitset<256> set;
    };
}}}}

#endif
