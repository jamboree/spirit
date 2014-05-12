/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_POS_ITERATOR_HPP_INCLUDED
#define BOOST_SPIRIT_X3_POS_ITERATOR_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/iterator/iterator_adaptor.hpp>


namespace boost { namespace spirit { namespace x3
{
    template <typename Iterator>
    struct pos_iterator
      : boost::iterator_adaptor
        <
            pos_iterator<Iterator>          // Derived
          , Iterator                        // Base
          , boost::use_default              // Value
          , boost::forward_traversal_tag    // CategoryOrTraversal
        >
    {
        pos_iterator()
          : pos_iterator::iterator_adaptor_(), _row(), _beg()
        {}

        pos_iterator(Iterator const& base)
          : pos_iterator::iterator_adaptor_(base), _row(), _beg(base)
        {}

        std::size_t row() const
        {
            return _row;
        }

        std::size_t col() const
        {
            return std::distance(_beg, this->base());
        }

        Iterator line_begin() const
        {
            return _beg;
        }

        Iterator line_end(Iterator const& end) const
        {
            typename pos_iterator::value_type c;
            for (Iterator it(_beg); it != end; ++it)
            {
                c = *it;
                if (c == '\r' || c == '\n')
                    return it;
            }
            return end;
        }

        operator Iterator() const
        {
            return this->base();
        }

        void newline()
        {
            ++_row;
            _beg = this->base();
        }

    private:

        std::size_t _row;
        Iterator _beg;
    };
    
    struct newl_parser : parser<newl_parser>
    {
        typedef unused_type attribute_type;
        static bool const has_attribute = false;
        static bool const caller_is_pass_through_unary = true;

        // newl
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
            , Context const& context, Attribute& /*attr*/) const
        {
            x3::skip_over(first, last, context);
            Iterator it(first);
            bool matched = false;
            if (it != last && *it == '\r')  // CR
            {
                matched = true;
                ++it;
            }
            if (it != last && *it == '\n')  // LF
            {
                matched = true;
                ++it;
            }
            if (matched)
            {
                it.newline();
                first = it;
            }
            return matched;
        }
        
        template <typename Parser>
        static typename extension::as_parser<Parser>::value_type
        transform_params(Parser const& p)
        {
            return as_parser(p);
        }
        
        // newl(p)
        template <typename Iterator, typename Context, typename Attribute, typename Parser>
        bool parse(Iterator& first, Iterator const& last
            , Context const& context, Attribute& attr, Parser const& p) const
        {
            static_assert(traits::is_parser<Parser>::value, "invalid parser");
            
            if (p.parse(first, last, context, attr))
            {
                first.newline();
                return true;
            }
            return false;
        }
    };
    
    newl_parser const newl{};
}}}


#endif

