/////////////////////////////////////////////////////////////////////
// File    : StlExMap.h
// Desc    : 
// Created : Friday, September 14, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

// * this is a drop-in replacement for std::map, that uses
// * a sorted vector instead of a binary tree

// * NOTE: this implementation is not identical to the standard,
// *	all iterators are invalidated whenever an object is added/removed

#pragma once

namespace std
{
	template<class K, class T, class P = std::less<K>, class V = std::vector< std::pair<K,T> > >
    class mapVector
	{
	// types
	public:
		typedef K key_type;
		typedef T referent_type;
		typedef P key_compare;
		typedef std::pair<const K, T> value_type;
		typedef V::size_type size_type;
		typedef V::difference_type difference_type;
		typedef V::reference reference;
		typedef V::const_reference const_reference;
		typedef V::iterator iterator;
		typedef V::const_iterator const_iterator;
	// construction
	public:
	    explicit mapVector(const P& comp = P(), const V& v = V());
	    mapVector(const mapVector& x);

		mapVector& operator= (const mapVector& x);

	// interface
	public:
		iterator		begin();
		const_iterator	begin() const;
		iterator		end();
		const_iterator	end() const;

		size_type		size() const;
		size_type		max_size() const;
		bool			empty() const;

		pair<iterator, bool> 
						insert(const value_type& x);

		iterator		erase(iterator it);
		iterator		erase(iterator first, iterator last);
		size_type		erase(const K& key);

		void			clear();

		void			reserve(size_t n);

		void			swap(mapVector& x);

		size_type		count(const K& key) const;

		iterator		lower_bound(const K& key);
		const_iterator	lower_bound(const K& key) const;
		iterator		upper_bound(const K& key);
		const_iterator	upper_bound(const K& key) const;

	// fields
	private:
		template< class INTKEY, class INTPRED >
		class LessPred
		{
		private:
			INTPRED m_pred;

		public:
			explicit LessPred(const INTPRED& pred)
				: m_pred(pred)
			{
			}
				
		public:
			// less than
			bool operator()( const INTKEY& lhs, const V::value_type& rhs ) const
			{
				return m_pred( lhs, rhs.first );
			}

			// less than
			bool operator()( const V::value_type& lhs, const INTKEY& rhs ) const
			{
				return m_pred( lhs.first, rhs );
			}

			// less than
			bool operator()( const V::value_type& lhs, const V::value_type& rhs ) const
			{
				return m_pred( lhs.first, rhs.first );
			}
		};

		template< class INTPRED >
		class EqualPred
		{
		private:
			INTPRED m_pred;

		public:
			explicit EqualPred(const INTPRED& pred)
				: m_pred(pred)
			{
			}
				
		public:
			// equal
			bool operator()( const V::value_type& lhs, const V::value_type& rhs ) const
			{
				return 
					!m_pred( lhs.first, rhs.first ) && 
					!m_pred( rhs.first, lhs.first );
			}
		};

		V		m_vector;
		P		m_pred;
		bool	m_dirty;

	// implementation
	private:
		void makesorted();

	// template member function
	public:
		template<class A>
			pair<const_iterator, const_iterator> equal_range(const A& key) const
		{
			//
			if( m_dirty ) const_cast<mapVector*>( this )->makesorted();

			//
			std::pair< const_iterator, const_iterator > range = std::equal_range
				( 
				m_vector.begin(),
				m_vector.end(),
				key,
				LessPred<A,P>( m_pred )
				);

			return range;
		}

		template<class A>
			pair<iterator, iterator> equal_range(const A& key)
		{
			//
			if( m_dirty ) makesorted();

			//
			std::pair< iterator, iterator > range = std::equal_range
				( 
				m_vector.begin(),
				m_vector.end(),
				key,
				LessPred<A,P>( m_pred )
				);

			return range;
		}

		template<class A>
			const_iterator find(const A& key) const
		{
			std::pair< const_iterator, const_iterator > range = 
				equal_range( key );

			return ( range.first != range.second )? range.first : end();
		}

		template<class A>
			iterator find(const A& key)
		{
			std::pair< iterator, iterator > range = 
				equal_range( key );

			return ( range.first != range.second )? range.first : end();
		}

		template<class A, class B>
			std::pair< iterator, bool > insert(const A& key, const B& value)
		{
			// check for duplicate
			std::pair<iterator, iterator> range = 
				equal_range(key);

			if( range.first != range.second )
				return std::make_pair( range.first, false );

			// insert in the right place
			iterator it = m_vector.insert( range.first, V::value_type( key, value ) );

			return std::make_pair( it, true );
		}

		template<class A, class B>
			void insert_unsorted(const A& key, const B& value)
		{
			m_vector.push_back( V::value_type( key, value ) );
			m_dirty = true;

			return;
		}

		template<class A>
			referent_type& operator[](const A& key)
		{
			iterator f = find(key);

			if( f != end() )
			{
				return f->second;
			}
			else
			{
				return insert( key, T() ).first->second;
			}
		}
    };
}

///////////////////////////////////////////////////////////////////// 
// implementation

template<class K, class T, class P, class V>
	std::mapVector<K,T,P,V>::mapVector(const P& comp, const V& v)
		: m_vector(v),
          m_pred (comp),
		  m_dirty(false)
{
}

template<class K, class T, class P, class V>
	std::mapVector<K,T,P,V>::mapVector(const std::mapVector<K,T,P,V>& rhs)
{
	*this = rhs;
}

template<class K, class T, class P, class V>
	std::mapVector<K,T,P,V>& std::mapVector<K,T,P,V>::operator= (const std::mapVector<K,T,P,V>& rhs)
{
	if( this != &rhs )
	{
		m_vector = rhs.m_vector;
        m_pred   = rhs.m_pred;
		m_dirty  = rhs.m_dirty;
	}

	return *this;
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::iterator std::mapVector<K,T,P,V>::begin()
{
	return m_vector.begin();
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::const_iterator std::mapVector<K,T,P,V>::begin() const
{
	return m_vector.begin();
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::iterator std::mapVector<K,T,P,V>::end()
{
	return m_vector.end();
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::const_iterator std::mapVector<K,T,P,V>::end() const
{
	return m_vector.end();
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::size_type std::mapVector<K,T,P,V>::size() const
{
	return m_vector.size();
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::size_type std::mapVector<K,T,P,V>::max_size() const
{
	return m_vector.max_size();
}

template<class K, class T, class P, class V>
	inline bool std::mapVector<K,T,P,V>::empty() const
{
	return m_vector.empty();
}

template<class K, class T, class P, class V>
	void std::mapVector<K,T,P,V>::clear()
{
	m_vector.clear();
	m_dirty = false;
}

template<class K, class T, class P, class V>
	void std::mapVector<K,T,P,V>::swap(std::mapVector<K,T,P,V>& x)
{
	m_vector.swap( x.m_vector );
	std::swap( m_pred, x.m_pred );
	std::swap( m_dirty, x.m_dirty );
}

template<class K, class T, class P, class V>
	std::mapVector<K,T,P,V>::iterator std::mapVector<K,T,P,V>::erase(iterator it)
{
	return m_vector.erase(it);
}

template<class K, class T, class P, class V>
	std::mapVector<K,T,P,V>::iterator std::mapVector<K,T,P,V>::erase(iterator first, iterator last)
{
	return m_vector.erase(first,last);
}

template<class K, class T, class P, class V>
	std::mapVector<K,T,P,V>::size_type std::mapVector<K,T,P,V>::erase(const K& key)
{
	std::pair<iterator, iterator> range = 
		equal_range(key);

	const size_type d = std::distance(range.first, range.second);

	erase(range.first, range.second);

	return d;
}

template<class K, class T, class P, class V>
	std::pair< std::mapVector<K,T,P,V>::iterator, bool > 
		std::mapVector<K,T,P,V>::insert(const value_type& x)
{
	return insert(x.first, x.second);
}

template<class K, class T, class P, class V>
	void std::mapVector<K,T,P,V>::makesorted()
{
	if( m_dirty )
	{
		// flag off
		m_dirty = false;

		// sort the vector
		LessPred<K,P> lesspred( m_pred );
		std::sort( m_vector.begin(), m_vector.end(), lesspred );

		// remove duplicates
		EqualPred<P> eqpred( m_pred );
		iterator it = std::adjacent_find(m_vector.begin(), m_vector.end(), eqpred );

		while( it != m_vector.end() )
		{
			it = m_vector.erase(it);
			it = std::adjacent_find(it, m_vector.end(), eqpred );
		}
	}

	return;
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::iterator 
		std::mapVector<K,T,P,V>::lower_bound(const K& key)
{
	//
	if( m_dirty ) makesorted();

	//
	iterator lb = std::lower_bound
		( 
		m_vector.begin(),
		m_vector.end(),
		LessPred<K,P>( m_pred )
		);

	return lb;
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::const_iterator 
		std::mapVector<K,T,P,V>::lower_bound(const K& key) const
{
	//
	if( m_dirty ) const_cast<mapVector*>(this)->makesorted();

	//
	iterator lb = std::lower_bound
		( 
		m_vector.begin(),
		m_vector.end(),
		LessPred<K,P>( m_pred )
		);

	return lb;
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::iterator 
		std::mapVector<K,T,P,V>::upper_bound(const K& key)
{
	//
	if( m_dirty ) makesorted();

	//
	iterator ub = std::upper_bound
		( 
		m_vector.begin(),
		m_vector.end(),
		LessPred<K,P>( m_pred )
		);

	return ub;
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::const_iterator 
		std::mapVector<K,T,P,V>::upper_bound(const K& key) const
{
	//
	if( m_dirty ) const_cast<mapVector*>(this)->makesorted();

	//
	iterator ub = std::upper_bound
		( 
		m_vector.begin(),
		m_vector.end(),
		LessPred<K,P>( m_pred )
		);

	return ub;
}

template<class K, class T, class P, class V>
	inline std::mapVector<K,T,P,V>::size_type std::mapVector<K,T,P,V>::count(const K& key) const
{
	std::pair<const_iterator, const_iterator> range = 
		equal_range(key);

	return std::distance( range.first, range.second );
}

template<class K, class T, class P, class V>
	inline void std::mapVector<K,T,P,V>::reserve(size_t n)
{
	m_vector.reserve(n);
}
