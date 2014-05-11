/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>

#include <iostream>
#include "test.hpp"

int
main()
{
    using spirit_test::test;
    using boost::spirit::x3::ascii::space;
    using boost::spirit::x3::ascii::space_type;
    using boost::spirit::x3::ascii::digit;
    using boost::spirit::x3::ascii::alpha;
    using boost::spirit::x3::ascii::alnum;
    using boost::spirit::x3::lexeme;

    { // lexeme[p]
        BOOST_TEST((test(" 1 2 3 4 5", +digit, space)));
        BOOST_TEST((!test(" 1 2 3 4 5", lexeme[+digit], space)));
        BOOST_TEST((test(" 12345", lexeme[+digit], space)));
        BOOST_TEST((test(" 12345  ", lexeme[+digit], space, false)));

        // lexeme collapsing
        BOOST_TEST((!test(" 1 2 3 4 5", lexeme[lexeme[+digit]], space)));
        BOOST_TEST((test(" 12345", lexeme[lexeme[+digit]], space)));
        BOOST_TEST((test(" 12345  ", lexeme[lexeme[+digit]], space, false)));
    }

    { // lexeme(lex)[p]
        auto const lex = (alpha | '_') >> *(alnum | '_');
        BOOST_TEST((test(" int  ", lexeme(lex)["int"], space, false)));
        BOOST_TEST((!test(" integer  ", lexeme(lex)["int"], space, false)));
    }

    return boost::report_errors();
}
