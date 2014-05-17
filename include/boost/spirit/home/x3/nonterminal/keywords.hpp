/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_KEYWORDS_HPP_INCLUDED
#define BOOST_SPIRIT_X3_KEYWORDS_HPP_INCLUDED

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/string/tst.hpp>
#include <boost/spirit/home/x3/nonterminal/detail/rule.hpp>
#include <boost/spirit/home/x3/support/traits/string_traits.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/cat.hpp>


namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template<class Char>
    tst<Char, int> make_kwd_tst(std::initializer_list<Char*> const& list)
    {
        tst<Char, int> table;
        int i = 0;
        for (auto s : list)
            table.add(traits::get_string_begin<Char>(s)
              , traits::get_string_end<Char>(s), i++);
        return std::move(table);
    }
}}}}


#define BOOST_SPIRIT_KEYWORDS_NAME_FILTER0(name, def)                           \
    (name) BOOST_SPIRIT_KEYWORDS_NAME_FILTER1
    /***/
#define BOOST_SPIRIT_KEYWORDS_NAME_FILTER1(name, def)                           \
    (name) BOOST_SPIRIT_KEYWORDS_NAME_FILTER0
    /***/
#define BOOST_SPIRIT_KEYWORDS_NAME_FILTER0_END
#define BOOST_SPIRIT_KEYWORDS_NAME_FILTER1_END
    /***/
#define BOOST_SPIRIT_KEYWORDS_DEF_FILTER0(name, def)                            \
    (def) BOOST_SPIRIT_KEYWORDS_DEF_FILTER1
    /***/
#define BOOST_SPIRIT_KEYWORDS_DEF_FILTER1(name, def)                            \
    (def) BOOST_SPIRIT_KEYWORDS_DEF_FILTER0
    /***/
#define BOOST_SPIRIT_KEYWORDS_DEF_FILTER0_END
#define BOOST_SPIRIT_KEYWORDS_DEF_FILTER1_END
    /***/
#define BOOST_SPIRIT_KWDDEF(rule, i)                                            \
    BOOST_PP_CAT(BOOST_PP_CAT(_kwd_def_, rule), i)
    /***/
#define BOOST_SPIRIT_KEYWORDS_DEF(r, rule, i, def)                              \
    auto const BOOST_SPIRIT_KWDDEF(rule, i)                                     \
        (::boost::spirit::x3::as_parser(def));
    /***/
#define BOOST_SPIRIT_KEYWORDS_CASE(z, i, rule)                                  \
    case i:                                                                     \
        if (!x3::detail::parse_rule<id>::parse_rhs_main(                        \
            BOOST_SPIRIT_KWDDEF(rule, i), right, last, ctx, attr)) return false;\
        break;
    /***/
#define BOOST_SPIRIT_DEFINE_KEYWORDS_(rule, lex, names, defs)                   \
    BOOST_PP_SEQ_FOR_EACH_I(BOOST_SPIRIT_KEYWORDS_DEF, rule , defs)             \
    BOOST_SPIRIT_KEYWORDS_DEF(_, rule, _lex, lex)                               \
    template <typename Iterator, typename Context>                              \
    bool parse_rule(decltype(rule) const&                                       \
      , Iterator& first, Iterator const& last, Context const& context           \
      , decltype(rule)::attribute_type& attr                                    \
      , decltype(rule)::params_type& params)                                    \
    {                                                                           \
        namespace x3 = ::boost::spirit::x3;                                     \
        typedef decltype(rule) rule_type;                                       \
        typedef rule_type::attribute_type attribute_type;                       \
        typedef rule_type::params_type params_type;                             \
        typedef rule_type::id id;                                               \
        static auto const table(                                                \
            x3::detail::make_kwd_tst({BOOST_PP_SEQ_ENUM(names)}));              \
        x3::skip_over(first, last, context);                                    \
        Iterator right(first);                                                  \
        auto const& skipper = x3::get<x3::skipper_tag>(context);                \
        typedef x3::unused_skipper<                                             \
            typename ::boost::remove_reference<decltype(skipper)>::type>        \
        unused_skipper_type;                                                    \
        unused_skipper_type no_skipper(skipper);                                \
        if (BOOST_SPIRIT_KWDDEF(rule, _lex).parse(right, last                   \
          , x3::make_context<x3::skipper_tag>(no_skipper, context), x3::unused))\
        {                                                                       \
            Iterator left(first);                                               \
            auto p = table.find(left, right);                                   \
            if (p && left == right)                                             \
            {                                                                   \
                x3::rule_context<attribute_type, params_type>                   \
                    r_context{::boost::addressof(attr), &params};               \
                auto ctx(x3::make_context<x3::rule_context_tag>(                \
                    r_context, context));                                       \
                switch (*p)                                                     \
                {                                                               \
                    BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(names)                    \
                      , BOOST_SPIRIT_KEYWORDS_CASE, rule)                       \
                }                                                               \
                first = right;                                                  \
                return true;                                                    \
            }                                                                   \
        }                                                                       \
        return false;                                                           \
    }
    /***/
#define BOOST_SPIRIT_DEFINE_KEYWORDS(rule, lex, seq)                            \
    BOOST_SPIRIT_DEFINE_KEYWORDS_(rule, lex,                                    \
        BOOST_PP_CAT(BOOST_SPIRIT_KEYWORDS_NAME_FILTER0 seq, _END)              \
      , BOOST_PP_CAT(BOOST_SPIRIT_KEYWORDS_DEF_FILTER0 seq, _END))
    /***/


#endif

