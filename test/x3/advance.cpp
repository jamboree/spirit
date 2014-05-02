//  Copyright (c) 2001-2010 Hartmut Kaiser
//  Copyright (c) 2001-2010 Joel de Guzman
//  Copyright (c) 2011 Aaron Graham
//  Copyright (c) 2014 Jamboree
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/actor.hpp>
#include <boost/phoenix/operator.hpp>

#include <forward_list>
#include "test.hpp"

namespace spirit_test
{
    template <typename Container, typename Parser>
    bool test_c(Container const& in, Parser const& p, bool full_match = true)
    {
        typename Container::const_iterator first = in.begin();
        typename Container::const_iterator const last = in.end();
        return boost::spirit::x3::parse(first, last, p)
            && (!full_match || (first == last));
    }
}

int main()
{
    using spirit_test::test;
    using spirit_test::test_c;

    using boost::spirit::x3::locals;
    using boost::spirit::x3::uint_;
    //using boost::spirit::x3::byte_;
    boost::spirit::x3::char_type byte_;
    using boost::spirit::x3::advance;

    { // test basic functionality with random-access iterators
        BOOST_TEST(test("a123bc", 'a' >> advance(3) >> "bc"));
        BOOST_TEST(test("x3", (advance(3) | 'x') >> '3'));
        BOOST_TEST(!test("0", advance(-1)));
        BOOST_TEST(test("x3", advance(-1) | "x3"));
        BOOST_TEST(test("abcdefghijklmnopqrstuvwxyz",
            advance(0) >> "abc" >> advance(10) >> "nopq" >> advance(0)
         >> advance(8) >> 'z'));
    }

    { // test locals
        BOOST_SPIRIT_USE_ACTORS(_1, _a)

        auto start = locals<unsigned>()[byte_ [_a = _1] >> advance(_a) >> "345"];
        BOOST_TEST(test("\x02""12345", start));
        BOOST_TEST(!test("\x60""345", start));
    }

    { // test basic functionality with forward iterators
        typedef std::forward_list<char> list;
        
        BOOST_SPIRIT_USE_ACTORS(_1, _a)

        BOOST_TEST(test_c(list{1,2,'a','b','c'}
            , locals<int>()[byte_ [_a = _1] >> advance(_a) >> "abc"]));
        BOOST_TEST(!test_c(list{3,'x','3'}
            , locals<int>()[byte_ [_a = _1] >> advance(_a)]));
        BOOST_TEST(test_c(list{3,'x','3'}
            , locals<int>()[byte_ [_a = _1] >> (advance(_a) | "x3")]));
        BOOST_TEST(test_c(list{'a','b','c','d','e','f','g','h','i','j','k'
            ,'l','m', 'n','o','p','q','r','s','t','u','v','w','x','y','z'}
            , advance(0) >> "abc" >> advance(10) >> "nopq" >> advance(0)
           >> advance(8) >> 'z'));
    }

    return boost::report_errors();
}

