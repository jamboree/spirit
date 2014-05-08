/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_RULE_JAN_08_2012_0326PM)
#define BOOST_SPIRIT_X3_RULE_JAN_08_2012_0326PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/nonterminal/detail/rule.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/spirit/home/x3/support/context.hpp>

#if !defined(BOOST_SPIRIT_X3_NO_RTTI)
#include <typeinfo>
#endif

namespace boost { namespace spirit { namespace x3
{
    template <typename ID, typename Attribute = unused_type>
    struct rule;

    template <typename ID>
    struct identity {};

    struct rule_context_tag;

    template <typename ID>
    struct rule_context_with_id_tag;

    template <typename Attribute>
    struct rule_context
    {
        Attribute& val() const
        {
            BOOST_ASSERT(attr_ptr);
            return *attr_ptr;
        }

        Attribute* attr_ptr;
    };

    template <typename Context>
    inline auto
    _val(Context const& context)
    -> decltype(x3::get<rule_context_tag>(context).val())
    {
        return x3::get<rule_context_tag>(context).val();
    }


#define BOOST_SPIRIT_DEFINE(id, def)                                                \
     template <typename Attribute, typename Iterator, typename Context              \
         , typename ActualAttribute, typename AttributePtr>                         \
     inline bool parse_rule(                                                        \
         boost::spirit::x3::rule<class id, Attribute> const& r                      \
       , Iterator& first, Iterator const& last                                      \
       , Context const& context, ActualAttribute& attr                              \
       , AttributePtr*& attr_ptr)                                                   \
   {                                                                                \
         return boost::spirit::x3::detail::parse_rule<Attribute, class id>          \
             ::call_rule_definition(                                                \
                 def, #id, first, last, context                                     \
               , attr, attr_ptr                                                     \
               , boost::mpl::true_());                                              \
   }                                                                                \
   /***/

    template <typename ID, typename Attribute>
    struct rule : parser<rule<ID, Attribute>>
    {
        typedef ID id;
        typedef Attribute attribute_type;
        static bool const has_attribute =
            !is_same<Attribute, unused_type>::value;
        static bool const handles_container =
            traits::is_container<Attribute>::value;

        template <typename Iterator, typename Context, typename Attribute_, typename AttributeContext>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr, AttributeContext& attr_ctx) const
        {
            // This is called when a rule-context has already been established.
            // In such case, we will not set up a rule-context again.
            return parse_rule(*this, first, last, context, attr, attr_ctx.attr_ptr);
        }
        
        template <typename Iterator, typename Context, typename Attribute_>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr, unused_type) const
        {
            // This is called when a rule-context has *not yet* been established.
            // This happens at the start of a potentially recursive rule invocation.
            // We will set up a new rule-context here.
            rule_context<Attribute> r_context = { 0 };

            auto rule_ctx1 = make_context<rule_context_with_id_tag<ID>>(r_context, context);
            auto rule_ctx2 = make_context<rule_context_tag>(r_context, rule_ctx1);
            auto this_context = make_context<ID>(*this, rule_ctx2);
            return parse_rule(*this, first, last, this_context, attr, r_context.attr_ptr);
        }
        
        template <typename Iterator, typename Context, typename Attribute_>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr) const
        {
            return parse(first, last, context, attr, x3::get<rule_context_with_id_tag<ID>>(context));
        }
    };
    
    namespace traits
    {
        template <typename T, typename Enable = void>
        struct is_rule : mpl::false_ {};
    
        template <typename ID, typename Attribute>
        struct is_rule<rule<ID, Attribute>> : mpl::true_ {};
    }

    template <typename T>
    struct get_info<T, typename traits::is_rule<T>::type>
    {
        typedef std::string result_type;
        std::string operator()(T const& r) const
        {
            return r.name;
        }
    };
}}}

#endif
