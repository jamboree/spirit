/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_TOKEN_HPP_INCLUDED
#define BOOST_SPIRIT_X3_TOKEN_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/nonterminal/rule.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/metafunctions.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/cat.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct token_tag;

    struct tok
    {
        template <typename Context>
        auto operator()(Context const& context) const
        {
            return x3::get<token_tag>(context);
        }
    };

    struct tokstr
    {
        template <typename Context>
        auto operator()(Context const& context) const
        {
            auto const& range = x3::get<token_tag>(context);
            typedef typename
                range_value<unrefcv_t<decltype(range)>>::type
            value_type;
            return std::basic_string<value_type>(range.begin(), range.end());
        }
    };
    
    tok const _tok{};
    tokstr const _tokstr{};
}}}

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename T>
    T type_from_list(std::initializer_list<T> const&);
}}}}


#define BOOST_SPIRIT_TOKEN_CASE_FILTER0(name, def)                              \
    (name) BOOST_SPIRIT_TOKEN_CASE_FILTER1
    /***/
#define BOOST_SPIRIT_TOKEN_CASE_FILTER1(name, def)                              \
    (name) BOOST_SPIRIT_TOKEN_CASE_FILTER0
    /***/
#define BOOST_SPIRIT_TOKEN_CASE_FILTER0_END
#define BOOST_SPIRIT_TOKEN_CASE_FILTER1_END
    /***/
#define BOOST_SPIRIT_TOKEN_DEF_FILTER0(name, def)                               \
    (def) BOOST_SPIRIT_TOKEN_DEF_FILTER1
    /***/
#define BOOST_SPIRIT_TOKEN_DEF_FILTER1(name, def)                               \
    (def) BOOST_SPIRIT_TOKEN_DEF_FILTER0
    /***/
#define BOOST_SPIRIT_TOKEN_DEF_FILTER0_END
#define BOOST_SPIRIT_TOKEN_DEF_FILTER1_END
    /***/
#define BOOST_SPIRIT_CASEDEF(rule, i)                                           \
    BOOST_PP_CAT(BOOST_PP_CAT(_case_def_, rule), i)
    /***/
#define BOOST_SPIRIT_TOKEN_DEF(r, rule, i, def)                                 \
    auto const BOOST_SPIRIT_CASEDEF(rule, i)(rule %= def);
    /***/
#define BOOST_SPIRIT_TOKEN_CASE(r, rule, i, c)                                  \
    case c:                                                                     \
        if (x3::detail::parse_def(BOOST_SPIRIT_CASEDEF(rule, i)                 \
          , it, last, tok_context, attr, ::boost::mpl::false_())) break;        \
        return false;
    /***/
#define BOOST_SPIRIT_DEFINE_TOKEN_(rule, tokenizer, cases, defs)                \
    BOOST_PP_SEQ_FOR_EACH_I(BOOST_SPIRIT_TOKEN_DEF, rule , defs)                \
    template <typename Iterator, typename Context>                              \
    bool parse_rule(decltype(rule) const&                                       \
      , Iterator& first, Iterator const& last, Context const& context           \
      , decltype(rule)::attribute_type& attr                                    \
      , decltype(rule)::params_type& params)                                    \
    {                                                                           \
        namespace x3 = ::boost::spirit::x3;                                     \
        typedef decltype(tokenizer) tokenizer_type;                             \
        static_assert(                                                          \
            x3::traits::is_parser<tokenizer_type>::value                        \
          , "invalid tokenizer");                                               \
        typedef decltype(rule) rule_type;                                       \
        typedef rule_type::attribute_type attribute_type;                       \
        typedef rule_type::params_type params_type;                             \
        x3::skip_over(first, last, context);                                    \
        decltype(x3::detail::type_from_list({BOOST_PP_SEQ_ENUM(cases)})) val{}; \
        x3::rule_context<attribute_type, params_type>                           \
            r_context{::boost::addressof(attr), &params};                       \
        auto ctx(x3::make_context<x3::rule_context_tag>(r_context, context));   \
        Iterator it(first);                                                     \
        if (tokenizer.parse(it, last, ctx, val))                                \
        {                                                                       \
            auto tok_range(::boost::make_iterator_range(first, it));            \
            auto tok_context(x3::make_context<x3::token_tag>(tok_range, ctx));  \
            switch (val)                                                        \
            {                                                                   \
            BOOST_PP_SEQ_FOR_EACH_I(BOOST_SPIRIT_TOKEN_CASE, rule , cases)      \
            default: return false;                                              \
            }                                                                   \
            first = it;                                                         \
            return true;                                                        \
        }                                                                       \
        return false;                                                           \
    }
    /***/
#define BOOST_SPIRIT_DEFINE_TOKEN(rule, tokenizer, seq)                         \
    BOOST_SPIRIT_DEFINE_TOKEN_(rule, tokenizer,                                 \
        BOOST_PP_CAT(BOOST_SPIRIT_TOKEN_CASE_FILTER0 seq, _END)                 \
      , BOOST_PP_CAT(BOOST_SPIRIT_TOKEN_DEF_FILTER0 seq, _END))
    /***/

#endif
