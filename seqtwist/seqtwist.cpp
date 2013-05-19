// seqtwist.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "WinUnit.h"

#include "concatenation.h"
#include <string>

BEGIN_TEST(ConcatenatingIterator_IndexOperator) {
	using namespace seqtwist;
	using namespace std;

	std::wstring s = L"Seq";
	std::wstring t = L"Twist";
	auto it = make_concatenating_iterator (begin (s), end (s), begin (t), end (t));
	WIN_ASSERT_TRUE ( it [2] == L'q' );
	WIN_ASSERT_TRUE ( it [3] == L'T' );
}
END_TEST

BEGIN_TEST(ConcatenatingIterator_EmptySequence) {
	using namespace seqtwist;
	using namespace std;

	std::wstring s = L"";
	std::wstring t = L"ABC";
	auto it = make_concatenating_iterator (begin (s), end (s), begin (t), end (t));
	WIN_ASSERT_TRUE (*it == L'A');
	WIN_ASSERT_TRUE (* (it + 1) == L'B');
}
END_TEST

BEGIN_TEST(ConcatenatingIterator_Iteration) {
	using namespace seqtwist;
	using namespace std;

	std::wstring s = L"Seq";
	std::wstring t = L"Twist";
	auto b = make_concatenating_iterator (begin (s), end (s), begin (t), end (t));
	auto e = make_concatenating_iterator (begin (s), end (s), begin (t), end (t), 8);

	std::wstring st (b, e);
	WIN_ASSERT_TRUE (st == L"SeqTwist");
}
END_TEST

BEGIN_TEST(ConcatenatingIterator_WithPointers) {
	using namespace seqtwist;
	using namespace std;

	std::wstring s = L"Seq";
	const wchar_t * t = L"Twist";
	auto it = make_concatenating_iterator (begin (s), end (s), t, t + 5);
	WIN_ASSERT_TRUE ( it [2] == L'q' );
	WIN_ASSERT_TRUE ( it [3] == L'T' );
}
END_TEST

template<class T>
bool testForRange (seqtwist::Range<T> * r) { return true; }

template<class T>
bool testForRange (T * r) { return false; }


BEGIN_TEST(ConcatenatingIterator_RangeTraits) {
	using namespace seqtwist;
	WIN_ASSERT_TRUE ( !range_traits<Range<int *>>::needs_wrapping::value );
	WIN_ASSERT_TRUE ( range_traits<int [4]>::needs_wrapping::value );

	WIN_ASSERT_TRUE ( !testForRange ( static_cast<int *> (0) ) );
	WIN_ASSERT_TRUE ( testForRange ( static_cast<range_traits<int [4]>::range_type *> (0) ) );
	WIN_ASSERT_TRUE ( testForRange ( static_cast<range_traits<Range<int *>>::range_type *> (0) ) );
}
END_TEST

template<class T>
bool isRvalueRangeReference (seqtwist::Range<T> && range) { return true; }

template<class T>
bool isRvalueRangeReference (const seqtwist::Range<T> & range) { return false; }

template<class R>
bool asRangeForwardTester (R && r) { using namespace seqtwist; return isRvalueRangeReference (as_range ( std::forward<R> (r) )); }

BEGIN_TEST(ConcatenatingIterator_AsRangeWorksLikeForward) {
	using namespace seqtwist;

	auto r = Range<int *> (0, 0);
	WIN_ASSERT_TRUE ( !asRangeForwardTester (r) );
	WIN_ASSERT_TRUE ( asRangeForwardTester ( (Range<int *> (0,0)) ) );
}
END_TEST

	
BEGIN_TEST(ConcatenatingIterator_WithCharArys) {
	using namespace seqtwist;
	using namespace std;

	const wchar_t s [] = L"Seq";
	const wchar_t t [] = L"Twist";
	std::wstring q (t);
	auto it = make_concatenating_iterator (s, q);

	std::wstring p = L"x";
	WIN_ASSERT_TRUE ( it [2] == 'q' );
	WIN_ASSERT_TRUE ( it [3] == 'T', (p + it [3]).c_str () );
}
END_TEST

BEGIN_TEST(Concatenation) {
using namespace seqtwist;
	using namespace std;

	std::wstring s = L"Seq";
	std::wstring t = L"Twist";
	auto c = make_concatenation (s, t);
	WIN_ASSERT_TRUE ( c [2] == L'q' );
	WIN_ASSERT_TRUE ( c [3] == L'T' );
}
END_TEST