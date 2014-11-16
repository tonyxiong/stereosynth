/* 
 * File:   iterator2d.h
 * Author: xion
 *
 * Created on November 15, 2014, 9:31 PM
 */

#ifndef ITERATOR2D_H
#define	ITERATOR2D_H

namespace pm {

	/**
	 * Iterator over a 2D grid
	 */
	template < typename Index, bool RowMajor = true >
	struct Iterator2D {
		typedef Iterator2D<Index, RowMajor> this_type;
		typedef Index this_value;

		explicit Iterator2D(int x0, int x1, int s0, int e0, int d0, int d1)
		: i0(x0), i1(x1), start(s0), end(e0), dir0(d0), dir1(d1) {
		}

		inline int x0() const {
			return i0;
		}

		inline int x1() const {
			return i1;
		}

		inline this_type & operator ++() {
			i0 += dir0;
			if (i0 == end) {
				i0 = start;
				i1 += dir1;
			}
			return *this;
		}

		inline bool operator==(const this_type &it) const {
			return it.i0 == i0 && it.i1 == i1;
		}

		operator this_value() const {
			return RowMajor ? this_value(i0, i1) : this_value(i1, i0);
		}

		inline this_value operator*() const {
			return this_value(*this);
		}

	private:
		int i0, i1;
		int start, end;
		int dir0, dir1;
	};

	/**
	 * Adaptor that creates an iterator over a 2D grid
	 */
	template < typename Index, bool RowMajor >
	struct Iterable2D {
		typedef Index index;
		typedef Iterator2D<Index, RowMajor> iterator;
		typedef iterator const_iterator;
		typedef iterator reverse_iterator;

		Iterable2D(int s0, int s1)
		: size0(RowMajor ? s0 : s1), 
		  size1(RowMajor ? s1 : s0) {
		}

		iterator begin() const {
			return iterator(
				0, 0,
				0, size0,
				1, 1
			);
		}

		iterator end() const {
			return iterator(
				0, size1,
				0, size0,
				1, 1
			);
		}

		iterator rbegin() const {
			return iterator(
				size0 - 1, size1 - 1,
				size0 - 1, -1,
				-1, -1
			);
		}

		iterator rend() const {
			return iterator(
				size0 - 1, -1,
				size0 - 1, -1
				- 1, -1
			);
		}

	private:
		int size0, size1;
	};

}

#endif	/* ITERATOR2D_H */
