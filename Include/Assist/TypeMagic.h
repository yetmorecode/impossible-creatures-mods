/////////////////////////////////////////////////////////////////////
// File    : TypeMagic.h
// Desc    : 
// Created : Monday, June 04, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

////////////////////////////////////////////////////////////////////////////////
// This code is extracted from 'The Loki Library'
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.

#pragma once

namespace TypeMagic
{
	///////////////////////////////////////////////////////////////////// 
	// 

	template<class T, class U> class IsType
	{
	private:
		template< class N > class Check    { public: typedef char	R; };
		template<>          class Check<U> { public: typedef double R; };

	public:
		enum { result = ( sizeof(Check<U>::R) == sizeof(Check<T>::R) ) };
	};

	///////////////////////////////////////////////////////////////////// 
	// 

	template<class T> class IsBuiltIn
	{
	public:
		enum
		{
			result = 
	// VC++ 7.0 has a built-in type wchar_t that's different than 'unsigned short'
#if defined(_MSC_VER) 
	#if _MSC_VER > 1200
				IsType<T,wchar_t       >::result ||
	#endif
#endif
				IsType<T,long          >::result ||
				IsType<T,unsigned long >::result ||
				IsType<T,short         >::result ||
				IsType<T,unsigned short>::result ||
				IsType<T,int           >::result ||
				IsType<T,unsigned int  >::result ||
				IsType<T,char          >::result ||
				IsType<T,unsigned char >::result ||
				IsType<T,float         >::result ||
				IsType<T,double        >::result
		};
	};

	///////////////////////////////////////////////////////////////////// 
	// 

	template<class T> class HasVirtualTable
	{
	private:
	   class X : public T
	   {
		  X();
		  virtual void dummy();
	   };

	public:
	   enum { result = ( sizeof(X) == sizeof(T) ) };
	};

	///////////////////////////////////////////////////////////////////// 
	// 

	template<class FROM, class TO> class IsConvertible
	{
	private:
		typedef char	TypeYes;
		typedef double	TypeNo;

		static TypeNo	Check(...);
		static TypeYes	Check(TO);

	public:
      static FROM _from;
	  enum { result = sizeof( Check(_from) ) == sizeof(TypeYes) };
	};
}

//
//	a better lengthof : 
//		- doesn't allow LENGTHOF(T*), only LENGTHOF(T[])
//		- produces a constant value ( handles "T b[LENGTHOF(a)];" and "for (i=0;i<LENGTHOF(a);++){}" )
//		- doesn't support local types, due to use of T
//		- source http://www.cuj.com/articles/1999/9907/9907i/9907i.htm?topic=articles
//
namespace lengthof
{
	template< size_t N >
	struct Sized
	{
		char x[ N ];
	};
		    
	template< class T, size_t N >
	Sized< N > Sizer( T (&)[ N ] )
	{
		Sized< N > x ;
		return( x ) ;
	}
}

#define LENGTHOF( a ) sizeof ( lengthof::Sizer( a ) )
