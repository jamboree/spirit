/*//////////////////////////////////////////////////////////////////////////////
    Copyright (c) 2014 Jamboree

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////*/
#ifndef BOOST_SPIRIT_X3_SUPPORT_UTILITY_INTEGER_SEQUENCE_HPP_INCLUDED
#define BOOST_SPIRIT_X3_SUPPORT_UTILITY_INTEGER_SEQUENCE_HPP_INCLUDED


#if defined(_MSC_VER)
#pragma once
#endif

#include <cstddef>


namespace boost { namespace spirit { namespace x3
{
	template<class T, T... ns>
	struct integer_sequence
    {};
}}}

namespace boost { namespace spirit { namespace x3 { namespace detail
{
	template<class T>
	struct integer_sequence_builder
	{
		template<class S1, class S2>
		struct concat;

		template<T... n1, T... n2>
		struct concat<integer_sequence<T, n1...>, integer_sequence<T, n2...>>
		{
			typedef integer_sequence<T, n1..., n2...> type;
		};

		template<T n, class S>
		struct offset;

		template<T n, T... ns>
		struct offset<n, integer_sequence<T, ns...>>
		{
			typedef integer_sequence<T, (n + ns)...> type;
		};

		template<T n, class = void>
		struct make
		{
			static T const m = n / 2;
			typedef typename make<m>::type part1;
			typedef typename make<n - m>::type part2;
			typedef typename
                concat<part1, typename offset<m, part2>::type>::type
            type;
		};

		template<class Dummy>
		struct make<0, Dummy>
		{
			typedef integer_sequence<T> type;
		};
		
		template<class Dummy>
		struct make<1, Dummy>
		{
			typedef integer_sequence<T, 0> type;
		};
	};
}}}}

namespace boost { namespace spirit { namespace x3
{
	template<std::size_t... ns>
	using index_sequence = integer_sequence<std::size_t, ns...>;

	template<class T, T n>
	using make_integer_sequence =
		typename detail::integer_sequence_builder<T>::
			template make<n>::type;

	template<std::size_t n>
	using make_index_sequence = make_integer_sequence<std::size_t, n>;

	template<class... T>
	using index_sequence_for = make_index_sequence<sizeof...(T)>;
}}}


#endif
