/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2011-2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <vector>

#include "test.hpp"


///////////////////////////////////////////////////////////////////////////////
int main()
{
    using namespace spirit_test;
    using boost::spirit::x3::seek;
    using boost::spirit::x3::eoi;
    using boost::spirit::x3::int_;
    using boost::spirit::x3::char_;
    using boost::spirit::x3::lit;
    using boost::spirit::x3::advance;
    using boost::spirit::x3::standard::space;

    { // test eoi
        BOOST_TEST(test("", seek[eoi]));
        BOOST_TEST(test(" ", seek[eoi], space));
        BOOST_TEST(test("a", seek[eoi]));
        BOOST_TEST(test(" a", seek[eoi], space));
    }

    { // test literal finding
        int i = 0;

        BOOST_TEST(
            test_attr("!@#$%^&*KEY:123", seek["KEY:"] >> int_, i)
            && i == 123
        );
    }

    { // test sequence finding
        int i = 0;

        BOOST_TEST(
            test_attr("!@#$%^&* KEY : 123", seek[lit("KEY") >> ':'] >> int_, i, space)
            && i == 123
        );
    }

    { // test attr finding
        std::vector<int> v;

        BOOST_TEST( // expect partial match
            test_attr("a06b78c3d", +seek[int_], v, false)
            && v[0] == 6 && v[1] == 78 && v[2] == 3
        );
    }
    
    { // test stepper
        std::vector<int> v;
        char const str[] = "gactgcatactg";
        auto begin = std::begin(str), end = std::end(str) - 1;
        char const* it;
        BOOST_TEST(parse(it = begin, end, seek["actg"]) && it == str + 5);
        BOOST_TEST(parse(it = begin, end, seek(advance(4))["actg"]) && it == str + 12);
    }
        
    { // test action
        bool b = false;
        auto f = [&b]{ b = true; };

        BOOST_TEST( // expect partial match
            test("abcdefg", seek["def"][f], false)
            && b
        );
    }

    return boost::report_errors();
}
