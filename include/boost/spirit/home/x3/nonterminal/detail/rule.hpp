/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_X3_DETAIL_RULE_JAN_08_2012_0326PM)
#define BOOST_SPIRIT_X3_DETAIL_RULE_JAN_08_2012_0326PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/subcontext.hpp>
#include <boost/spirit/home/x3/support/traits/make_attribute.hpp>
#include <boost/spirit/home/x3/auxiliary/guard.hpp>
#include <boost/spirit/home/x3/nonterminal/detail/check_args.hpp>
#include <boost/spirit/home/x3/nonterminal/detail/transform_attribute.hpp>
#include <boost/utility/addressof.hpp>

#if defined(BOOST_SPIRIT_X3_DEBUG)
#include <boost/spirit/home/x3/nonterminal/simple_trace.hpp>
#endif

namespace boost { namespace spirit { namespace x3
{
    template <typename ID>
    struct identity;
    
    template <typename... IDs>
    struct filter;
    
    struct parse_pass_context_tag;
    
    template <typename Iterator>
    struct expectation_failure;
}}}

namespace boost { namespace spirit { namespace x3 { namespace detail
{
#if defined(BOOST_SPIRIT_X3_DEBUG)
    template <typename Iterator, typename Attribute>
    struct context_debug
    {
        context_debug(
            char const* rule_name
          , Iterator const& first, Iterator const& last
          , Attribute const& attr)
          : fail(true), rule_name(rule_name)
          , first(first), last(last)
          , attr(attr)
          , f(detail::get_simple_trace())
        {
            f(first, last, attr, pre_parse, rule_name);
        }

        ~context_debug()
        {
            auto status = fail ? failed_parse : successful_parse;
            f(first, last, attr, status, rule_name);
        }

        bool fail;
        char const* rule_name;
        Iterator const& first;
        Iterator const& last;
        Attribute const& attr;
        detail::simple_trace_type& f;
    };
#endif

    struct no_exception_handler {};

    template <typename ID, typename Iterator, typename Exception, typename Context>
    inline no_exception_handler
    on_error(ID, Iterator&, Exception const&, Context const&)
    {
        return no_exception_handler();
    }

    template <typename ID, typename Iterator, typename Attribute, typename Context>
    inline void
    on_success(ID, Iterator const&, Iterator const&, Attribute&, Context const&)
    {
        // no-op
    }

    template <typename ID>
    struct make_id
    {
        typedef identity<ID> type;
    };

    template <typename ID>
    struct make_id<identity<ID>>
    {
        typedef identity<ID> type;
    };
    
    template <typename Filter>
    struct filter_subcontext;
    
    template <typename... IDs>
    struct filter_subcontext<filter<IDs...>>
    {
        template <typename Context>
        static auto apply(Context const& context)
        {
            return make_subcontext<IDs...>(x3::get<IDs>(context)...);
        }
    };
    
    template <typename Iterator, typename Context>
    void preskip_rule(mpl::false_, Iterator&, Iterator const&, Context const&)
    {}

    template <typename Iterator, typename Context>
    void preskip_rule(mpl::true_
      , Iterator& first, Iterator const& last, Context const& context)
    {
        x3::skip_over(first, last, context);
    }
    
    template <typename Rule, typename Iterator, typename Context, typename Attribute, typename... Ts>
    bool parse_rule_main(Rule const& r, Iterator& first, Iterator const& last
      , Context const& context, Attribute& attr, Ts&&... ts)
    {
        typedef typename Rule::filter_type filter_type;
        typedef typename Rule::attribute_type attribute_type;
        typedef typename Rule::params_type params_type;
        
        static_assert(
            check_args<params_type, Ts...>::value
          , "args/params not matched");
          
        typedef traits::make_attribute<attribute_type, Attribute> make_attribute;

        // do down-stream transformation, provides attribute for
        // rhs parser
        typedef traits::transform_attribute<
            typename make_attribute::type, attribute_type, parser_id>
        transform;

        typedef typename make_attribute::value_type value_type;
        typedef typename transform::type transform_attr;
        value_type made_attr = make_attribute::call(attr);
        transform_attr attr_ = transform::pre(made_attr);
        params_type params(std::forward<Ts>(ts)...);
        auto const subcontext(filter_subcontext<filter_type>::apply(context));

#if defined(BOOST_SPIRIT_X3_DEBUG)
        context_debug<Iterator, typename make_attribute::value_type>
            dbg(r.name, first, last, made_attr);
#endif
        mpl::bool_<has_skipper<Context>::value
            && !has_skipper<decltype(subcontext)>::value> needs_preskip;
        preskip_rule(needs_preskip, first, last, context);
        // parse_rule is found by ADL
        if (parse_rule(r, first, last, subcontext, attr_, params))
        {
            // do up-stream transformation, this integrates the results
            // back into the original attribute value, if appropriate
            traits::post_transform(attr, attr_);

#if defined(BOOST_SPIRIT_X3_DEBUG)
            dbg.fail = false;
#endif
            return true;
        }
        return false;
    }
    
    template <typename ID>
    struct parse_rule
    {
        template <typename RHS, typename Iterator, typename Context, typename ActualAttribute>
        static bool parse_rhs_main(
            RHS const& rhs
          , Iterator& first, Iterator const& last
          , Context const& context, ActualAttribute& attr, mpl::true_)
        {
            Iterator i = first;
            bool r = rhs.parse(i, last, context, attr);
            if (r)
            {
                bool pass = true;
                auto action_context = make_context<parse_pass_context_tag>(pass, context);
                on_success(
                    typename make_id<ID>::type()
                  , first
                  , i
                  , attr
                  , action_context
                );
                if (pass)
                    first = i;
            }
            return r;
        }

        template <typename RHS, typename Iterator, typename Context, typename ActualAttribute>
        static bool parse_rhs_main(
            RHS const& rhs
          , Iterator& first, Iterator const& last
          , Context const& context, ActualAttribute& attr, mpl::false_)
        {
            for (;;)
            {
                try
                {
                    return parse_rhs_main(rhs, first, last, context, attr, mpl::true_());
                }
                catch (expectation_failure<Iterator> const& x)
                {
                    switch (on_error(typename make_id<ID>::type(), first, x, context))
                    {
                        case error_handler_result::fail:
                            return false;
                        case error_handler_result::retry:
                            continue;
                        case error_handler_result::accept:
                            return true;
                        case error_handler_result::rethrow:
                            throw;
                    }
                }
            }
        }

        template <typename RHS, typename Iterator, typename Context, typename ActualAttribute>
        static bool parse_rhs_main(
            RHS const& rhs
          , Iterator& first, Iterator const& last
          , Context const& context, ActualAttribute& attr)
        {
            typedef
                decltype(
                    on_error(
                        typename make_id<ID>::type()
                      , first
                      , boost::declval<expectation_failure<Iterator> const&>()
                      , context
                    )
                )
            on_error_result;

            return parse_rhs_main(
                rhs, first, last, context, attr
              , is_same<on_error_result, no_exception_handler>()
            );
        }

        template <typename RHS, typename Iterator, typename Context, typename ActualAttribute>
        static bool parse_rhs(
            RHS const& rhs
          , Iterator& first, Iterator const& last
          , Context const& context, ActualAttribute& attr
          , mpl::false_)
        {
            return parse_rhs_main(rhs, first, last, context, attr);
        }

        template <typename RHS, typename Iterator, typename Context, typename ActualAttribute>
        static bool parse_rhs(
            RHS const& rhs
          , Iterator& first, Iterator const& last
          , Context const& context, ActualAttribute&
          , mpl::true_)
        {
            return parse_rhs_main(rhs, first, last, context, unused);
        }
    };
}}}}

#endif
