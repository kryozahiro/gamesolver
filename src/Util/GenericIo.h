/*
 * GenericIo.h
 *
 *  Created on: 2014/08/04
 *      Author: kryozahiro
 */

#ifndef GENERICIO_H_
#define GENERICIO_H_

#include <algorithm>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <boost/spirit/home/support/container.hpp>
#include <boost/lexical_cast.hpp>

namespace GenericIo {

namespace detail {

//スコープ内で空白をスキップする
//特にlexical_castに必要
template<class CharT>
class ScopedSkip {
public:
	ScopedSkip(std::basic_istream<CharT>& is) : is(is), skip(is.flags() & std::ios_base::skipws) {
		if (!skip) {
			is >> std::skipws;
		}
	}
	~ScopedSkip() {
		if (!skip) {
			is >> std::noskipws;
		}
	}
private:
	std::basic_istream<CharT>& is;
	bool skip;
};

}

//std::pair
template <class T, class U>
std::istream& operator>>(std::istream& is, std::pair<T, U>& rhs) {
	detail::ScopedSkip<char> skip(is);
	char dummy;
	is >> dummy;	//'['
	is >> rhs.first;
	is >> dummy;	//','
	is >> rhs.second;
	is >> dummy;	//']'
	return is;
}

template <class T, class U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& rhs) {
	os << '(' << rhs.first << ", " << rhs.second << ')';
	return os;
}

//std::tuple
template <std::size_t I = 0, class... Types>
typename std::enable_if<I == sizeof...(Types) - 1, void>::type
read(std::istream& is, std::tuple<Types...>& t) {
	is >> std::get<I>(t);
}
template <std::size_t I = 0, class... Types>
typename std::enable_if<I < sizeof...(Types) - 1, void>::type
read(std::istream& is, std::tuple<Types...>& t) {
	char dummy;
	is >> std::get<I>(t);
	is >> dummy;	//','
	read<I + 1>(is, t);
}
template <class... Types>
std::istream& operator>>(std::istream& is, std::tuple<Types...>& rhs) {
	detail::ScopedSkip<char> skip(is);
	char dummy;
	is >> dummy;	//'('
	read(is, rhs);
	is >> dummy;	//')'
	return is;
}

template <std::size_t I = 0, class... Types>
typename std::enable_if<I == sizeof...(Types) - 1, void>::type
write(std::ostream& os, const std::tuple<Types...>& t) {
	os << std::get<I>(t);
}
template <std::size_t I = 0, class... Types>
typename std::enable_if<I < sizeof...(Types) - 1, void>::type
write(std::ostream& os, const std::tuple<Types...>& t) {
	os << std::get<I>(t) << ", ";
	write<I + 1>(os, t);
}
template <class... Types>
std::ostream& operator<<(std::ostream& os, const std::tuple<Types...>& rhs) {
	os << '(';
	write(os, rhs);
	os << ')';
	return os;
}

//container
template <template <class, class> class C, class T, class A,
	class = typename std::enable_if<boost::spirit::traits::is_container<C<T,A>>::type::value>::type
>
std::istream& operator>>(std::istream& is, C<T, A>& rhs) {
	detail::ScopedSkip<char> skip(is);
	char dummy;
	is >> dummy;	//'['
	is >> dummy;	//readahead
	if (dummy == ']') {
		return is;
	}
	is.unget();
	do {
		T t;
		is >> t;
		rhs.push_back(t);

		is >> dummy;	//','
	} while (dummy != ']');
	return is;
}

template <template <class, class> class C, class T, class A,
	class = typename std::enable_if<boost::spirit::traits::is_container<C<T,A>>::type::value>::type
>
std::ostream& operator<<(std::ostream& os, const C<T, A>& rhs) {
	os << '[';
	typename C<T, A>::const_iterator itr = rhs.cbegin();
	if (itr != rhs.cend()) {
		os << *itr;
		for(++itr; itr != rhs.cend(); ++itr) {
			os << ", " << *itr;
		}
	}
	os << ']';
	return os;
}

}

//ADL対策
//特にlexical_cast出来るようにする
namespace boost {
	using GenericIo::operator>>;
	using GenericIo::operator<<;

	//has_right_shiftの特殊化
	template <class T, class U>
	struct has_right_shift<std::istream, std::pair<T, U>> : public true_type {};

	template <class... Types>
	struct has_right_shift<std::istream, std::tuple<Types...>> : public true_type {};

	template <template <class, class> class C, class T, class A>
	struct has_right_shift<std::istream, C<T, A>> : public true_type {
		typedef typename std::enable_if<boost::spirit::traits::is_container<C<T,A>>::type::value>::type sfinae;
	};

	//has_left_shiftの特殊化
	template <class T, class U>
	struct has_left_shift<std::ostream, std::pair<T, U>> : public true_type {};

	template <class... Types>
	struct has_left_shift<std::ostream, std::tuple<Types...>> : public true_type {};

	template <template <class, class> class C, class T, class A>
	struct has_left_shift<std::ostream, C<T, A>> : public true_type {
		typedef typename std::enable_if<boost::spirit::traits::is_container<C<T,A>>::type::value>::type sfinae;
	};
}

using GenericIo::operator>>;
using GenericIo::operator<<;

#endif /* GENERICIO_H_ */
