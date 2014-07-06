/*=============================================================================
    Copyright (c) 2009  Hartmut Kaiser
    Copyright (c) 2014  Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(SPIRIT_X3_BOOL_SEP_29_2009_0709AM)
#define SPIRIT_X3_BOOL_SEP_29_2009_0709AM

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/caller.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/numeric/bool_policies.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename T, typename BoolPolicies = bool_policies<T>>
    struct bool_parser : parser<bool_parser<T, BoolPolicies>>
    {
        typedef T attribute_type;
        static bool const has_attribute = true;

        bool_parser()
        	: policies() {}

        bool_parser(BoolPolicies const& policies)
        	: policies(policies) {}

        template <typename Iterator, typename Context>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, T& attr) const
        {
            x3::skip_over(first, last, context);
            return policies.parse_true(first, last, attr)
                || policies.parse_false(first, last, attr);
        }

        // literal
        template <typename Iterator, typename Context, typename U>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, T& attr, U val) const
        {
            x3::skip_over(first, last, context);
            return val?
                policies.parse_true(first, last, attr)
              : policies.parse_false(first, last, attr);
        }

        template <typename Iterator, typename Context, typename Attribute, typename... Param>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr, Param... param) const
        {
            // this case is called when Attribute is not T
            T attr_;
            if (parse(first, last, context, attr_, param...))
            {
                traits::move_to(attr_, attr);
                return true;
            }
            return false;
        }
                
        BoolPolicies policies;
    };

    typedef bool_parser<bool> bool_type;
    bool_type const bool_{};

    typedef caller<bool_type, mpl::true_> true_type;
    true_type const true_{{}, mpl::true_{}};

    typedef caller<bool_type, mpl::false_> false_type;
    false_type const false_{{}, mpl::false_{}};
}}}

#endif
