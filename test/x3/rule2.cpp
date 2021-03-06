/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

// this file deliberately contains non-ascii characters
// boostinspect:noascii

#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>

#include <string>
#include <cstring>
#include <iostream>
#include "test.hpp"

int
main()
{
    using spirit_test::test_attr;
    using spirit_test::test;

    using namespace boost::spirit::x3::ascii;
    //~ using boost::spirit::x3::locals;
    using boost::spirit::x3::rule;
    using boost::spirit::x3::int_;
    //~ using boost::spirit::x3::uint_;
    //~ using boost::spirit::x3::fail;
    //~ using boost::spirit::x3::on_error;
    //~ using boost::spirit::x3::debug;
    using boost::spirit::x3::lit;
    using boost::spirit::x3::unused_type;

    { // context tests

        char ch;
        auto a = rule<class a, char>() = alpha;

        // this semantic action requires both the context and attrubute
        auto f = [&](auto&, char attr){ ch = attr; };
        BOOST_TEST(test("x", a[f]));
        BOOST_TEST(ch == 'x');

        // the semantic action may optionally not require the context to be passed
        auto f2 = [&](char attr){ ch = 'y'; };
        BOOST_TEST(test("x", a[f2]));
        BOOST_TEST(ch == 'y');

        // the semantic action may optionally not have any arguments at all
        auto f3 = [&]{ ch = 'z'; };
        BOOST_TEST(test("x", a[f3]));
        BOOST_TEST(ch == 'z');

        BOOST_TEST(test_attr("z", a, ch)); // attribute is given.
        BOOST_TEST(ch == 'z');
    }

    { // auto rules tests

        char ch = '\0';
        auto a = rule<class a, char>() = alpha;
        auto f = [&](char attr){ ch = attr; };

        BOOST_TEST(test("x", a[f]));
        BOOST_TEST(ch == 'x');
        ch = '\0';
        BOOST_TEST(test_attr("z", a, ch)); // attribute is given.
        BOOST_TEST(ch == 'z');

        ch = '\0';
        BOOST_TEST(test("x", a[f]));
        BOOST_TEST(ch == 'x');
        ch = '\0';
        BOOST_TEST(test_attr("z", a, ch)); // attribute is given.
        BOOST_TEST(ch == 'z');
    }

    { // auto rules tests: allow stl containers as attributes to
      // sequences (in cases where attributes of the elements
      // are convertible to the value_type of the container or if
      // the element itself is an stl container with value_type
      // that is convertible to the value_type of the attribute).

        std::string s;
        auto f = [&](std::string attr){ s = attr; };

        {
            auto r = rule<class r, std::string>()
                = char_ >> *(',' >> char_)
                ;

            BOOST_TEST(test("a,b,c,d,e,f", r[f]));
            BOOST_TEST(s == "abcdef");
        }

        {
            auto r = rule<class r, std::string>()
                = char_ >> *(',' >> char_);
            s.clear();
            BOOST_TEST(test("a,b,c,d,e,f", r[f]));
            BOOST_TEST(s == "abcdef");
        }

        {
            auto r = rule<class r, std::string>()
                = char_ >> char_ >> char_ >> char_ >> char_ >> char_;
            s.clear();
            BOOST_TEST(test("abcdef", r[f]));
            BOOST_TEST(s == "abcdef");
        }
    }

    return boost::report_errors();
}
