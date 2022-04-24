/////////////////////////////////////////////////////////////////////
//	File	: MeanValue.h
//	Desc.	: 
//		(c)2000 relic entertainment inc.
//

#pragma once

template< class T, size_t C >
	class MeanValue
{
// fields
private:
	T				m_Value[C];
	T				m_Sum;
	T				m_Average;

	// this should never go above C
	size_t			m_Count;

	// current index in the circular array
	size_t			m_Index;
	
	// count of value equal to Zero
		// NOTE: this is needed for float average, which could be close to zero, 
		// yet not equal
	size_t			m_CountZero;

// construction
public:
	/////////////////////////////////////////////////////////////////////
	//	Name	: MeanValue::MeanValue
	//	Desc.	: 
	//	Param.	: 
	//	Result	: 
	//
	MeanValue()
	:	m_Index(0),
	    m_Count(0),
		m_CountZero(0),
		m_Sum(T()),
		m_Average(T())
	{
	}

	MeanValue( T t )
	:	m_Index(0),
	    m_Count(0),
		m_CountZero(0),
		m_Sum(T()),
		m_Average(T())
	{
		SetValue( t );
	}

	MeanValue& operator = ( const MeanValue<T,C>& rhs )
	{
		if( this != &rhs )
		{
			m_Value = rhs.m_Value;
			m_Index = rhs.m_Index;
			m_Count = rhs.m_Count;
			m_CountZero = rhs.m_CountZero;
			m_Sum = rhs.m_Sum;
			m_Average = rhs.m_Average;
		}

		return *this;
	}

	MeanValue( const MeanValue<T,C>& rhs )
	{
		*this = rhs;
	}

// interface
public:
	/////////////////////////////////////////////////////////////////////
	//	Name	: MeanValue::operator =
	//	Desc.	: Adds a new value to the 
	//	Param.	: 
	//	Result	: 
	//
	void Push( T value )
	{
		// erase old value
		if( m_Index < m_Count )
		{
			m_Sum -= m_Value[m_Index];
			m_CountZero -= ( m_Value[m_Index] == 0 )? 1 : 0;
		}

		// add new value
		m_Value[m_Index] = value;
		m_Sum += value;
		m_CountZero += ( value == 0 )? 1 : 0;

		// keep count
		if( m_Count < C )
			m_Count++;

		// increment index
		if( m_Index == C - 1 )
			m_Index = 0;
		else
			m_Index++;

		// calc new average
		if( m_CountZero == m_Count )
			m_Average = 0;
		else
			m_Average = m_Sum / m_Count;

		return;
	}

	T Mean() const
	{
		return m_Average;
	}

	/////////////////////////////////////////////////////////////////////
	//	Name	: MeanValue::SetValue()
	//	Desc.	: Overrides the mean value assigning 'value' as the mean
	//	Param.	: value - the value that should be mean now
	//	Result	: none
	//
	void SetValue( T value )
	{
		std::fill( &m_Value[0], &m_Value[C], value );
		m_Index = 0;
		m_Count = C;
		m_CountZero = ( value == 0 )? C : 0;
		m_Sum = value*m_Count;
		m_Average = value;
	}

	/////////////////////////////////////////////////////////////////////
	//	Name	: MeanValue::operator =
	//	Desc.	: Adds a new value to the 
	//	Param.	: 
	//	Result	: 
	//
	MeanValue& operator= ( T value )
	{
		Push( value );

		return *this;
	}

	/////////////////////////////////////////////////////////////////////
	//	Name	: MeanValue::operator T
	//	Desc.	: Retrieve the average value
	//	Param.	: 
	//	Result	: 
	//
	operator T() const
	{
		return Mean();
	}

};

//	eof
