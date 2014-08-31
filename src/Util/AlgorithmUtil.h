/*
 * AlgorithmUtil.h
 *
 *  Created on: 2014/06/22
 *      Author: kryozahiro
 */

#ifndef ALGORITHMUTIL_H_
#define ALGORITHMUTIL_H_

#include <algorithm>
#include <random>

namespace AlgorithmUtil {

//[first, last)からcount個を選んで先頭に配置し、残りを後ろへ回す
//残った要素の先頭へのイテレータを返す
template <class RandomIt, class URNG>
void partial_shuffle(RandomIt first, RandomIt middle, RandomIt last, URNG&& g) {
	typedef typename std::iterator_traits<RandomIt>::difference_type diff_t;
	typedef typename std::make_unsigned<diff_t>::type udiff_t;
	typedef typename std::uniform_int_distribution<udiff_t> distr_t;
	typedef typename distr_t::param_type param_t;

	distr_t D;
	diff_t n = last - first - 1;
	while (first != middle) {
		std::iter_swap(first, first + D(g, param_t(0, n)));
		--n;
		++first;
	}
}

//組み合わせの実装
namespace detail {
template <class BidirectionalIterator>
bool next_combination(BidirectionalIterator first1, BidirectionalIterator last1,
		BidirectionalIterator first2, BidirectionalIterator last2) {
	if ((first1 == last1) || (first2 == last2)) {
		return false;
	}
	BidirectionalIterator m1 = last1;
	BidirectionalIterator m2 = last2; --m2;
	while (--m1 != first1 && !(*m1 < *m2)) {
	}
	bool result = (m1 == first1) && !(*first1 < *m2);
	if (!result) {
		while (first2 != m2 && !(*m1 < *first2)) {
			++first2;
		}
		first1 = m1;
		std::iter_swap(first1 , first2);
		++first1;
		++first2;
	}
	if ((first1 != last1) && (first2 != last2)) {
		m1 = last1;
		m2 = first2;
		while ((m1 != first1) && (m2 != last2)) {
			std::iter_swap(--m1, m2);
			++m2;
		}
		std::reverse (first1 , m1);
		std::reverse (first1 , last1);
		std::reverse (m2, last2);
		std::reverse (first2 , last2);
	}
	return !result;
}
}

//次の組み合わせを返す
template <class BidirectionalIterator>
inline bool next_combination(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
	return detail::next_combination(first, middle, middle, last);
}

//前の組み合わせを返す
template <class BidirectionalIterator>
inline bool prev_combination(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
	return detail::next_combination(middle, last, first, middle);
}

}

#endif /* ALGORITHMUTIL_H_ */
