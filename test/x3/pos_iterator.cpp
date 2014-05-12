/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/pos_iterator.hpp>

int main()
{
    using boost::spirit::x3::pos_iterator;
    using boost::spirit::x3::parse;
    using boost::spirit::x3::char_;
    using boost::spirit::x3::newl;
    
    char const str[] =
        "0\r"
        "0+2\n"
        "0123-\r\n"
        "*"
        ;
    auto begin = std::begin(str);
    pos_iterator<char const*> it, end(std::end(str) - 1);

    { // newl
        BOOST_TEST((parse(it = begin, end, *((newl | char_) - '+'))
          && it.row() == 1 && it.col() == 1 && it.line_begin() - begin == 2));
        BOOST_TEST((parse(it = begin, end, *((newl | char_) - '-'))
          && it.row() == 2 && it.col() == 4 && it.line_begin() - begin == 6));
        BOOST_TEST((parse(it = begin, end, *((newl | char_) - '*'))
          && it.row() == 3 && it.col() == 0 && it.line_begin() - begin == 13));
    }
    
    { // newl(p)
        BOOST_TEST((parse(it = begin, end, *((newl('\n') | char_) - '-'))
          && it.row() == 1 && it.col() == 4 && it.line_begin() - begin == 6));
        BOOST_TEST((parse(it = begin, end, *((newl("\r\n") | char_) - '*'))
          && it.row() == 1 && it.col() == 0 && it.line_begin() - begin == 13));
    }

    return boost::report_errors();
}
