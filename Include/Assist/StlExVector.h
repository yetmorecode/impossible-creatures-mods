/////////////////////////////////////////////////////////////////////
// File    : StlExVector.h
// Desc    : 
// Created : Monday, October 08, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * utility classes to use with std::vector

#pragma once

///////////////////////////////////////////////////////////////////// 
// 

namespace std
{
	// erase an element from the vector by swapping it with the last one
	// and pop_back
	template< class V, class IT >
		IT vector_eraseback(V& v, IT it)
	{
		// swap
		std::swap( *it, v.back() );

		// erase
		v.pop_back();

		// this assumes that removing an element does NOT reallocate
		return it;
	}
}
