////////////////////////////////////////////////////////////////////
// Callback.h
//
// Dominic Mathieu 
// 2000-10-23
//       (c) relic entertainment inc. 
//
//	* Callback librarie: These callback are used to patch between
//	*	sender and receiver, where receiver can be:
//	*	- a free function
//	*	- a static member function of any kind of object
//	*	- a member function of any kind of object
////////////////////////////////////////////////////////////////////

// NOTE: all asserts have been commented because they crash the compiler

////////////////////////////////////////////////////////////////////
// USAGE:
//
// 1- typedef the specific type of callback 
//		ie: typedef Obj0NRet myCallbackType;
// 2- bind a function to the callback, using the bind function
//      ie: myCallbackType cb = myCallbackType::Bind( ... );
// 3- use the function operator on the callback object
//      ie: cb( );

// No return value
//		- Callback::Obj0nRet = Callback::Obj0nRet::Bind( &function )
//        Callback::Obj0nRet = Callback::Obj0nRet::Bind( &object, &member-function )
//		- Callback::Obj1nRet< param-1 > = Callback::Obj1nRet< param-1 >::Bind( &function )
//        Callback::Obj1nRet< param-1 > = Callback::Obj1nRet< param-1 >::Bind( &object, &member-function )
//		- Callback::Obj2nRet< param-1, parm-2 > = Callback::Obj2nRet< param-1, parm-2 >::Bind( &function )
//        Callback::Obj2nRet< param-1, parm-2 > = Callback::Obj2nRet< param-1, parm-2 >::Bind( &object, &member-function )

// With return value
//		- Callback::Obj0wRet< return-type > = Callback::Obj0wRet< return-type >::Bind( &function )
//        Callback::Obj0wRet< return-type > = Callback::Obj0wRet< return-type >::Bind( &object, &member-function )
//		- Callback::Obj1wRet< return-type, param-1 > = Callback::Obj1wRet< return-type, param-1 >::Bind( &function )
//        Callback::Obj1wRet< return-type, param-1 > = Callback::Obj1wRet< return-type, param-1 >::Bind( &object, &member-function )
//		- Callback::Obj2wRet< return-type, param-1, parm-2 > = Callback::Obj2wRet< return-type, param-1, parm-2 >::Bind( &function )
//        Callback::Obj2wRet< return-type, param-1, parm-2 > = Callback::Obj2wRet< return-type, param-1, parm-2 >::Bind( &object, &member-function )
//		- Callback::Obj3wRet< ... >
//		- Callback::Obj4wRet< ... >

////////////////////////////////////////////////////////////////////
// 

// WARNING: Callback and virtual functions don't mix very well

// NOTE: This library can be used to patch between function with
// different but compatible signature

////////////////////////////////////////////////////////////////////
// The following code is quite complex, cryptic, and ugly. 
// The cryptic comes from avoiding copy-pasting too much code. 

#pragma once

#include <Memory/MemorySmall.h>
#include <Debug/ctAssert.h>

namespace Callback
{
	class CBBase
	{
	private:
		int m_count;

	public:
		CBBase()
			: m_count(1)
		{
		}

	protected:
		virtual ~CBBase()
		{
			dbAssert( m_count == 0 );
		}

	public:
		CBBase* AddRef()
		{
			++m_count;

			return this;
		}

		void Release()
		{
			if( --m_count == 0 )
			{
				delete this;
			}
		}
	};

#define CB_OBJ_INTERNAL_BASE(NAME, TEMPLATE_STATEMENT, RETURN_VALUE, DO_PARMS)	\
	TEMPLATE_STATEMENT														\
		class NAME : public CBBase											\
	{																		\
	public:																	\
		virtual RETURN_VALUE Do( DO_PARMS ) const = 0;						\
																			\
	}

#define CB_OBJ_INTERNAL_MEM(NAME, BASE, TEMPLATE_PARMS_LIST, RETURN_VALUE, RETURN_STATEMENT, DO_PARMS, DO_PARMS_USE ) \
	template< class T, class F /*, comma part of macro */ TEMPLATE_PARMS_LIST >	\
		class NAME : public BASE											\
	{																		\
	/* fields */															\
	private:																\
		T*	m_ptrObject;													\
		F	m_ptrToFunction;												\
																			\
	/* construction */														\
	public:																	\
		NAME( T* t, F f )													\
			: m_ptrObject    ( t ),											\
			  m_ptrToFunction( f )											\
			{																\
			}																\
																			\
	/* copy ctor -- do not define */										\
	private:																\
		NAME( const NAME& );												\
		NAME& operator = ( const NAME& );									\
																			\
	/* interface */															\
	public:																	\
		virtual RETURN_VALUE Do( DO_PARMS ) const							\
			{																\
			RETURN_STATEMENT ( m_ptrObject->*m_ptrToFunction )( DO_PARMS_USE );	\
			}																\
	}

#define CB_OBJ_INTERNAL_FREE(NAME, BASE, TEMPLATE_PARMS_LIST, RETURN_VALUE, RETURN_STATEMENT, DO_PARMS, DO_PARMS_USE) \
	template< class F/*, comma part of macro */ TEMPLATE_PARMS_LIST >		\
		class NAME : public BASE											\
	{																		\
	/* fields */															\
	private:																\
		F	m_ptrToFunction;												\
																			\
	/* construction */														\
	public:																	\
		NAME( F f )															\
			: m_ptrToFunction( f )											\
			{																\
			}																\
																			\
	/* copy ctor -- do not define */										\
	private:																\
		NAME( const NAME& );												\
		NAME& operator = ( const NAME& );									\
																			\
	/* interface */															\
	public:																	\
		virtual RETURN_VALUE Do( DO_PARMS ) const							\
			{																\
			RETURN_STATEMENT m_ptrToFunction( DO_PARMS_USE );				\
			}																\
	}

#define CB_OBJ_INTERNAL_FREEBOUNDED(NAME, BASE, TEMPLATE_PARMS_LIST, RETURN_VALUE, RETURN_STATEMENT, DO_PARMS, DO_PARMS_USE) \
	template< class F, class BOUNDED/*, comma part of macro */ TEMPLATE_PARMS_LIST >		\
		class NAME : public BASE											\
	{																		\
	/* fields */															\
	private:																\
		F		m_ptrToFunction;											\
		BOUNDED	m_bounded;													\
																			\
	/* construction */														\
	public:																	\
		NAME( F f, BOUNDED b )												\
			: m_ptrToFunction( f ),											\
			  m_bounded( b )												\
			{																\
			}																\
																			\
	/* copy ctor -- do not define */										\
	private:																\
		NAME( const NAME& );												\
		NAME& operator = ( const NAME& );									\
																			\
	/* interface */															\
	public:																	\
		virtual RETURN_VALUE Do( DO_PARMS ) const							\
			{																\
			RETURN_STATEMENT m_ptrToFunction( DO_PARMS_USE, m_bounded );	\
			}																\
	}

#define CB_OBJ_HANDLE(NAME, NAMEFULL, BASE, INTERNALMEMBER, INTERNALFREE, TEMPLATE_STATEMENT, TEMPLATE_PARMS_USE_LIST3, RETURN_VALUE, RETURN_STATEMENT, DO_PARMS, DO_PARMS_USE) \
	TEMPLATE_STATEMENT														\
		class NAME															\
	{																		\
	/* types */																\
	public:																	\
		typedef BASE Type;													\
																			\
	/* fields */															\
	public:																	\
		Type* m_obj;														\
																			\
	/* construction */														\
	public:																	\
		template< class F >													\
			static NAMEFULL Bind( F f )										\
			{																\
			/* object to be returned */										\
			NAMEFULL r;														\
																			\
			/* */															\
			typedef INTERNALFREE< F /*, comma part of macro */ TEMPLATE_PARMS_USE_LIST3 > \
				CB;															\
																			\
			r.m_obj = new CB( f );											\
																			\
			return( r );													\
			}																\
																			\
		template< class T, class F >										\
			static NAMEFULL Bind( T* t, F f )								\
			{																\
			/* object to be returned */										\
			NAMEFULL r;														\
																			\
			/* */															\
			typedef INTERNALMEMBER< T, F /*, comma part of macro */ TEMPLATE_PARMS_USE_LIST3 > \
				CB;															\
																			\
			r.m_obj = new CB( t, f );										\
																			\
			return( r );													\
			}																\
																			\
			static NAMEFULL Bind( Type* p )									\
			{																\
			/* object to be returned */										\
			NAMEFULL r;														\
																			\
			/* */															\
			r.m_obj = p;													\
																			\
			return( r );													\
			}																\
																			\
		 NAME( )															\
			: m_obj(0)														\
			{																\
			}																\
																			\
		~NAME( )															\
			{																\
			reset();														\
			}																\
																			\
		 NAME( const NAME& rhs )											\
			: m_obj( rhs.m_obj? static_cast<Type*>( rhs.m_obj->AddRef() ) : 0 ) \
			{																\
			}																\
																			\
		 NAME& operator = ( const NAME& rhs )								\
			{																\
				NAME tmp( rhs );											\
																			\
				std::swap( m_obj, tmp.m_obj );								\
																			\
				return( *this );											\
			}																\
																			\
	/* interface */															\
	public:																	\
		/* returns false if the object contains a */						\
		/* callback function */												\
		bool empty() const													\
			{																\
				return m_obj == 0;											\
			}																\
																			\
		/* delete the function pointer inside this callback */				\
		void reset()														\
			{																\
				if( m_obj )													\
				{															\
					m_obj->Release();										\
					m_obj = 0;												\
				}															\
			}																\
																			\
		RETURN_VALUE operator ( ) ( DO_PARMS ) const						\
			{																\
			RETURN_STATEMENT m_obj->Do( DO_PARMS_USE );						\
			}																\
	}

	////////////////////////////////////////////////////////////////////
	// 0nRet

#define CB_OBJ_TEMPLATE_PARMS			
#define CB_OBJ_TEMPLATE_PARMS_USE		
#define CB_OBJ_RETURN_VALUE				void
#define CB_OBJ_RETURN_STATEMENT		
#define CB_OBJ_DO_PARMS					
#define CB_OBJ_DO_PARMS_USE				

#define CB_OBJ_TEMPLATE_PARMS_LIST		
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	
#define CB_OBJ_TEMPLATE_STATEMENT		

	CB_OBJ_INTERNAL_BASE( Obj0nRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );
	CB_OBJ_INTERNAL_MEM ( Obj0nRetInternalMember, Obj0nRetInternal, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj0nRetInternalFree,   Obj0nRetInternal, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj0nRet,               Obj0nRet, Obj0nRetInternal, Obj0nRetInternalMember, Obj0nRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 1nRet

#define CB_OBJ_TEMPLATE_PARMS			class P1
#define CB_OBJ_TEMPLATE_PARMS_USE		P1
#define CB_OBJ_RETURN_VALUE				void
#define CB_OBJ_RETURN_STATEMENT		
#define CB_OBJ_DO_PARMS					P1 p1
#define CB_OBJ_DO_PARMS_USE				p1

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj1nRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );
	CB_OBJ_INTERNAL_MEM ( Obj1nRetInternalMember, Obj1nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj1nRetInternalFree,   Obj1nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj1nRet, Obj1nRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj1nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj1nRetInternalMember, Obj1nRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 2nRet

#define CB_OBJ_TEMPLATE_PARMS			class P1, class P2
#define CB_OBJ_TEMPLATE_PARMS_USE		P1, P2
#define CB_OBJ_RETURN_VALUE				void
#define CB_OBJ_RETURN_STATEMENT		
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2
#define CB_OBJ_DO_PARMS_USE				p1, p2

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj2nRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );
	CB_OBJ_INTERNAL_MEM ( Obj2nRetInternalMember, Obj2nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj2nRetInternalFree,   Obj2nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj2nRet, Obj2nRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj2nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj2nRetInternalMember, Obj2nRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 3nRet

#define CB_OBJ_TEMPLATE_PARMS			class P1, class P2, class P3
#define CB_OBJ_TEMPLATE_PARMS_USE		P1, P2, P3
#define CB_OBJ_RETURN_VALUE				void
#define CB_OBJ_RETURN_STATEMENT		
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2, P3 p3
#define CB_OBJ_DO_PARMS_USE				p1, p2, p3

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj3nRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );
	CB_OBJ_INTERNAL_MEM ( Obj3nRetInternalMember, Obj3nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj3nRetInternalFree,   Obj3nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj3nRet, Obj3nRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj3nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj3nRetInternalMember, Obj3nRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 4nRet

#define CB_OBJ_TEMPLATE_PARMS			class P1, class P2, class P3, class P4
#define CB_OBJ_TEMPLATE_PARMS_USE		P1, P2, P3, P4
#define CB_OBJ_RETURN_VALUE				void
#define CB_OBJ_RETURN_STATEMENT		
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2, P3 p3, P4 p4
#define CB_OBJ_DO_PARMS_USE				p1, p2, p3, p4

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj4nRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );
	CB_OBJ_INTERNAL_MEM ( Obj4nRetInternalMember, Obj4nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj4nRetInternalFree,   Obj4nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj4nRet, Obj4nRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj4nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj4nRetInternalMember, Obj4nRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 5nRet

#define CB_OBJ_TEMPLATE_PARMS			class P1, class P2, class P3, class P4, class P5
#define CB_OBJ_TEMPLATE_PARMS_USE		P1, P2, P3, P4, P5
#define CB_OBJ_RETURN_VALUE				void
#define CB_OBJ_RETURN_STATEMENT		
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2, P3 p3, P4 p4, P5 p5
#define CB_OBJ_DO_PARMS_USE				p1, p2, p3, p4, p5

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj5nRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );
	CB_OBJ_INTERNAL_MEM ( Obj5nRetInternalMember, Obj5nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj5nRetInternalFree,   Obj5nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj5nRet, Obj5nRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj5nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj5nRetInternalMember, Obj5nRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 6nRet

#define CB_OBJ_TEMPLATE_PARMS			class P1, class P2, class P3, class P4, class P5, class P6
#define CB_OBJ_TEMPLATE_PARMS_USE		P1, P2, P3, P4, P5, P6
#define CB_OBJ_RETURN_VALUE				void
#define CB_OBJ_RETURN_STATEMENT		
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6
#define CB_OBJ_DO_PARMS_USE				p1, p2, p3, p4, p5, p6

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj6nRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );
	CB_OBJ_INTERNAL_MEM ( Obj6nRetInternalMember, Obj6nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj6nRetInternalFree,   Obj6nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj6nRet, Obj6nRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj6nRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj6nRetInternalMember, Obj6nRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT



	////////////////////////////////////////////////////////////////////
	// 0wRet

#define CB_OBJ_TEMPLATE_PARMS			class R
#define CB_OBJ_TEMPLATE_PARMS_USE		R
#define CB_OBJ_RETURN_VALUE				R
#define CB_OBJ_RETURN_STATEMENT			return
#define CB_OBJ_DO_PARMS				
#define CB_OBJ_DO_PARMS_USE			

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj0wRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );

	CB_OBJ_INTERNAL_MEM ( Obj0wRetInternalMember, Obj0wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj0wRetInternalFree,   Obj0wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj0wRet, Obj0wRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj0wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj0wRetInternalMember, Obj0wRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_STATEMENT
#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 1wRet

#define CB_OBJ_TEMPLATE_PARMS			class R, class P1
#define CB_OBJ_TEMPLATE_PARMS_USE		R, P1
#define CB_OBJ_RETURN_VALUE				R
#define CB_OBJ_RETURN_STATEMENT			return
#define CB_OBJ_DO_PARMS					P1 p1
#define CB_OBJ_DO_PARMS_USE				p1

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj1wRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );

	CB_OBJ_INTERNAL_MEM ( Obj1wRetInternalMember, Obj1wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj1wRetInternalFree,   Obj1wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj1wRet, Obj1wRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj1wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj1wRetInternalMember, Obj1wRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_STATEMENT
#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 2wRet

#define CB_OBJ_TEMPLATE_PARMS			class R, class P1, class P2
#define CB_OBJ_TEMPLATE_PARMS_USE		R, P1, P2
#define CB_OBJ_RETURN_VALUE				R
#define CB_OBJ_RETURN_STATEMENT			return
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2
#define CB_OBJ_DO_PARMS_USE				p1, p2

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj2wRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );

	CB_OBJ_INTERNAL_MEM ( Obj2wRetInternalMember, Obj2wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj2wRetInternalFree,   Obj2wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj2wRet, Obj2wRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj2wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj2wRetInternalMember, Obj2wRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_STATEMENT
#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 3wRet

#define CB_OBJ_TEMPLATE_PARMS			class R, class P1, class P2, class P3
#define CB_OBJ_TEMPLATE_PARMS_USE		R, P1, P2, P3
#define CB_OBJ_RETURN_VALUE				R
#define CB_OBJ_RETURN_STATEMENT			return
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2, P3 p3
#define CB_OBJ_DO_PARMS_USE				p1, p2, p3

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj3wRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );

	CB_OBJ_INTERNAL_MEM ( Obj3wRetInternalMember, Obj3wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj3wRetInternalFree,   Obj3wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj3wRet, Obj3wRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj3wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj3wRetInternalMember, Obj3wRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_STATEMENT
#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

	////////////////////////////////////////////////////////////////////
	// 4wRet

#define CB_OBJ_TEMPLATE_PARMS			class R, class P1, class P2, class P3, class P4
#define CB_OBJ_TEMPLATE_PARMS_USE		R, P1, P2, P3, P4
#define CB_OBJ_RETURN_VALUE				R
#define CB_OBJ_RETURN_STATEMENT			return
#define CB_OBJ_DO_PARMS					P1 p1, P2 p2, P3 p3, P4 p4
#define CB_OBJ_DO_PARMS_USE				p1, p2, p3, p4

#define CB_OBJ_TEMPLATE_PARMS_LIST		, CB_OBJ_TEMPLATE_PARMS
#define CB_OBJ_TEMPLATE_PARMS_LIST2 	CB_OBJ_TEMPLATE_PARMS, 
#define CB_OBJ_TEMPLATE_PARMS_USE_LIST3	, CB_OBJ_TEMPLATE_PARMS_USE
#define CB_OBJ_TEMPLATE_STATEMENT		template< CB_OBJ_TEMPLATE_PARMS >

	CB_OBJ_INTERNAL_BASE( Obj4wRetInternal, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_RETURN_VALUE, CB_OBJ_DO_PARMS );

	CB_OBJ_INTERNAL_MEM ( Obj4wRetInternalMember, Obj4wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );
	CB_OBJ_INTERNAL_FREE( Obj4wRetInternalFree,   Obj4wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, CB_OBJ_TEMPLATE_PARMS_LIST, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

	CB_OBJ_HANDLE       ( Obj4wRet, Obj4wRet< CB_OBJ_TEMPLATE_PARMS_USE >, Obj4wRetInternal< CB_OBJ_TEMPLATE_PARMS_USE >, Obj4wRetInternalMember, Obj4wRetInternalFree, CB_OBJ_TEMPLATE_STATEMENT, CB_OBJ_TEMPLATE_PARMS_USE_LIST3, CB_OBJ_RETURN_VALUE, CB_OBJ_RETURN_STATEMENT, CB_OBJ_DO_PARMS, CB_OBJ_DO_PARMS_USE );

#undef CB_OBJ_TEMPLATE_STATEMENT
#undef CB_OBJ_TEMPLATE_PARMS
#undef CB_OBJ_TEMPLATE_PARMS_USE
#undef CB_OBJ_RETURN_VALUE
#undef CB_OBJ_RETURN_STATEMENT
#undef CB_OBJ_DO_PARMS
#undef CB_OBJ_DO_PARMS_USE
#undef CB_OBJ_TEMPLATE_PARMS_LIST
#undef CB_OBJ_TEMPLATE_PARMS_LIST2
#undef CB_OBJ_TEMPLATE_PARMS_USE_LIST3
#undef CB_OBJ_TEMPLATE_STATEMENT

} // namespace
