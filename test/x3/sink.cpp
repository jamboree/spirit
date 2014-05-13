/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/ref.hpp>

#include <string>
#include "test.hpp"

int main()
{
    using spirit_test::test;
    using spirit_test::test_attr;
    using boost::spirit::x3::unused_type;
    using boost::spirit::x3::char_;
    using boost::spirit::x3::sink;
    
    using boost::ref;
    
    using boost::fusion::vector;
    using boost::fusion::at_c;

    {
        char c = 0;
        BOOST_TEST((test("c", sink(ref(c))[char_]) && c == 'c'));
    }
    
    {
        char c = 0, attr = 0;
        BOOST_TEST((test_attr("abc", sink(ref(c))[char_] >> 'b' >> char_, attr)));
        BOOST_TEST((c == 'a' && attr == 'c'));
    }
    
    {
        vector<char, char> data, attr;
        BOOST_TEST((test_attr("abcde",
            char_ >> sink(ref(data))[char_ >> 'c' >> char_] >> char_, attr)));
        BOOST_TEST((at_c<0>(data) == 'b'));
        BOOST_TEST((at_c<1>(data) == 'd'));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'e'));
    }
    
    { // lazy
        char c = 0;
        auto f = [&c](unused_type)->char& { return c; };
        BOOST_TEST((test("c", sink(f)[char_]) && c == 'c'));
    }

    return boost::report_errors();
}
