#pragma once

#include <iterator>

namespace seqtwist {

    // *** iterator_for trait

	template<class T>                struct iterator_for               { typedef typename T::iterator type; };
	template<class T>                struct iterator_for <const T>     { typedef typename T::const_iterator type; };
	template<class T>                struct iterator_for <T *>         { typedef T * type; };
	template<class T>                struct iterator_for <T * const>   { typedef T * type; };
	template<class T, std::size_t S> struct iterator_for <T [S]>       { typedef T * type; };
	template<class T, std::size_t S> struct iterator_for <const T [S]> { typedef const T * type; };



	// *** non-member function size () for range-like objects (i.e. objects to which non-member begin and end can be applied)

	template<class T>
	auto size (T & range)
	-> typename std::iterator_traits<typename iterator_for<T>::type>::difference_type {
		using namespace std;
		return distance (begin (range), end (range));
	}


	template<class T> const T && as_const (T && t) { return std::forward<T> (t); }


}