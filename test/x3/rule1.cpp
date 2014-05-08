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


namespace g
{
    namespace x3 = boost::spirit::x3;
    using x3::lit;

    auto a = lit('a');
    auto b = lit('b');
    auto c = lit('c');
   
    x3::rule<class r> r;
    BOOST_SPIRIT_DEFINE(r, *(a | b | c));
   
    x3::rule<class r2> r2;
    BOOST_SPIRIT_DEFINE(r2, (a | b) >> (r | b));
   
    x3::rule<class r3> r3;
    BOOST_SPIRIT_DEFINE(r3, *(a | b | c));
   
    x3::rule<class r4> r4;
    BOOST_SPIRIT_DEFINE(r4, (a | b) >> (r4 | b));
   
    x3::rule<class a_r> a_r;
    x3::rule<class b_r> b_r;
    x3::rule<class c_r> c_r;

    BOOST_SPIRIT_DEFINE(a_r, lit('a'));
    BOOST_SPIRIT_DEFINE(b_r, lit('b'));
    BOOST_SPIRIT_DEFINE(c_r, lit('c'));
    
    x3::rule<class r5> r5;
    auto const r_def5 = (a | b) >> (r5 | c);
    BOOST_SPIRIT_DEFINE(r5, r_def5);
}

int
main()
{
    using spirit_test::test_attr;
    using spirit_test::test;

    using namespace boost::spirit::x3::ascii;
    using boost::spirit::x3::rule;
    using boost::spirit::x3::int_;
    using boost::spirit::x3::lit;
    using boost::spirit::x3::unused_type;
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::skip_flag;
    
    using namespace g;

    { // basic tests

        {
            BOOST_TEST(test("abcabcacb", r));
        }

        {
            BOOST_TEST(test("aaaabababaaabbb", r2));
            BOOST_TEST(test("aaaabababaaabba", r2, false));

            // ignore the skipper!
            BOOST_TEST(test("aaaabababaaabba", r2, space, false));
        }
    }

    { // basic tests w/ skipper

        {
            BOOST_TEST(test(" a b c a b c a c b ", r3, space));
        }

        {
            BOOST_TEST(test(" a a a a b a b a b a a a b b b ", r4, space));
            BOOST_TEST(test(" a a a a b a b a b a a a b b a ", r4, space, false));
        }
    }

    { // basic tests w/ skipper but no final post-skip

        {
            auto start = *(a_r | b_r) >> c_r;

            char const *s1 = " a b a a b b a c ... "
              , *const e1 = s1 + std::strlen(s1);
            BOOST_TEST(phrase_parse(s1, e1, start, space, skip_flag::dont_post_skip)
              && s1 == e1 - 5);

        }

        {
            {
                char const *s1 = " a a a a b a b a b a a a b b b c "
                  , *const e1 = s1 + std::strlen(s1);
                BOOST_TEST(phrase_parse(s1, e1, r5, space, skip_flag::post_skip)
                  && s1 == e1);
            }
            
            {
                char const *s1 = " a a a a b a b a b a a a b b b c "
                  , *const e1 = s1 + std::strlen(s1);
                BOOST_TEST(phrase_parse(s1, e1, r5, space, skip_flag::dont_post_skip)
                  && s1 == e1 - 1);
            }
        }
    }

    return boost::report_errors();
}

