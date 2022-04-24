////////////////////////////////////////////////////////////////////
// Array2D.h
//
// Shelby
// 2000-10-21
//       (c) relic entertainment inc. 
//
//	* Just a generic 2D array that checks for out of bounds errors and has a simple
//	* inline interface for accessing this templatized array
//
//	* Note: could use Image.h if you need a bit more functionality (ie. load/save/blt)
////////////////////////////////////////////////////////////////////

#pragma once

// forward declarations
template< class T, class Allocator > class Array2D;

////////////////////////////////////////////////////////////////////
// Array2DIteratorBase

// NOTE: we need a base class to define iterator_category & distance_type (STLport stuff)
class Array2DIteratorBase
{
public:
  typedef size_t					size_type;
  typedef ptrdiff_t					difference_type;
  typedef std::forward_iterator_tag	iterator_category;
};

// NOTE: the iterator class is defined as a standard class instead of an inner class 'cuz MSVC is weak
template< class T, class Allocator >
	class Array2DIterator : public Array2DIteratorBase
{
	friend Array2D<T,Allocator>;

public:
	typedef T	value_type;

	typedef T*	pointer;
	typedef T&	reference;

public:
	Array2DIterator();
	Array2DIterator( const Array2DIterator& );

	Array2DIterator& operator= ( const Array2DIterator& );

	T&						operator * ();
	T*						operator ->();

	const T&				operator * () const;
	const T*				operator ->() const;

	bool					operator !=( const Array2DIterator& ) const;
	bool					operator ==( const Array2DIterator& lhs ) const;
	Array2DIterator&		operator ++();
	const Array2DIterator&	operator ++() const;
	Array2DIterator&		operator +=( size_t inc );
	const Array2DIterator&	operator +=( size_t inc ) const;

	void					SetXY( size_t x, size_t y );

private:
	T*	m_ptr;
	Array2D<T,Allocator>*	
		m_array;
};

///////////////////////////////////////////////////////////////////// 
// Array2DNewAllocator

template< class T >
	class Array2DNewAllocator
{
public:
	void Free( T* p, unsigned long w, unsigned long h )
	{
		UNREF_P( w );
		UNREF_P( h );

		DELETEARRAYZERO( p );
	}

	T* Alloc( unsigned long w, unsigned long h )
	{
		return new T[ w * h ];
	}
};

////////////////////////////////////////////////////////////////////
// Array2D

template< class T, class Allocator = Array2DNewAllocator<T> >
	class Array2D : private Allocator
{
// typedefs
public:
	typedef Array2DIterator<T,Allocator>			iterator;
	typedef const Array2DIterator<T,Allocator>	const_iterator;

// construction
public:
		// array will be unitialized
	 Array2D( );
	 Array2D( const Array2D& );
	~Array2D( );

// interface
public:
	void			SetSize  ( unsigned long width, unsigned long height );
	unsigned long	GetWidth () const;
	unsigned long	GetHeight() const;
	unsigned long	GetSize  () const;

	const T*		GetData  () const;
	T*				GetData  ();

	const T&		GetValue ( unsigned long x, unsigned long y ) const;
	T&				GetValue ( unsigned long x, unsigned long y );

	void			SetValue ( unsigned long x, unsigned long y, const T& c );

	void			FillValue( const T& c );

	Array2D&		operator= ( const Array2D& );

	iterator		begin();
	const_iterator	begin() const;
	iterator		end();
	const_iterator	end() const;

// fields
private:
	T*					m_array;
	unsigned long		m_width;
	unsigned long		m_height;
	iterator			m_begin, m_end;
};

////////////////////////////////////////////////////////////////////
// Array2D implementation

template< class T, class Allocator >
	Array2D<T,Allocator>::Array2D()
		: m_array( NULL ),
		  m_width( 0 ),
		  m_height( 0 )
{
}

template< class T, class Allocator >
	Array2D<T,Allocator>::Array2D( const Array2D& a )
		: m_array( NULL ),
		  m_width( 0 ),
		  m_height( 0 )
{
	*this = a;
}

template< class T, class Allocator >
	Array2D<T,Allocator>::~Array2D()
{
	SetSize(0,0);
}

template< class T, class Allocator >
	void Array2D<T,Allocator>::SetSize( unsigned long w, unsigned long h )
{
	if ( ( w != m_width ) || ( h != m_height ) )
	{
		// 
		if( m_array != 0 )
		{
			Allocator::Free( m_array, m_width, m_height );
		}

		//
		m_width = w;
		m_height = h;

		// allocate memory
		if ( m_width > 0 && m_height > 0 )
		{
			m_array = Allocator::Alloc( m_width, m_height );
		}
		else
		{
			m_array = NULL;
		}

		// setup iterator pointers
		m_begin.m_ptr = m_array;
		m_begin.m_array = this;
		m_end.m_ptr = m_array + (m_width * m_height);
		m_end.m_array = this;
	}
}

template< class T, class Allocator >
	unsigned long Array2D<T,Allocator>::GetWidth( ) const	
{	
	return m_width;
}

template< class T, class Allocator >
	unsigned long Array2D<T,Allocator>::GetHeight( ) const	
{	
	return m_height;
}

template< class T, class Allocator >
	unsigned long Array2D<T,Allocator>::GetSize( ) const		
{	
	return GetWidth( ) * GetHeight( );		
}

template< class T, class Allocator >
	const T* Array2D<T,Allocator>::GetData() const	
{	
	return m_array;		
}

template< class T, class Allocator >
	T* Array2D<T,Allocator>::GetData()
{	
	return m_array;		
}

template< class T, class Allocator >
	const T& Array2D<T,Allocator>::GetValue( unsigned long x, unsigned long y ) const
{
	dbAssert( ( x >= 0) && ( x < m_width  ) );
	dbAssert( ( y >= 0) && ( y < m_height ) );

	return m_array[ ( y * GetWidth( ) ) + x ];
}

template< class T, class Allocator >
	T& Array2D<T,Allocator>::GetValue( unsigned long x, unsigned long y )
{
	dbAssert( ( x >= 0) && ( x < m_width  ) );
	dbAssert( ( y >= 0) && ( y < m_height ) );

	return m_array[ ( y * GetWidth( ) ) + x ];
}

template< class T, class Allocator >
	void Array2D<T,Allocator>::SetValue( unsigned long x, unsigned long y, const T& c )
{
	dbAssert( ( x >= 0) && ( x < m_width  ) );
	dbAssert( ( y >= 0) && ( y < m_height ) );

	m_array[ ( y * GetWidth( ) ) + x ] = c;

	return;
}

template< class T, class Allocator >
	void Array2D<T,Allocator>::FillValue( const T& c )
{
	std::fill( m_array, m_array + GetSize(), c );
}

template< class T, class Allocator >
	Array2D<T,Allocator>& Array2D<T,Allocator>::operator =( const Array2D& rhs )
{
	if( this != &rhs )
	{
		SetSize( rhs.GetWidth(), rhs.GetHeight() );
		std::copy( rhs.begin(), rhs.end(), begin() );
	}
	
	return *this;
}

template< class T, class Allocator >
	Array2D<T,Allocator>::iterator Array2D<T,Allocator>::begin()
{
	return m_begin;
}

template< class T, class Allocator >
	Array2D<T,Allocator>::const_iterator Array2D<T,Allocator>::begin() const
{
	return m_begin;
}

template< class T, class Allocator >
	Array2D<T,Allocator>::iterator Array2D<T,Allocator>::end()
{
	return m_end;
}

template< class T, class Allocator >
	Array2D<T,Allocator>::const_iterator Array2D<T,Allocator>::end() const
{
	return m_end;
}

////////////////////////////////////////////////////////////////////
// Array2D::iterator implementation

namespace std
{
	inline forward_iterator_tag
	iterator_category(const Array2DIteratorBase&) {return forward_iterator_tag();}

	inline ptrdiff_t* 
	distance_type(const Array2DIteratorBase&) {return static_cast<ptrdiff_t*>(0);}
}

template< class T, class Allocator >
	Array2DIterator<T,Allocator>::Array2DIterator()
		: m_array (0),
		  m_ptr   (0)
{
}

template< class T, class Allocator >
	Array2DIterator<T,Allocator>::Array2DIterator( const Array2DIterator& rhs )
		: m_array (0),
		  m_ptr   (0)
{
	*this = rhs;
}

template< class T, class Allocator >
	Array2DIterator<T,Allocator>& Array2DIterator<T,Allocator>::operator= ( const Array2DIterator& rhs )
{
	m_array = rhs.m_array;
	m_ptr   = rhs.m_ptr;

	return *this;
}

template< class T, class Allocator >
	inline T& Array2DIterator<T,Allocator>::operator *()
{
	dbAssert( m_array );
	dbAssert( m_ptr >= m_array->GetData() );
	dbAssert( m_ptr < m_array->GetData()+(m_array->GetWidth()*m_array->GetHeight()) );
	
	return *m_ptr;
}

template< class T, class Allocator >
	inline T* Array2DIterator<T,Allocator>::operator ->()
{
	dbAssert( m_array );
	dbAssert( m_ptr >= m_array->GetData() );
	dbAssert( m_ptr < m_array->GetData()+(m_array->GetWidth()*m_array->GetHeight()) );

	return m_ptr;
}

template< class T, class Allocator >
	inline bool Array2DIterator<T,Allocator>::operator !=( const Array2DIterator& iter ) const
{
	return m_ptr != iter.m_ptr;
}

template< class T, class Allocator >
	inline bool Array2DIterator<T,Allocator>::operator ==( const Array2DIterator& iter ) const
{
	return m_ptr == iter.m_ptr;
}

template< class T, class Allocator >
	inline const Array2DIterator<T,Allocator>& Array2DIterator<T,Allocator>::operator ++() const
{
	return const_cast<Array2DIterator*>(this)->operator++();
}

template< class T, class Allocator >
	inline Array2DIterator<T,Allocator>& Array2DIterator<T,Allocator>::operator ++()
{
	dbAssert( m_array );
	dbAssert( m_ptr >= m_array->GetData() );
	dbAssert( m_ptr < m_array->GetData()+(m_array->GetWidth()*m_array->GetHeight()) );

	++m_ptr;

	return *this;
}

template< class T, class Allocator >
	inline const Array2DIterator<T,Allocator>& Array2DIterator<T,Allocator>::operator +=( size_t inc ) const
{
	return const_cast<Array2DIterator*>(this)->operator+=(inc);
}

template< class T, class Allocator >
	inline Array2DIterator<T,Allocator>& Array2DIterator<T,Allocator>::operator +=( size_t inc )
{
	dbAssert( m_array );
	dbAssert( m_ptr >= m_array->GetData() );
	dbAssert( m_ptr < m_array->GetData()+(m_array->GetWidth()*m_array->GetHeight()) );

	m_ptr += inc;

	return *this;
}

template< class T, class Allocator >
	inline void Array2DIterator<T,Allocator>::SetXY( size_t x, size_t y )
{
	dbAssert( m_array );
	dbAssert( m_ptr );
	dbAssert( x < m_array->GetWidth() );
	dbAssert( y < m_array->GetHeight() );

	m_ptr = m_array->GetData() + (m_array->GetWidth()*y + x);
}
