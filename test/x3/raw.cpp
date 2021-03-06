/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>

#include <iostream>
#include <string>
#include "test.hpp"

int
main()
{
    using spirit_test::test;
    using spirit_test::test_attr;
    using namespace boost::spirit::x3::ascii;
    using boost::spirit::x3::raw;
    using boost::spirit::x3::eps;

    {
        boost::iterator_range<char const*> range;
        std::string str;
        BOOST_TEST((test_attr("spirit_test_123", raw[alpha >> *(alnum | '_')], range)));
        BOOST_TEST((std::string(range.begin(), range.end()) == "spirit_test_123"));
        BOOST_TEST((test_attr("  spirit", raw[*alpha], range, space)));
        BOOST_TEST((range.size() == 6));
    }

    {
        std::string str;
        BOOST_TEST((test_attr("spirit_test_123", raw[alpha >> *(alnum | '_')], str)));
        BOOST_TEST((str == "spirit_test_123"));
    }

    {
        boost::iterator_range<char const*> range;
        BOOST_TEST((test("x", raw[alpha])));
        BOOST_TEST((test_attr("x", raw[alpha], range)));
    }
    
    {
        boost::iterator_range<char const*> range;
        BOOST_TEST((test("x", raw[alpha][ ([&](auto& attr){ range = attr; }) ])));
        BOOST_TEST(range.size() == 1 && *range.begin() == 'x');
    }

    return boost::report_errors();
}
