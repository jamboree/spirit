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
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/cat.hpp>

#if !defined(BOOST_SPIRIT_X3_NO_RTTI)
#include <typeinfo>
#endif

namespace boost { namespace spirit { namespace x3
{
    template <typename ID>
    struct identity {};

    struct rule_context_tag;

    template <typename Attribute, typename Params>
    struct rule_context
    {
        Attribute& val() const
        {
            return *attr_ptr;
        }

        Params& params() const
        {
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
          : rhs(std::move(rhs)), name(name) {}

        template <typename Iterator, typename Context, typename Attribute, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            return detail::parse_rule_main(*this, first, last, context, attr
                , std::forward<Ts>(ts)...);
        }
          
        template <typename Iterator, typename Skipper>
        friend bool parse_rule(rule_definition const& r
          , Iterator& first, Iterator const& last
          , attribute_type& attr, params_type& params, Skipper const& skipper)
        {
            rule_context<attribute_type, params_type>
                r_context{boost::addressof(attr), &params};
            auto context(make_context<rule_context_tag>(r_context));
            
            return detail::parse_rule<id>::parse_rhs(r.rhs, first, last
              , make_context<skipper_tag>(skipper, context), attr
              , mpl::bool_<(RHS::has_action && !explicit_attribute_propagation_)>());
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

        template <typename Iterator, typename Context, typename Attribute_, typename... Ts>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute_& attr, Ts&&... ts) const
        {
            return detail::parse_rule_main(*this, first, last, context, attr
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
    
#define BOOST_SPIRIT_RULEDEF(n, i)                                              \
    BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(_rule_def, n), _), i)
    /***/
#define BOOST_SPIRIT_DEFINE_(r, data, def)                                      \
    auto const BOOST_SPIRIT_RULEDEF(__LINE__, r)(def);                          \
    template <typename Iterator, typename Attribute                             \
        , typename Params, typename Skipper>                                    \
    inline bool parse_rule(                                                     \
        decltype(BOOST_SPIRIT_RULEDEF(__LINE__, r))::lhs_type const& r_         \
      , Iterator& first, Iterator const& last                                   \
      , Attribute& attr, Params& params, Skipper const& skipper)                \
    {                                                                           \
        return parse_rule(BOOST_SPIRIT_RULEDEF(__LINE__, r)                     \
            , first, last, attr, params, skipper);                              \
    }
    /***/
#define BOOST_SPIRIT_DEFINE(...) BOOST_PP_SEQ_FOR_EACH(                         \
    BOOST_SPIRIT_DEFINE_, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
    /***/
#define BOOST_SPIRIT_DECLARE_(r, data, rule)                                    \
    template <typename Iterator, typename Attribute                             \
        , typename Params, typename Skipper>                                    \
    bool parse_rule(rule const&, Iterator&, Iterator const&                     \
        , Attribute&, Params&, Skipper const&);
    /***/
#define BOOST_SPIRIT_DECLARE(...) BOOST_PP_SEQ_FOR_EACH(                        \
    BOOST_SPIRIT_DECLARE_, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
    /***/
#define BOOST_SPIRIT_INSTANTIATE(iterator, ...)                                 \
    template bool parse_rule<iterator                                           \
        , decltype(BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__))::attribute_type      \
        , decltype(BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__))::params_type         \
        , decltype(BOOST_PP_VARIADIC_ELEM(1, __VA_ARGS__                        \
            , ::boost::spirit::x3::unused))>(                                   \
            decltype(BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__)) const&             \
          , iterator&, iterator const&                                          \
          , decltype(BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__))::attribute_type&   \
          , decltype(BOOST_PP_VARIADIC_ELEM(0, __VA_ARGS__))::params_type&      \
          , decltype(BOOST_PP_VARIADIC_ELEM(1, __VA_ARGS__                      \
            , ::boost::spirit::x3::unused)) const&);
    /***/
}}}

#endif
