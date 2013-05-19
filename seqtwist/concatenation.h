#pragma once

#include "range.h"
#include<iterator>
#include<type_traits>

namespace seqtwist {

	/*
	template<class T>
	typename std::add_rvalue_reference<T>::type declval ();

	template<class T> struct StringConst;

	template<class T, std::size_t S>
	struct StringConst<T [S]> {
		typedef const T * iterator;
		typedef const T * const_iterator;
		typedef typename std::iterator_traits<iterator>::difference_type difference_type;

		const T * str;
		StringConst (const T str_ []) : str (str_) {
		}

		iterator begin () const { return str; }
		iterator end () const { return str + S - 1; }

	};
	*/



	/*
	template<class T>
	auto as_range (T t [])
	-> typename range_traits<T []>::range_type {
		return make_range (t);
	}
	*/

	// ****************************************************************************************************
	// iterator class for concatenation
	// ****************************************************************************************************
	/*
	template<class R1, class R2>
	struct ConcatenatingIterator {

	public:
		typedef typename iterator_for<R1>::type iterator1;
		typedef typename iterator_for<R2>::type iterator2;

		typedef std::random_access_iterator_tag iterator_category;
		typedef typename std::iterator_traits<iterator1>::difference_type difference_type;
		typedef typename std::iterator_traits<iterator1>::value_type value_type;
		typedef typename std::iterator_traits<iterator1>::pointer pointer;
		typedef typename std::iterator_traits<iterator1>::reference reference;

	public:

		ConcatenatingIterator (
			const R1 & left, const R2 & right,
			const difference_type & pos = 0
		)
		: m_left (left), m_right (right)
		, m_position (pos)
		, m_leftSize (size (left)), m_rightSize (size (right)) {
		}

		//const difference_type position () { return m_position; }
		reference operator * () const {
			using namespace std;
			return (m_position < m_leftSize) ? * (begin (m_left) + m_position) : * (begin (m_right) + (m_position - m_leftSize));
		}
		reference operator -> () const {
			return *(*this);
		}

		reference operator [] (difference_type n) { return * (*this + n); }

		ConcatenatingIterator & operator += (difference_type diff) { m_position += diff; return *this; }
		ConcatenatingIterator operator + (difference_type diff) const { auto tmp = *this; tmp += diff; return tmp; }
		ConcatenatingIterator & operator ++ () { ++m_position; return *this; }
		ConcatenatingIterator operator ++ (int) { auto tmp = *this; ++m_position; return tmp; }
		ConcatenatingIterator & operator -= (difference_type diff) { m_position -= diff; return *this; }
		ConcatenatingIterator operator - (difference_type diff) const { auto tmp = *this; tmp -= diff; return tmp; }
		difference_type operator - (const ConcatenatingIterator<R1,R2> & iter2) { return m_position - iter2.m_position; }
		ConcatenatingIterator & operator -- () { --m_position; return *this; }
		ConcatenatingIterator operator -- (int) { auto tmp = *this; --m_position; return tmp; }
			
		bool operator == (const ConcatenatingIterator & iter2) const {
			return ( (m_left == iter2.m_left) && (m_right == iter2.m_right) && (m_position == iter2.m_position) );
		}
		bool operator != (const ConcatenatingIterator & iter2) const { return !(*this == iter2); }

			
			
		friend ConcatenatingIterator operator + (difference_type diff, const ConcatenatingIterator<R1,R2> & iter) { return iter + diff; }
						
		friend bool operator < (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position < iter2.m_position;
		}

			
		friend bool operator > (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position > iter2.m_position;
		}

			
		friend bool operator <= (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position <= iter2.m_position;
		}

			
		friend bool operator >= (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position >= iter2.m_position;
		}

		template<class It>
		friend void copy (
			const ConcatenatingIterator<R1,R2> inBeg,
			const ConcatenatingIterator<R1,R2> inEnd,
			It outIter
		);

	private:
		const R1 m_left;
		const R2 m_right;

		// these have to be defined *after* m_(left|right)(Begin|End) !!!
		difference_type m_position;
		const difference_type m_leftSize;
		const difference_type m_rightSize;


	};
	*/

	template<class R1, class R2> struct ConcatenatingIterator;

	// ****************************************************************************************************
	// Concatenation class
	// ****************************************************************************************************
	template<class R1, class R2>
	class Concatenation {

	public:
		typedef typename iterator_for<const R1>::type left_iterator;
		typedef typename iterator_for<const R2>::type right_iterator;

		typedef ConcatenatingIterator<Range<typename iterator_for<const R1>::type>, Range<typename iterator_for<const R2>::type> > iterator;
		typedef typename std::iterator_traits<iterator>::difference_type size_type;
//		typedef typename std::iterator_traits<iterator>::reference reference;
//		typedef typename T1::value_type value_type;

		typedef typename std::iterator_traits<left_iterator>::difference_type difference_type;
		typedef typename std::iterator_traits<left_iterator>::value_type value_type;
		typedef typename std::iterator_traits<left_iterator>::pointer pointer;
		typedef typename std::iterator_traits<left_iterator>::reference reference;

	private:
		static const Range<iterator> generate_concatenation_range (const R1 & left, const R2 & right, const size_type size) {
			auto b = make_concatenating_iterator (left, right, 0);
			auto e = b + size;
			return Range<iterator> (b, e);
		}


	public:
		Concatenation (R1 && left, R2 && right)
		: m_left (std::forward<R1> (left))
		, m_right (std::forward<R2> (right))
		, m_leftRange (make_readonly_range (m_left))
		, m_rightRange (make_readonly_range (m_right))
		, m_leftSize (size (m_leftRange))
		, m_rightSize (size (m_rightRange))
		, m_size (m_leftSize + m_rightSize)
		, m_concatenation_range (generate_concatenation_range (m_left, m_right, m_size)) {
		}

	public:

		iterator begin () const { return begin (m_concatenation_range); }
		iterator end () const { return end (m_concatenation_range); }
		const Range<iterator> range () const { return m_concatenation_range; }

		const R1 & left () const { return m_left; }
		const R2 & right () const { return m_right; }

		const reference operator [] (size_type pos) const {
			return (pos < m_leftSize) ? * (m_leftRange.begin () + pos) : * (m_rightRange.begin () + (pos - m_leftSize));
		}

	private:
		const R1 m_left;
		const R2 m_right;

		// these must be defined after m_left, m_right
		const Range<typename iterator_for<R1>::type> m_leftRange;
		const Range<typename iterator_for<R2>::type> m_rightRange;

		// this must be defined after m_left, m_right
		size_type m_leftSize;
		size_type m_rightSize;

		// must be defined after m_leftSize, m_rightSize
		size_type m_size;

		// this must be defined after m_left, m_right, m_size !!!
		const Range<iterator> m_concatenation_range;
	};

	template<class R1, class R2, class F>
	void visitComponents (const Concatenation<R1,R2> & c, F & f) {
		using namespace std;
		f (begin (c.left ()), end (c.left ()));
		f (begin (c.right ()), end (c.right ()));
	}

	template<class R, class F>
	void visitComponents (const R & r, F & f) {
		using namespace std;
		f (begin (r), end (r));
	}


	template<class R1, class R2>
	Concatenation<
		Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
		Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
	>
	make_concatenation (R1 && left, R2 && right) {
		return Concatenation<
			Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
			Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
		> (
			make_readonly_range ( std::forward<const R1> (left) ),
			make_readonly_range ( std::forward<const R2> (right) )
		);
	}


	template<class R1, class R2>
	struct ConcatenatingIterator {

	public:

		typedef std::random_access_iterator_tag iterator_category;
		typedef typename Concatenation<R1,R2>::difference_type difference_type;
		typedef typename Concatenation<R1,R2>::value_type value_type;
		typedef typename Concatenation<R1,R2>::pointer pointer;
		typedef typename Concatenation<R1,R2>::reference reference;

	public:

		ConcatenatingIterator (
			const Concatenation<R1,R2> concatenation,
			const difference_type & pos = 0
		)
		: m_concatenation (concatenation)
		, m_position (pos) {
		}

		//const difference_type position () { return m_position; }
		reference operator * () const { return m_concatenation [m_position]; }
		pointer operator -> () const {
			return & (m_concatenation [m_position]);
		}

		reference operator [] (difference_type n) { return * (*this + n); }

		ConcatenatingIterator & operator += (difference_type diff) { m_position += diff; return *this; }
		ConcatenatingIterator operator + (difference_type diff) const { auto tmp = *this; tmp += diff; return tmp; }
		ConcatenatingIterator & operator ++ () { ++m_position; return *this; }
		ConcatenatingIterator operator ++ (int) { auto tmp = *this; ++m_position; return tmp; }
		ConcatenatingIterator & operator -= (difference_type diff) { m_position -= diff; return *this; }
		ConcatenatingIterator operator - (difference_type diff) const { auto tmp = *this; tmp -= diff; return tmp; }
		difference_type operator - (const ConcatenatingIterator<R1,R2> & iter2) { return m_position - iter2.m_position; }
		ConcatenatingIterator & operator -- () { --m_position; return *this; }
		ConcatenatingIterator operator -- (int) { auto tmp = *this; --m_position; return tmp; }
			
		bool operator == (const ConcatenatingIterator & iter2) const {
			return ( (m_concatenation == iter2.m_concatenation) && (m_position == iter2.m_position) );
		}
		bool operator != (const ConcatenatingIterator & iter2) const { return !(*this == iter2); }

			
			
		friend ConcatenatingIterator operator + (difference_type diff, const ConcatenatingIterator<R1,R2> & iter) { return iter + diff; }
						
		friend bool operator < (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position < iter2.m_position;
		}

			
		friend bool operator > (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position > iter2.m_position;
		}

			
		friend bool operator <= (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position <= iter2.m_position;
		}

			
		friend bool operator >= (const ConcatenatingIterator<R1,R2> & iter1, const ConcatenatingIterator<R1,R2> & iter2) {
			return iter1.m_position >= iter2.m_position;
		}

		template<class It>
		friend void copy (
			const ConcatenatingIterator<R1,R2> inBeg,
			const ConcatenatingIterator<R1,R2> inEnd,
			It outIter
		);

	private:
		const Concatenation<R1,R2> & m_concatenation;

		// these have to be defined *after* m_(left|right)(Begin|End) !!!
		difference_type m_position;

	};

	/*
	template<class T1_, class T2_>
	ConcatenatingIterator<Range<T1_>,Range<T2_>> make_concatenating_iterator (
		const T1_ & leftBegin, const T1_ & leftEnd, const T2_ & rightBegin, const T2_ & rightEnd
	) {
		return ConcatenatingIterator<Range<T1_>,Range<T2_>> (make_range (leftBegin, leftEnd), make_range (rightBegin, rightEnd), 0);
	}

	template<class T1_, class T2_, class D>
	ConcatenatingIterator<Range<T1_>,Range<T2_>> make_concatenating_iterator (
		const T1_ & leftBegin, const T1_ & leftEnd, const T2_ & rightBegin, const T2_ & rightEnd, const D pos
	) {
		return ConcatenatingIterator<Range<T1_>,Range<T2_>> (make_range (leftBegin, leftEnd), make_range (rightBegin, rightEnd), pos);
	}

	template<class R1, class R2>
	ConcatenatingIterator<
		Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
		Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
	>
	make_concatenating_iterator (
		const R1 & left, const R2 & right
	) {
		return ConcatenatingIterator<
			Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
			Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
		> (make_readonly_range (left), make_readonly_range (right), 0);
	}


	template<class R1, class R2, class D>
	ConcatenatingIterator<
		Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
		Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
	>
	make_concatenating_iterator (
		const R1 & left, const R2 & right, const D pos
	) {
		return ConcatenatingIterator<
			Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
			Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
		> (make_readonly_range (left), make_readonly_range (right), pos);
	}
	*/

	template<class T1_, class T2_>
	ConcatenatingIterator<Range<T1_>,Range<T2_>> make_concatenating_iterator (
		const T1_ & leftBegin, const T1_ & leftEnd, const T2_ & rightBegin, const T2_ & rightEnd
	) {
		return ConcatenatingIterator<Range<T1_>,Range<T2_>> (make_concatenation (make_range (leftBegin, leftEnd), make_range (rightBegin, rightEnd)), 0);
	}

	template<class T1_, class T2_, class D>
	ConcatenatingIterator<Range<T1_>,Range<T2_>> make_concatenating_iterator (
		const T1_ & leftBegin, const T1_ & leftEnd, const T2_ & rightBegin, const T2_ & rightEnd, const D pos
	) {
		return ConcatenatingIterator<Range<T1_>,Range<T2_>> (make_concatenation (make_range (leftBegin, leftEnd), make_range (rightBegin, rightEnd)), pos);
	}

	template<class R1, class R2>
	ConcatenatingIterator<
		Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
		Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
	>
	make_concatenating_iterator (
		const R1 & left, const R2 & right
	) {
		return ConcatenatingIterator<
			Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
			Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
		> (make_concatenation (make_readonly_range (left), make_readonly_range (right)), 0);
	}


	template<class R1, class R2, class D>
	ConcatenatingIterator<
		Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
		Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
	>
	make_concatenating_iterator (
		const R1 & left, const R2 & right, const D pos
	) {
		return ConcatenatingIterator<
			Range<typename iterator_for<const typename std::remove_reference<R1>::type>::type>,
			Range<typename iterator_for<const typename std::remove_reference<R2>::type>::type>
		> (make_concatenation (make_readonly_range (left), make_readonly_range (right)), pos);
	}


}