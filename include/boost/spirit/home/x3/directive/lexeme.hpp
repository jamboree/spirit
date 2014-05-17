/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_LEXEME_MARCH_24_2007_0802AM)
#define SPIRIT_LEXEME_MARCH_24_2007_0802AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/directive.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace spirit { namespace x3
{
    struct lexeme_directive : directive<lexeme_directive>
    {
        static bool const is_pass_through_unary = true;

        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename... Ts>
        typename enable_if<has_skipper<Context>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            x3::skip_over(first, last, context);
            auto const& skipper = x3::get<skipper_tag>(context);

            typedef unused_skipper<
                typename remove_reference<decltype(skipper)>::type>
            unused_skipper_type;
            unused_skipper_type unused_skipper(skipper);

            return parse_impl(subject, first, last
              , make_context<skipper_tag>(unused_skipper, context)
              , attr, std::forward<Ts>(ts)...);
        }

        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename... Ts>
        typename disable_if<has_skipper<Context>, bool>::type
        parse(Subject const& subject, Iterator& first, Iterator const& last
          , Context const& context, Attribute& attr, Ts&&... ts) const
        {
            //  no need to pre-skip if skipper is unused
            return parse_impl(subject, first, last, context, attr
                , std::forward<Ts>(ts)...);
        }

        // lexeme[p]
        template <typename Subject, typename Iterator, typename Context, typename Attribute>
        bool parse_impl(Subject const& subject, Iterator& first
          , Iterator const& last, Context const& context, Attribute& attr) const
        {
            return subject.parse(first, last, context, attr);
        }

        template <typename Lex>
        static typename extension::as_parser<Lex>::value_type
        transform_params(Lex const& lex)
        {
            return as_parser(lex);
        }

        // lexeme(lex)[p]
        template <typename Subject, typename Iterator, typename Context, typename Attribute, typename Lex>
        bool parse_impl(Subject const& subject, Iterator& first
          , Iterator const& last, Context const& context
          , Attribute& attr, Lex const& lex) const
        {
            static_assert(traits::is_parser<Lex>::value, "invalid lex");
            
            Iterator right(first);
            if (lex.parse(right, last, context, attr))
            {
                Iterator left(first);
                if (subject.parse(left, right, context, attr) && left == right)
                {
                    first = right;
                    return true;
                }
            }
            return false;
        }
    };

    lexeme_directive const lexeme{};
}}}

#endif
