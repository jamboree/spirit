/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>

#include <string>
#include "test.hpp"

int
main()
{
    using spirit_test::test;
    using spirit_test::test_attr;
    using boost::spirit::x3::unused_type;
    using boost::spirit::x3::regex;
    using boost::spirit::x3::space;

    { // basics
        BOOST_TEST((test("int8", regex(R"([a-zA-Z]\w+)"))));
        BOOST_TEST((!test("8int", regex(R"([a-zA-Z]\w+)"))));
        BOOST_TEST((test("int+", regex(R"([a-zA-Z]\w+)") >> '+')));
        BOOST_TEST((test("int8 a", regex(R"([a-zA-Z]\w+)") >> 'a', space)));
        
        std::string attr;
        BOOST_TEST((test_attr("int+", regex(R"([a-zA-Z]\w+)"), attr, false)));
        BOOST_TEST(attr == "int");
    }
    
    { // lazy
        auto val = [](char const* str) { return [str](unused_type) { return str; }; };
        BOOST_TEST((test("int8", regex(val(R"([a-zA-Z]\w+)")))));
        
        std::string attr;
        BOOST_TEST((test_attr("int+", regex(val(R"([a-zA-Z]\w+)")), attr, false)));
        BOOST_TEST(attr == "int");
    }

    return boost::report_errors();
}
