#pragma once
#include <algorithm>

template<typename _InputIter, typename _UnaryPred>
_InputIter findIf(_InputIter beg, _InputIter end, _UnaryPred pred) {
	while (beg != end) {
		if (pred(*beg)) {
			return beg;
		}
		++beg;
	}
	return end;
}

template <class _ForwardIter>
_ForwardIter minElement(_ForwardIter beg, _ForwardIter end) {
	if (beg == end) {
		return end;
	}
	_ForwardIter smallest = beg;
	while (++beg != end) {
		if (*beg < *smallest) {
			smallest = beg;
		}
	}	
	return smallest;
}

template <class _ForwardIter>
_ForwardIter maxElement(_ForwardIter beg, _ForwardIter end) {
	if (beg == end) {
		return end;
	}
	_ForwardIter largest = beg;
	while (++beg != end) {
		if (*beg > *largest) {
			largest = beg;
		}
	}
	return largest;
}

template<class _InputIter, class _Func>
void forEach(_InputIter beg, _InputIter end, _Func op) {
	while (beg != end) {
		op(*beg);
		++beg;
	}
}

template <class _BidirectionalIter>
void sort(_BidirectionalIter beg, _BidirectionalIter end) {
	if (end <= beg) {
		return;
	}
	_BidirectionalIter pivot = beg, middle = beg + 1;
	for (_BidirectionalIter i = middle; i < end; ++i) {
		if (*i < *pivot) {
			std::iter_swap(i, middle);
			++middle;
		}
	}
	std::iter_swap(beg, middle - 1);
	sort(beg, middle - 1);
	sort(middle, end);
}

template <class _InputIter, class _OutputIter, class _UnaryPred>
_OutputIter copyIf(_InputIter source_beg, _InputIter source_end, _OutputIter dest_beg, _UnaryPred pred) {
	while (source_beg != source_end) {
		if (pred(*source_beg)) {
			*dest_beg = *source_beg;
			++dest_beg;
		}
		++source_beg;
	}
	return dest_beg;
}
