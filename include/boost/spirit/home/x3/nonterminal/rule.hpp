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
#include <boost/spirit/home/x3/nonterminal/detail/decompose_attribute.hpp>
#include <boost/spirit/home/x3/nonterminal/detail/check_args.hpp>
#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#if !defined(BOOST_SPIRIT_X3_NO_RTTI)
#include <typeinfo>
#endif

namespace boost { namespace spirit { namespace x3
{
    template <typename ID>
    struct identity {};

    struct rule_context_tag;

    template <typename ID>
    struct rule_context_with_id_tag;

    template <typename Attribute, typename Params>
    struct rule_context
    {
        rule_context(Attribute* attr_ptr = nullptr, Params* params_ptr = nullptr)
          : attr_ptr(attr_ptr), params_ptr(params_ptr) {}

        Attribute& val() const
        {
            BOOST_ASSERT(attr_ptr);
            return *attr_ptr;
        }

        Params& params() const
        {
            BOOST_ASSERT(params_ptr);
            return *params_ptr;
        }
        
        Attribute* attr_ptr;
        Params* params_ptr;
    };

    template <typename Context>
    inline auto _val(Context const& context)
    -> decltype(x3::get<rule_context_tag>(context).val())
    {
        return x3::get<rule_context_tag>(context).val();
    }
    
    template <typename Context>
    inline auto _params(Context const& context)
    -> decltype(x3::get<rule_context_tag>(context).params())
    {
        return x3::get<rule_context_tag>(context).params();
    }

    template <typename LHS, typename RHS, bool explicit_attribute_propagation_>
    struct rule_definition : parser<rule_definition<LHS, RHS, explicit_attribute_propagation_>>
    {
        typedef rule_definition<LHS, RHS, explicit_attribute_propagation_> this_type;
        typedef LHS lhs_type;
        typedef RHS rhs_type;
        typedef typename LHS::id id;
        typedef typename LHS::attribute_type attribute_type;
        typedef typename LHS::params_type params_type;
        static bool const has_attribute = LHS::has_attribute;
        static bool const handles_container = LHS::handles_container;
        static bool const caller_is_pass_through_unary = true;
        static bool const explicit_attribute_propagation =
            explicit_attribute_propagation_;

        rule_definition(RHS rhs, char const* name)
          : rhs(rhs), name(name) {}

        template <typename Iterator, typename Context, typename Attribute_, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr, Ts&&... ts) const
        {
            static_assert(
                detail::check_args<params_type, Ts...>::value
              , "args/params not matched");
            
            params_type params(std::forward<Ts>(ts)...);
            rule_context<attribute_type, params_type> r_context;
            auto rule_ctx1 = make_context<rule_context_with_id_tag<id>>(r_context, context);
            auto rule_ctx2 = make_context<rule_context_tag>(r_context, rule_ctx1);
            auto this_context = make_context<id>(*this, rule_ctx2);

            return detail::parse_rule<attribute_type, params_type, id>
                ::call_rule_definition(
                    rhs, name, first, last, this_context
                  , attr, params, r_context.attr_ptr, r_context.params_ptr
                  , mpl::bool_<explicit_attribute_propagation>());
        }

        RHS rhs;
        char const* name;
    };

    template <typename ID, typename Attribute = unused_type>
    struct rule : parser<rule<ID, Attribute>>
    {
        typedef ID id;
        typedef detail::decompose_attribute<Attribute> decompose_attr;
        typedef typename decompose_attr::result_type attribute_type;
        typedef typename decompose_attr::params_type params_type;
        static bool const has_attribute =
            !is_same<attribute_type, unused_type>::value;
        static bool const handles_container =
            traits::is_container<attribute_type>::value;
        static bool const caller_is_pass_through_unary = true;

#if !defined(BOOST_SPIRIT_X3_NO_RTTI)
        rule(char const* name = typeid(rule).name()) : name(name) {}
#else
        rule(char const* name = "unnamed") : name(name) {}
#endif

        template <typename RHS>
        rule_definition<rule, typename extension::as_parser<RHS>::value_type, false>
        operator=(RHS const& rhs) const
        {
            return {as_parser(rhs), name};
        }

        template <typename RHS>
        rule_definition<rule, typename extension::as_parser<RHS>::value_type, true>
        operator%=(RHS const& rhs) const
        {
            return {as_parser(rhs), name};
        }

        template <typename Iterator, typename Context
            , typename Attribute_, typename... Ts>
        friend inline typename enable_if_c<!is_same<typename Context::template
            get_result<mpl::identity<ID>>::type, unused_type>::value, bool>::type
        parse_rule(rule const& r, Iterator& first, Iterator const& last
            , Context const& context, Attribute_& attr, Ts&&... ts)
        {
            return detail::parse_rule<attribute_type, params_type, ID>
                ::call_from_rule(
                    x3::get<ID>(context), r.name
                  , first, last, context, attr
                  , x3::get<rule_context_with_id_tag<ID>>(context)
                  , std::forward<Ts>(ts)...);
        }

        template <typename Iterator, typename Context, typename Attribute_, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr, Ts&&... ts) const
        {
            return parse_rule(*this, first, last, context, attr
                , std::forward<Ts>(ts)...);
        }

        char const* name;
    };
    
    namespace traits
    {
        template <typename T, typename Enable = void>
        struct is_rule : mpl::false_ {};
    
        template <typename ID, typename Attribute>
        struct is_rule<rule<ID, Attribute>> : mpl::true_ {};
        
        template <typename LHS, typename RHS, bool explicit_attribute_propagation>
        struct is_rule<rule_definition<LHS, RHS, explicit_attribute_propagation>>
          : mpl::true_ {};
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
    
#define BOOST_SPIRIT_DEFINES_(r, data, def)                                     \
    template <typename Iterator, typename Context                               \
        , typename Attribute_, typename... Ts>                                  \
    inline bool parse_rule(typename decltype(def)::lhs_type const& r__          \
        , Iterator& first__, Iterator const& last__, Context const& context__   \
        , Attribute_& attr__, Ts&&... ts__)                                     \
    {                                                                           \
        typedef decltype(def) rule_def_t;                                       \
        typedef typename rule_def_t::attribute_type attribute_type;             \
        typedef typename rule_def_t::params_type params_type;                   \
        typedef typename rule_def_t::id id;                                     \
        static auto rule_def(def);                                              \
        return ::boost::spirit::x3::detail::                                    \
            parse_rule<attribute_type, params_type, id>::call_from_rule(        \
                rule_def, r__.name                                              \
              , first__, last__, context__, attr__                              \
              , ::boost::spirit::x3::get<::boost::spirit::x3::                  \
                    rule_context_with_id_tag<id>>(context__)                    \
              , std::forward<Ts>(ts__)...);                                     \
    }
    /***/
#define BOOST_SPIRIT_DEFINES(...) BOOST_PP_SEQ_FOR_EACH(                        \
    BOOST_SPIRIT_DEFINES_, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
    /***/
}}}

#endif
