/*=============================================================================
    Copyright (arg) 2001-2013 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_ACTION_JANUARY_07_2007_1128AM)
#define SPIRIT_ACTION_JANUARY_07_2007_1128AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/traits/make_attribute.hpp>
#include <boost/spirit/home/x3/support/utility/is_callable.hpp>
#include <boost/spirit/home/x3/nonterminal/detail/transform_attribute.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mem_fn.hpp>

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Action, typename Context, typename Attribute>
    using action_arity = typename mpl::eval_if<
        is_callable<Action(Context, Attribute)>, mpl::int_<2>
      , mpl::eval_if<is_callable<Action(Attribute)>, mpl::int_<1>
          , mpl::if_<is_callable<Action()>, mpl::int_<0>, mpl::int_<-1>>>>::type;

    template <typename Action, typename Enable = void>
    struct wrap_action
    {
        typedef Action type;

        static Action&& apply(Action& f)
        {
            return std::move(f);
        }
    };
    
    template <typename Action>
    struct wrap_action<Action
        , typename enable_if<is_member_pointer<Action>>::type>
    {
        typedef decltype(mem_fn(declval<Action>())) type;

        static type apply(Action f)
        {
            return mem_fn(f);
        }
    };
}}}}

namespace boost { namespace spirit { namespace x3
{
    struct parse_pass_context_tag;
    struct raw_attribute_type;
    
    template <typename Context>
    inline bool& _pass(Context const& context)
    {
        return x3::get<parse_pass_context_tag>(context);
    }

    template <typename Subject, typename Action>
    struct action : unary_parser<Subject, action<Subject, Action>>
    {
        typedef unary_parser<Subject, action<Subject, Action>> base_type;
        typedef detail::wrap_action<Action> wrap_action;
        typedef typename wrap_action::type action_type;
        static bool const is_pass_through_unary = true;
        static bool const has_action = true;

        action(Subject const& subject, Action f)
          : base_type(subject), f(wrap_action::apply(f)) {}

        template <typename Context, typename Attribute>
        bool call_action(mpl::int_<2> // arity
            , Context const& context, Attribute& attr) const
        {
            bool pass = true;
            auto action_context =
                make_context<parse_pass_context_tag>(pass, context);
            f(action_context, attr); // pass in the context and attribute
            return pass;
        }

        template <typename Context, typename Attribute>
        bool call_action(mpl::int_<1> // arity
            , Context const& /*context*/, Attribute& attr) const
        {
            f(attr); // pass attribute only
            return true;
        }

        template <typename Context, typename Attribute>
        bool call_action(mpl::int_<0> // arity
            , Context const& /*context*/, Attribute& /*attr*/) const
        {
            f(); // pass attribute only
            return true;
        }
        
        // action wants attribute
        template <int N, typename Iterator, typename Context, typename Attribute>
        typename enable_if_c<(N >= 0), bool>::type
        parse_impl(mpl::int_<N> arity, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            Iterator save = first;
            if (this->subject.parse(first, last, context, attr))
            {
                if (call_action(arity, context, attr))
                    return true;

                // reset iterators if semantic action failed the match
                // retrospectively
                first = save;
            }
            return false;
        }
        
        // attr==raw_attribute_type, action wants iterator_range (see raw.hpp)
        template <int N, typename Iterator, typename Context>
        typename enable_if_c<(N > 0), bool>::type
        parse_impl(mpl::int_<N> arity, Iterator& first, Iterator const& last
          , Context const& context, raw_attribute_type const&) const
        {
            boost::iterator_range<Iterator> rng;
            return parse_impl(arity, first, last, context, rng);
        }

        // attr==unused, action wants attribute
        template <int N, typename Iterator, typename Context>
        typename enable_if_c<(N > 0), bool>::type
        parse_impl(mpl::int_<N> arity, Iterator& first, Iterator const& last
          , Context const& context, unused_type) const
        {
            typedef typename
                traits::attribute_of<Subject, Context>::type
            attribute_type;
            typedef
                traits::make_attribute<attribute_type, unused_type>
            make_attribute;
            typedef traits::transform_attribute<
                typename make_attribute::type, attribute_type, parser_id>
            transform;

            // synthesize the attribute since one is not supplied
            typename make_attribute::type made_attr =
                make_attribute::call(unused_type());
            typename transform::type attr = transform::pre(made_attr);
            return parse_impl(arity, first, last, context, attr);
        }

        // main parse function
        template <typename Iterator, typename Context, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr) const
        {
            typedef typename
                traits::make_attribute<
                    typename traits::attribute_of<Subject, Context>::type
                  , typename remove_const<Attribute>::type
                >::value_type&
            attribute_type;
            
            using arity = detail::action_arity<action_type const
              , x3::context<parse_pass_context_tag, bool, Context> const&
              , attribute_type>;

            static_assert(arity::value != -1, "invalid action");
            return parse_impl(arity(), first, last, context, attr);
        }

        action_type f;
    };

    template <typename P, typename Action>
    inline action<typename extension::as_parser<P>::value_type, Action>
    operator/(P const& p, Action f)
    {
        return {as_parser(p), f};
    }
}}}

#endif
