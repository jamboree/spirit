/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_REAL_APRIL_18_2006_0850AM)
#define BOOST_SPIRIT_X3_REAL_APRIL_18_2006_0850AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/literal.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/numeric/real_policies.hpp>
#include <boost/spirit/home/x3/support/numeric_utils/extract_real.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename T, typename RealPolicies = real_policies<T> >
    struct real_parser : parser<real_parser<T, RealPolicies> >
    {
        typedef T attribute_type;
        static bool const has_attribute = true;
        static bool const caller_is_pass_through_unary = true;
        typedef extract_real<T, RealPolicies> extract;
        
        real_parser()
        	: policies() {}

        real_parser(RealPolicies const& policies)
        	: policies(policies) {}

        template <typename Iterator, typename Context>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, T& attr_) const
        {
            x3::skip_over(first, last, context);
            return extract::parse(first, last, attr_, policies);
        }

        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr_param) const
        {
            // this case is called when Attribute is not T
            T attr_;
            if (parse(first, last, context, attr_))
            {
                traits::move_to(attr_, attr_param);
                return true;
            }
            return false;
        }
        
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context& context, Attribute& attr, T val) const
        {
            x3::skip_over(first, last, context);
            Iterator it(first);
            T attr_;
            if (extract::parse(it, last, attr_, policies) && attr_ == val)
            {
                x3::traits::move_to(attr_, attr);
                first = it;
                return true;
            }
            return false;
        }
        
        RealPolicies policies;
    };
    
    namespace extension
    {
        template <typename T>
        struct literal<T, typename enable_if<is_floating_point<T>>::type>
        {
            typedef real_parser<T> type;
        };
    }
    
    typedef real_parser<float> float_type;                                    
    float_type const float_{};    
                                       
    typedef real_parser<double> double_type;                                    
    double_type const double_{};                                       

}}}

#endif
