/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_ACTOR_HPP_INCLUDED
#define BOOST_SPIRIT_X3_ACTOR_HPP_INCLUDED


#include <tuple>
#include <boost/phoenix/core.hpp>
#include <boost/fusion/include/at.hpp>


namespace boost { namespace spirit { namespace x3
{
    struct rule_context_tag;
    struct rule_args_tag;
    struct locals_tag;
}}}

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    struct full_attr_eval
    {
        template <typename Env>
        auto eval(Env const& env)->(fusion::at_c<1>(env.args())) const
        {
            return fusion::at_c<1>(env.args());
        }
    };
    
    template <int N>
    struct attr_eval
    {
        template <typename Env>
        auto eval(Env const& env)->(fusion::at_c<N>(fusion::at_c<1>(env.args()))) const
        {
            return fusion::at_c<N>(fusion::at_c<1>(env.args()));
        }
    };
    
    template <class Tag, int N>
    struct ctx_eval
    {
        template <typename Env>
        auto eval(Env const& env)->(std::get<N>(x3::get<Tag>(fusion::at_c<0>(env.args())))) const
        {
            return std::get<N>(x3::get<Tag>(fusion::at_c<0>(env.args())));
        }
    };
    
    struct rule_ctx_eval
    {
        template <typename Env>
        auto eval(Env const& env)->(x3::get<rule_context_tag>(fusion::at_c<0>(env.args())).val()) const
        {
            return x3::get<rule_context_tag>(fusion::at_c<0>(env.args())).val();
        }
    };
}}}}

namespace boost { namespace spirit { namespace x3
{
    template <typename Eval>
    struct actor : Eval {};
    
    template <typename Eval>
    using make_actor = phoenix::actor<actor<Eval>>;
    
    template <int N>
    using attribute_actor = make_actor<detail::attr_eval<N>>; 

    template <int N>
    using argument_actor = make_actor<detail::ctx_eval<rule_args_tag, N>>;
    
    template <int N>
    using local_actor = make_actor<detail::ctx_eval<locals_tag, N>>;
    
    // _0 refers to the whole attribute as generated by the lhs parser
    typedef make_actor<detail::full_attr_eval> _0_type;
    
    // _1, _2, ... refer to the attributes of the single components the lhs
    // parser is composed of
    typedef attribute_actor<0> _1_type;
    typedef attribute_actor<1> _2_type;
    typedef attribute_actor<2> _3_type;
    
    // _val refers to the 'return' value of a rule (same as _r0)
    typedef make_actor<detail::rule_ctx_eval> _val_type;
    typedef make_actor<detail::rule_ctx_eval> _r0_type;
    
    // _r1, _r2, ... refer to the rule arguments
    typedef argument_actor<0> _r1_type;
    typedef argument_actor<1> _r2_type;
        
    // _a, _b, ... refer to the local variables of a rule
    typedef local_actor<0> _a_type;
    typedef local_actor<1> _b_type;
    typedef local_actor<2> _c_type;
    typedef local_actor<3> _d_type;
    typedef local_actor<4> _e_type;
    typedef local_actor<5> _f_type;
    typedef local_actor<6> _g_type;
    typedef local_actor<7> _h_type;
    typedef local_actor<8> _i_type;
    typedef local_actor<9> _j_type;
}}}

BOOST_PHOENIX_DEFINE_CUSTOM_TERMINAL(
    template <typename Eval>
  , boost::spirit::x3::actor
  , mpl::false_                 // is not nullary
  , v2_eval(
        proto::make<
            boost::spirit::x3::actor<Eval>()
        >
      , proto::call<
            functional::env(proto::_state)
        >
    )
)


#endif

