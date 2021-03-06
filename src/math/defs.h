/* 
 * File:   defs.h
 * Author: Alexandre Kaspar <akaspar@mit.edu>
 *
 * Created on November 15, 2014, 9:17 PM
 */

#ifndef MATH_DEFS_H
#define	MATH_DEFS_H

#include <algorithm>
#include <cmath>

namespace pm {
	
	/**
	 * Image data type bits
	 */
	enum DataType {
		IM_8U		= 0,
		IM_8S		= 1,
		IM_32S		= 2,
		IM_32F		= 3,
		IM_64F		= 4,
		IM_USRTYPE	= 7,
		IM_UNKNOWN	= -1
	};
	
// bit boundaries
#define IM_CN_MAX     512
#define IM_CN_SHIFT   3
#define IM_DEPTH_MAX  (1 << IM_CN_SHIFT)

// depth of type
#define IM_MAT_DEPTH_MASK       (IM_DEPTH_MAX - 1)
#define IM_MAT_DEPTH(flags)     ((flags) & IM_MAT_DEPTH_MASK)

// full type generation
#define IM_MAKETYPE(depth, cn) (IM_MAT_DEPTH(depth) + (((cn)-1) << IM_CN_SHIFT))
#define IM_MAKE_TYPE(depth, cn) IM_MAKETYPE(depth, cn)

#define IM_8UC1 IM_MAKETYPE(IM_8U,1)
#define IM_8UC3 IM_MAKETYPE(IM_8U,3)
#define IM_8UC(n) IM_MAKETYPE(IM_8U,(n))

#define IM_32SC1 IM_MAKETYPE(IM_32S,1)
#define IM_32SC3 IM_MAKETYPE(IM_32S,3)
#define IM_32SC(n) IM_MAKETYPE(IM_32S,(n))

#define IM_32FC1 IM_MAKETYPE(IM_32F,1)
#define IM_32FC3 IM_MAKETYPE(IM_32F,3)
#define IM_32FC(n) IM_MAKETYPE(IM_32F,(n))

#define IM_64FC1 IM_MAKETYPE(IM_64F,1)
#define IM_64FC3 IM_MAKETYPE(IM_64F,3)
#define IM_64FC(n) IM_MAKETYPE(IM_64F,(n))

// channel & type extraction
#define IM_MAT_CN_MASK          ((IM_CN_MAX - 1) << IM_CN_SHIFT)
#define IM_MAT_CN(flags)        ((((flags) & IM_MAT_CN_MASK) >> IM_CN_SHIFT) + 1)
#define IM_MAT_TYPE_MASK        (IM_DEPTH_MAX*IM_CN_MAX - 1)
#define IM_MAT_TYPE(flags)      ((flags) & IM_MAT_TYPE_MASK)

// size of a type
#define IM_SIZEOF_DEPTH(depth)	(depth <= IM_8S ? sizeof(byte) : \
									(depth <= IM_32F ? sizeof(int) : \
									(depth == IM_64F ? sizeof(double) : sizeof(int*)) ))
#define IM_SIZEOF_IMPL(depth, cn)	(cn > 0 ? cn * IM_SIZEOF_DEPTH(depth) : 0)
#define IM_SIZEOF(flags)		IM_SIZEOF_IMPL(IM_MAT_DEPTH(flags), IM_MAT_CN(flags))
#define IM_SIZEOF_BY_CHANNEL(flags)	IM_SIZEOF_IMPL(IM_MAT_DEPTH(flags), 1)
	
	/**
	 * Basic byte type
	 */
	typedef unsigned char byte;

	/**
	 * Single data type numerical representation
	 */
	template<typename Scalar> struct DataDepth { enum { value = -2, fmt = -1, exact = 0 }; };

	template<> struct DataDepth<bool> { enum { value = IM_8U, fmt=(int)'u', exact = 1 }; };
	template<> struct DataDepth<unsigned char> { enum { value = IM_8U, fmt=(int)'u', exact = 1 }; };
	template<> struct DataDepth<signed char> { enum { value = IM_8S, fmt=(int)'c', exact = 1 }; };
	template<> struct DataDepth<char> { enum { value = IM_8S, fmt=(int)'c', exact = 1 }; };
	template<> struct DataDepth<int> { enum { value = IM_32S, fmt=(int)'i', exact = 1 }; };
	template<> struct DataDepth<unsigned int> { enum { value = IM_32S, fmt=(int)'i', exact = 1 }; };
	template<> struct DataDepth<float> { enum { value = IM_32F, fmt=(int)'f', exact = 0 }; };
	template<> struct DataDepth<double> { enum { value = IM_64F, fmt=(int)'d', exact = 0 }; };
	template<typename Scalar> struct DataDepth<Scalar*> { enum { value = IM_USRTYPE, fmt=(int)'r', exact = 1 }; };
	
	template<int flag> struct DataSize {};
    
    ////////////////////////////////////////////////////////////////////////////
    ///// Rounding implementation //////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    
    // round implementation (this is not perfect!)
	inline int round(float f) {
		int i = std::floor(f + 0.5f);
		// if (i - f > 0.5f) std::cerr << "Invalid floor of " << f << " = " << i << "\n";
		return i;
	}
	template <typename T>
	inline T roundOrNot(float f);
	template <>
	inline float roundOrNot<float>(float f){
		return f;
	}
    template <>
	inline double roundOrNot<double>(float f){
		return f;
	}
	template <>
	inline int roundOrNot<int>(float f){
		return round(f);
	}
    
}

#endif	/* MATH_DEFS_H */

