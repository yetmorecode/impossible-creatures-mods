/////////////////////////////////////////////////////////////////////
// File    : StlExSmallVector.h
// Desc    : 
// Created : Tuesday, October 2, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

// * this is a drop-in replacement for std::vector, that will use
// * a compile-time array instead of a dynamically allocated one until
// * the size exceeds the fixed buffer.  It will then allocate memory 
// * hold the vector.  

// * NOTE: this implementation is not identical to the standard,
// *	the objects contained are created when the vector is instantiated,
// *	not when it is resized

namespace std
{
	// forward declarations
	template< typename T, size_t C, class Allocator = std::allocator< T > > class smallvector;


	// NOTE: we need a base class to define iterator_category & distance_type (STLport stuff)
	class smallvectorIterBase
	{
	public:
		typedef size_t						size_type;
		typedef ptrdiff_t					difference_type;
		typedef random_access_iterator_tag	iterator_category;
	};

	// NOTE: the iterator class is defined as a standard class instead of an inner class 'cuz MSVC is weak
	template< class T, size_t C, class Allocator >
		class smallvectorRevIter : public smallvectorIterBase
	{
		friend smallvector<T, C, Allocator>;

	public:
		typedef T	value_type;

		typedef T*	pointer;
		typedef T&	reference;

	public:
		smallvectorRevIter();
		smallvectorRevIter( const smallvectorRevIter& );

		smallvectorRevIter& operator= ( const smallvectorRevIter& );

		T&							operator * ();
		T*							operator ->();

		const T&					operator * () const;
		const T*					operator ->() const;

		bool						operator !=( const smallvectorRevIter& ) const;
		bool						operator ==( const smallvectorRevIter& lhs ) const;
		smallvectorRevIter&			operator ++();
		const smallvectorRevIter&	operator ++() const;
		smallvectorRevIter&			operator +=( size_t inc );
		const smallvectorRevIter&	operator +=( size_t inc ) const;

		T*							base();
		const T*					base() const;

	private:

		smallvectorRevIter( T* );

		T*	m_ptr;
	};

	template< typename T, size_t C, class Allocator = std::allocator< T > >
	class smallvector
	{
	// types
	public:
		enum { MAXLOCAL  = C };
		
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T			value_type;
		typedef T*			iterator;
		typedef const T*	const_iterator;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef Allocator	allocator_type;
		typedef smallvectorRevIter<T, C, Allocator>			reverse_iterator;
		typedef const smallvectorRevIter<T, C, Allocator>	const_reverse_iterator;

	// construction
	public:
		 explicit smallvector( const allocator_type& A = allocator_type() );
		 smallvector( const smallvector& rhs );
		~smallvector();

		smallvector& operator= (const smallvector& rhs);

	// interface
	public:
		const_reference			operator []( size_t index ) const;
		reference				operator []( size_t index );

		iterator				begin();
		const_iterator			begin() const;
		iterator				end();
		const_iterator			end() const;

		reverse_iterator		rbegin();
		const_reverse_iterator	rbegin() const;
		reverse_iterator		rend();
		const_reverse_iterator	rend() const;

		reference				front();
		const_reference			front() const;
		reference				back();
		const_reference			back() const;

		size_t					size() const;
		bool					empty() const;
		size_t					capacity() const;

		void					push_back( const value_type& );
		void					pop_back();
		void					assign(const_iterator first, const_iterator last);
		void					assign(size_type n, const T& x = T());
		iterator				insert(iterator it, const T& x = T());
		void					insert(iterator it, size_type n, const T& x);
		void					insert(iterator it, const_iterator first, const_iterator last);

		void					resize( size_t );
		void					resize( size_t, const value_type& );
		iterator				erase( iterator it );
		iterator				erase( iterator first, iterator last );

		void					clear();

		void					reserve( size_t );

		void					swap( smallvector& );

	// fields
	private:
		value_type*			m_pData;
		size_t				m_count;
		size_t				m_capacity;

		unsigned char		m_fixedData[ MAXLOCAL * sizeof(value_type) ];

		allocator_type		m_allocator;

	// implementation
	private:
		void				extend( size_t size );

		value_type*			internal_addr( );

	};

}

///////////////////////////////////////////////////////////////////// 
// smallvector utilities

template<typename T, size_t C, class A >
	bool operator== 
		( 
		const std::smallvector<T,C,A>& lhs, 
		const std::smallvector<T,C,A>& rhs
		)
{
	if( lhs.size() != rhs.size() )
		return false;

	size_t i = 0;
	size_t e = lhs.size();

	for( ; i != e; ++i )
	{
		if( !(lhs[i] == rhs[i]) )
			break;
	}

	return i == e;
}

////////////////////////////////////////////////////////////////////
// smallvector::reverse_iterator implementation
// must come before it's usage in smallvector's implementation

namespace std
{
	inline forward_iterator_tag
	iterator_category(const smallvectorIterBase&) {return forward_iterator_tag();}

	inline ptrdiff_t* 
	distance_type(const smallvectorIterBase&) {return static_cast<ptrdiff_t*>(0);}
}

template< class T, size_t C, class Allocator >
	std::smallvectorRevIter<T,C,Allocator>::smallvectorRevIter()
		: m_ptr   (0)
{
}

template< class T, size_t C, class Allocator >
	std::smallvectorRevIter<T,C,Allocator>::smallvectorRevIter( T* ptr)
		: m_ptr   (ptr)
{
}

template< class T, size_t C, class Allocator >
	std::smallvectorRevIter<T,C,Allocator>::smallvectorRevIter( const std::smallvectorRevIter<T,C,Allocator>& rhs )
		: m_ptr   (rhs.m_ptr)
{
}

template< class T, size_t C, class Allocator >
	std::smallvectorRevIter<T,C,Allocator>& std::smallvectorRevIter<T,C,Allocator>::operator= ( const std::smallvectorRevIter<T,C,Allocator>& rhs )
{
	m_ptr   = rhs.m_ptr;
	return *this;
}

template< class T, size_t C, class Allocator >
	inline T& std::smallvectorRevIter<T,C,Allocator>::operator *()
{
	dbAssert( m_ptr );
    
	T* tmp = m_ptr;
    return *--tmp;
}

template< class T, size_t C, class Allocator >
	inline T* std::smallvectorRevIter<T,C,Allocator>::operator ->()
{
	return &(operator*());
}

template< class T, size_t C, class Allocator >
	inline bool std::smallvectorRevIter<T,C,Allocator>::operator !=( const std::smallvectorRevIter<T,C,Allocator>& iter ) const
{
	return m_ptr != iter.m_ptr;
}

template< class T, size_t C, class Allocator >
	inline bool std::smallvectorRevIter<T,C,Allocator>::operator ==( const std::smallvectorRevIter<T,C,Allocator>& iter ) const
{
	return m_ptr == iter.m_ptr;
}

template< class T, size_t C, class Allocator >
	inline const std::smallvectorRevIter<T,C,Allocator>& std::smallvectorRevIter<T,C,Allocator>::operator ++() const
{
	return const_cast<std::smallvectorRevIter*>(this)->operator++();
}

template< class T, size_t C, class Allocator >
	inline std::smallvectorRevIter<T,C,Allocator>& std::smallvectorRevIter<T,C,Allocator>::operator ++()
{
	--m_ptr;
	return *this;
}

template< class T, size_t C, class Allocator >
	inline const std::smallvectorRevIter<T,C,Allocator>& std::smallvectorRevIter<T,C,Allocator>::operator +=( size_t inc ) const
{
	return const_cast<smallvectorRevIter*>(this)->operator+=(inc);
}

template< class T, size_t C, class Allocator >
	inline std::smallvectorRevIter<T,C,Allocator>& std::smallvectorRevIter<T,C,Allocator>::operator +=( size_t inc )
{
	m_ptr -= inc;
	return *this;
}

template< class T, size_t C, class Allocator >
	inline T* std::smallvectorRevIter<T,C,Allocator>::base()
{
	return m_ptr;
}

template< class T, size_t C, class Allocator >
	const T* std::smallvectorRevIter<T,C,Allocator>::base() const
{
	return m_ptr;
}

template< class T, size_t C, class Allocator >
	inline std::smallvectorRevIter<T,C,Allocator>::difference_type
		 operator-( const std::smallvectorRevIter<T,C,Allocator>& lhs, 
					const std::smallvectorRevIter<T,C,Allocator>& rhs )
{
	// NOTE: the rhs and lhs are reversed because this is a reverse iterator
	// normally lhs would be higher than rhs in memory but not for reverse iterators
	return rhs.base() - lhs.base();
}

///////////////////////////////////////////////////////////////////// 
// smallvector implementation

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::value_type* std::smallvector<T,C,A>::internal_addr( )
{
	return reinterpret_cast< value_type* >( &m_fixedData[0] );
}

template<typename T, size_t C, class A >
	std::smallvector<T,C,A>::smallvector( const allocator_type& a )
		: m_pData( internal_addr() ),
		  m_count( 0 ),
		  m_capacity( C ),
		  m_allocator( a )
{
}

template<typename T, size_t C, class A >
	std::smallvector<T,C,A>::smallvector( const std::smallvector<T,C,A>& rhs )
		: m_pData( internal_addr() ),
		  m_count( 0 ),
		  m_capacity( C )
{
	*this = rhs;
}

template<typename T, size_t C, class A >
	std::smallvector<T,C,A>::~smallvector()
{
	// call the destructors for the controlled sequence
	clear();

	// free the memory
	if( m_pData != internal_addr() )
	{
		m_allocator.deallocate( m_pData, m_capacity );
	}

	return;
}

template<typename T, size_t C, class A >
	void std::smallvector<T,C,A>::assign(const_iterator first, const_iterator last)
{
	// validate parm
	dbAssert( first <= last );

	size_type n = std::distance( first, last );

	if( m_count < n )
	{
		// make sure our capacity is large enough
		reserve( n );

		// run copy until the end of our controlled sequence
		std::copy( first, first + m_count, begin() );

		// copy construct any remaining elements
		std::uninitialized_copy( first + m_count, last, m_pData + m_count );

		m_count = n;
	}
	else
	{
		// 
		resize( n );

		// copy the entire contents of the rhs into our controlled sequence
		std::copy( first, last, begin() );
	}

	return;
}

template<typename T, size_t C, class A >
	std::smallvector<T,C,A>& std::smallvector<T,C,A>::operator= ( const std::smallvector<T,C,A>& rhs )
{
	if( this != &rhs )
	{
		assign( rhs.begin(), rhs.end() );
	}

	return *this;
}

template<typename T, size_t C, class A >
	void std::smallvector<T,C,A>::push_back( const T& t )
{
	insert( end(), t );
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::const_reference 
		std::smallvector<T,C,A>::operator []( size_t index ) const
{
	dbAssert( (index < m_count) || (index == 0 && m_count == 0) );
	return m_pData[ index ];
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::reference
		std::smallvector<T,C,A>::operator []( size_t index )
{
	dbAssert( (index < m_count) || (index == 0 && m_count == 0) );
	return m_pData[ index ];
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::iterator std::smallvector<T,C,A>::begin()
{
	return m_pData;
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::const_iterator std::smallvector<T,C,A>::begin() const
{
	return m_pData;
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::iterator std::smallvector<T,C,A>::end()
{
	return m_pData+m_count;
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::const_iterator std::smallvector<T,C,A>::end() const
{
	return m_pData+m_count;
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::reverse_iterator std::smallvector<T,C,A>::rbegin()
{
	return reverse_iterator( end() );
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::const_reverse_iterator std::smallvector<T,C,A>::rbegin() const
{
	return reverse_iterator( end() );
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::reverse_iterator std::smallvector<T,C,A>::rend()
{
	return reverse_iterator( begin() );
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::const_reverse_iterator std::smallvector<T,C,A>::rend() const
{
	return reverse_iterator( begin() );
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::reference std::smallvector<T,C,A>::front()
{
	dbAssert( m_count > 0 );
	return m_pData[0];
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::const_reference std::smallvector<T,C,A>::front() const
{
	dbAssert( m_count > 0 );
	return m_pData[0];
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::reference std::smallvector<T,C,A>::back()
{
	dbAssert( m_count > 0 );
	return m_pData[m_count-1];
}

template<typename T, size_t C, class A >
	inline std::smallvector<T,C,A>::const_reference std::smallvector<T,C,A>::back() const
{
	dbAssert( m_count > 0 );
	return m_pData[m_count-1];
}

template<typename T, size_t C, class A >
	inline size_t std::smallvector<T,C,A>::size() const
{
	return m_count;
}

template<typename T, size_t C, class A >
	inline bool std::smallvector<T,C,A>::empty() const
{
	return (m_count==0);
}

template<typename T, size_t C, class A >
	inline size_t std::smallvector<T,C,A>::capacity() const
{
	return m_capacity;
}

template<typename T, size_t C, class A >
	std::smallvector<T,C,A>::iterator
		std::smallvector<T,C,A>::insert( iterator it, const T& x )
{
	// validate parm
	dbAssert( it >= begin() && it <= end() );

	const size_type d = std::distance(begin(), it);

	// it will get blown away by extend()
	extend( m_count+1 );

	if ( d == m_count )
	{
		// push_back()
		std::uninitialized_fill_n( end(), 1, x );
		it = end();
	}
	else
	{
		// insert() not at the end, duplicating last element
		std::uninitialized_fill_n( end(), 1, back() );

		// find where the iterator should be now
		it = begin();
		std::advance( it, d );
		
		// copy_backwards all but the last, since we dup'd it above
		std::copy_backward(it, end()-1, end() );
		*it = x;
	}

	++m_count;

	return it;
}

template<typename T, size_t C, class A >
	std::smallvector<T,C,A>::iterator
		std::smallvector<T,C,A>::erase( iterator it )
{
	dbAssert( it >= begin() && it < end() );

	//
	// 
	std::copy( it + 1, m_pData + m_count, it );
	
	// destroy the last object in the list
	( end() - 1 )->~T();

	m_count--;

	return it;
}

template<typename T, size_t C, class A >
	std::smallvector<T,C,A>::iterator
		std::smallvector<T,C,A>::erase( iterator first, iterator last )
{
	// validate parms
	dbAssert( first >= begin() && first <= last && last <= end() );

	//
	const size_type d = std::distance(first, last);

	// 
	std::copy( last, m_pData + m_count, first );
	
	// destroy old objects
	std::destroy( m_pData + (m_count - d), m_pData + m_count );

	//
	m_count -= d;

	return first;
}

template<typename T, size_t C, class A >
	inline void std::smallvector<T,C,A>::pop_back()
{
	dbAssert( m_count > 0 );

	// destroy the last object in the list
	( end() - 1 )->~T();

	m_count--;
}

template<typename T, size_t C, class A >
	void std::smallvector<T,C,A>::resize(size_t s, const value_type& t)
{
	if( m_count < s )
	{
		// make sure our capacity is large enough
		reserve( s );

		// construct the objects
		std::uninitialized_fill_n( m_pData + m_count, s - m_count, t );
	}
	else
	{
		// erase the end elements of the list
		erase( m_pData + s, m_pData + m_count );
	}

	m_count = s;

	return;
}

template<typename T, size_t C, class A >
	inline void std::smallvector<T,C,A>::resize( size_t s )
{
	resize( s, T() );
}

template<typename T, size_t C, class A >
	inline void std::smallvector<T,C,A>::clear()
{
	erase( begin(), end() );
}

template<typename T, size_t C, class A >
	inline void std::smallvector<T,C,A>::assign(size_type n, const T& x)
{
	clear();
	resize( n, x );
}

template<typename T, size_t C, class A >
	void std::smallvector<T,C,A>::extend(size_t s)
{
	//
	if( m_capacity < s )
	{
		// extend it to the next bigger power of 2
		size_t c;
		for( c = 1; c < s; c <<= 1 )
		{
		}

		// reserve storage
		reserve( c );
	}
}

template<typename T, size_t C, class A >
	void std::smallvector<T,C,A>::reserve(size_t cap)
{
	if( m_capacity < cap )
	{
		//
		const size_t oldcapacity = m_capacity;
		
		// allocate for it
		value_type* pNewData = m_allocator.allocate( cap );

		// copy construct the new memory
		std::uninitialized_copy( m_pData, m_pData + m_count, pNewData );

		// destroy old memory
		std::destroy(begin(), end());

		// 
		std::swap( m_pData, pNewData );
		m_capacity = cap;

		// free old
		if( pNewData != internal_addr() )
		{
			m_allocator.deallocate( pNewData, oldcapacity );
		}
	}

	return;
}

template<typename T, size_t C, class A >
	void std::smallvector<T,C,A>::swap( std::smallvector<T,C,A>& v )
{
	if ( m_pData != internal_addr() && v.m_pData != v.internal_addr() )
	{
		std::swap( m_pData, v.m_pData );
	}
	else
	if ( m_pData != internal_addr() )
	{
		//	Copy from v into the fixed space
		iterator iter = v.begin();
		iterator eiter = v.end();
		iterator fixiter = internal_addr();

		// placement new copy construct the new memory
		for( ; iter != eiter; ++iter, ++fixiter )
		{
			new ( fixiter )T( *iter );
		}

		//	Move the pointer to the other vector
		v.m_pData = m_pData;

		//	Use the fixed data for this vector
		m_pData = internal_addr();
	}
	else
	if ( v.m_pData != v.internal_addr() )
	{
		//	Copy from this into the v's fixed space
		iterator iter = begin();
		iterator eiter = end();
		iterator fixiter = v.internal_addr();

		// placement new copy construct the new memory
		for( ; iter != eiter; ++iter, ++fixiter )
		{
			new ( fixiter )T( *iter );
		}

		//	Move the pointer to this vector
		m_pData = v.m_pData;

		//	Use the fixed data for the other vector
		v.m_pData = v.internal_addr();
	}
	else
	{
		//	Both using the fixed buffer, swap elements

		iterator iter;
		iterator eiter;
		iterator destiter;
		iterator edestiter;

		if ( m_count >= v.m_count )
		{
			iter = begin();
			eiter = end();

			destiter = v.begin();
			edestiter = v.end();
		}
		else
		{
			iter = v.begin();
			eiter = v.end();

			destiter = begin();
			edestiter = end();
		}

		// run the swap up until the end of common controlled sequence
		for ( ; destiter != edestiter; ++iter, ++destiter )
		{
			// swap the elements
			std::swap( *iter, *destiter );
		}

		// copy construct into the uninitialized dest, and destroy the now uncontrolled space
		for ( ; iter != eiter; ++iter, ++destiter )
		{
			// placement new into the uninitialed portion
			new ( destiter )T( *iter );
			// destroy the extra space
			iter->~T();
		}
	}

	std::swap( m_count, v.m_count );
	std::swap( m_capacity, v.m_capacity );

	return;
}