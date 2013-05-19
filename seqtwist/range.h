#pragma once

#include "iterator_util.h"
#include <iterator>

namespace seqtwist {

	template<class It>
	class Range {
	public:
		typedef It iterator;
		typedef It const_iterator;
		typedef typename std::iterator_traits<It>::difference_type difference_type;

		Range (const It & b, const It & e) : m_b (b), m_e (e) {
		}

		It begin () const { return m_b; }
		It end () const { return m_e; }

		bool equals_to (const Range<It> & it2) const { return (m_b == it2.m_b) && (m_e == it2.m_e); }

	private:
		It m_b;
		It m_e;
	};

	template<class It>
	bool operator == (const Range<It> & range1, const Range<It> & range2) { return range1.equals_to (range2); }

	template<class It>
	bool operator != (const Range<It> & range1, const Range<It> & range2) { return !range1.equals_to (range2); }


	template<class It>
	auto make_range (const It & b, const It & e)
	-> Range<It> {
		return Range<It> (b, e);
	}

	template<class R>
	auto make_range (R & r)
	-> Range<typename iterator_for<R>::type> {
		using namespace std;
		return make_range (begin (r), end (r));
	}

	template<class R>
	auto make_readonly_range (const R & r)
	-> Range<typename iterator_for<const R>::type> {
		using namespace std;
		return make_range (begin (r), end (r));
	}

	/*
	template<class It>
	typename Range<It>::difference_type size (const Range<It> & range) { using namespace std; return distance (begin (range), end (range)); }
	*/


	template<class T>
	struct can_be_used_like_range { typedef std::false_type type; };

	template<class T>
	struct can_be_used_like_range<Range<T>> { typedef std::true_type type; };

	template<class T>
	struct can_be_used_like_range<std::basic_string<T>> { typedef std::true_type type; };

	/*
	template<class T>
	struct can_be_used_like_range<StringConst<T>> { typedef std::true_type type; };
	*/

	template<class T>
	struct range_traits {
		typedef typename std::remove_reference<T>::type unreferenced_type;
		typedef typename std::remove_const<unreferenced_type>::type naked_type;

		typedef typename std::_If<can_be_used_like_range<naked_type>::type::value, std::false_type, std::true_type>::_Type needs_wrapping;
		typedef typename std::_If<needs_wrapping::value, Range<typename iterator_for<const unreferenced_type>::type>, T>::_Type range_type;
	};



	// *** as_range
	// the no-op version of as_range is supposed preserve const, as well as rvalue / lvalue references

	template<class T>
	auto _as_range (T && t, std::true_type)
	-> decltype ( make_range (std::forward<T> (t)) ) {
		return make_range (std::forward<T> (t));
	}

	template<class T>
	auto _as_range (T && t, std::false_type)
	-> T && {
		return std::forward<T> (t);
	}

	template<class T>
	auto as_range (T && t)
	-> decltype ( _as_range<T> (
			std::forward<T> (t),
			range_traits<T>::needs_wrapping ()
	)) {
		return _as_range<T> (
			std::forward<T> (t),
			range_traits<T>::needs_wrapping ()
		);
	}


	template<class T>
	auto _as_readonly_range (T && t, std::true_type)
	-> decltype ( make_readonly_range (std::forward<T> (t)) ) {
		return make_readonly_range (std::forward<T> (t));
	}

	template<class T>
	auto _as_readonly_range (T && t, std::false_type)
	-> const T && {
		return std::forward<const T> (t);
	}

	template<class T>
	auto as_readonly_range (T && t)
	-> decltype ( _as_readonly_range<T> (
			std::forward<const T> (t),
			range_traits<const T>::needs_wrapping ()
	)) {
		return _as_readonly_range<T> (
			std::forward<const T> (t),
			range_traits<const T>::needs_wrapping ()
		);
	}
}