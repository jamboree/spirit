/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "expression.hpp"
#include "error_handler.hpp"

namespace client
{
    typedef std::string::const_iterator iterator_type;

    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////////
    //  The calculator grammar
    ///////////////////////////////////////////////////////////////////////////////
    namespace calculator_grammar
    {
        using x3::uint_;
        using x3::char_;

        x3::rule<class term, ast::expression> const term("term");
        x3::rule<class factor, ast::operand> const factor("factor");

        BOOST_SPIRIT_DEFINE
        (
            expression =
                term
                >> *(   (char_('+') > term)
                    |   (char_('-') > term)
                    )
          , term =
                factor
                >> *(   (char_('*') > factor)
                    |   (char_('/') > factor)
                    )
          , factor =
                    uint_
                |   "(" > expression > ')'
                |   (char_('-') > factor)
                |   (char_('+') > factor)
        )

        BOOST_SPIRIT_INSTANTIATE(iterator_type, expression
          , x3::subcontext<x3::skipper_tag(x3::ascii::space_type const&)>)
    }
}
