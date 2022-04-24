/////////////////////////////////////////////////////////////////////
// File    : LuaBinding.h
// Desc    : 
// Created : Monday, April 23, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//

#pragma once

#include "LuaConfig.h"

///////////////////////////////////////////////////////////////////// 
// LuaBinding

// the 1st thing of interest in here is LuaBinding::Bind()
// the function works like this
// LuaBinding::Bind( LuaConfig, LuaFunctionName, ObjPtr, ObjMemFunc, Error string )

// the 2nd thing of interest is LuaBinding::Call()

// the 3rd thing of interest is LuaBinding::StaticBound

namespace LuaBinding
{
	//
	struct StaticBound
	{
		const char* name;
		LuaConfig::LuaFunc func;
	};

	//
	void LUA_API StaticBind ( LuaConfig& lc, const StaticBound* array, size_t arrayLen );
	void LUA_API StaticClear( LuaConfig& lc, const StaticBound* array, size_t arrayLen );

	// forward declaration
	class ObjInternalBase;

	///////////////////////////////////////////////////////////////////// 
	// Obj

	// hold refcounted internal class, used for keeping in list
	class LUA_API Obj
	{
	// construction
	public:
		 explicit Obj( ObjInternalBase* );
	
	public:
		 Obj();
		 Obj( const Obj& );
		~Obj();
		
		 Obj& operator= ( const Obj& );

	// interface
	public:
		bool IsRegistered() const;
		void Reset();

	// fields
	private:
		ObjInternalBase* m_sharedCb;

	// implementation
	private:
		static int Dispatcher( lua_State* L );
	};

	///////////////////////////////////////////////////////////////////// 
	// ObjInternalBase

	// base class for internal classes
	class LUA_API ObjInternalBase
	{
		friend Obj;

	// construction
	protected:
		ObjInternalBase( LuaConfig* lc, const char* funcname, const char* usage );
		virtual ~ObjInternalBase();

	// interface
	public:
		const char*		GetFunctionName () const;
		const char*		GetFunctionUsage() const;

		virtual int		Dispatch( lua_State* ) = 0;

	protected:
		void			CheckArgs( lua_State* L, const size_t argc );
	
	// fields
	private:
		const char*	m_funcname;
		const char*	m_usage;

		LuaConfig*	m_lc;

		int			m_count;

	// copy -- do not define
	private:
		ObjInternalBase( const ObjInternalBase& );
		ObjInternalBase& operator= ( const ObjInternalBase& );
	};

	///////////////////////////////////////////////////////////////////// 
	// traits

	// traits to pop/push params from lua stack
	template< class TYPE > class ParmTraits
	{
	public:
		typedef TYPE type;

	public:
		static void Push( const type t, lua_State* );
		static bool Pop ( type& t, lua_State*, int n );
	};

	template<> inline bool ParmTraits<float>::Pop( type& t, lua_State* ls, int n )
	{
		if( lua_isnumber( ls, n ) == 0 )
			return false;

		t = static_cast< float >( lua_tonumber( ls, n ) );

		return true;
	}

	template<> inline void ParmTraits<float>::Push( const type t, lua_State* ls )
	{
		double d = static_cast< double >( t );

		lua_pushnumber( ls, d );

		return;
	}

	template<> inline bool ParmTraits<int>::Pop( type& t, lua_State* ls, int n )
	{
		if( lua_isnumber( ls, n ) == 0 )
			return false;

		t = static_cast< int >( lua_tonumber( ls, n ) );

		return true;
	}

	template<> inline void ParmTraits<int>::Push( const type t, lua_State* ls )
	{
		double d = static_cast< double >( t );

		lua_pushnumber( ls, d );

		return;
	}

	template<> inline bool ParmTraits<unsigned int>::Pop( type& t, lua_State* ls, int n )
	{
		if( lua_isnumber( ls, n ) == 0 )
			return false;

		t = static_cast< unsigned int >( lua_tonumber( ls, n ) );

		return true;
	}

	template<> inline void ParmTraits<unsigned int>::Push( const type t, lua_State* ls )
	{
		double d = static_cast< double >( t );

		lua_pushnumber( ls, d );

		return;
	}

	template<> inline bool ParmTraits<long>::Pop( type& t, lua_State* ls, int n )
	{
		if( lua_isnumber( ls, n ) == 0 )
			return false;

		t = static_cast< long >( lua_tonumber( ls, n ) );

		return true;
	}

	template<> inline void ParmTraits<long>::Push( const type t, lua_State* ls )
	{
		double d = static_cast< double >( t );

		lua_pushnumber( ls, d );

		return;
	}

	template<> inline bool ParmTraits<unsigned long>::Pop( type& t, lua_State* ls, int n )
	{
		if( lua_isnumber( ls, n ) == 0 )
			return false;

		t = static_cast< unsigned long >( lua_tonumber( ls, n ) );

		return true;
	}

	template<> inline void ParmTraits<unsigned long>::Push( const type t, lua_State* ls )
	{
		double d = static_cast< double >( t );

		lua_pushnumber( ls, d );

		return;
	}

	template<> inline bool ParmTraits<bool>::Pop( type& t, lua_State* ls, int n )
	{
		if( lua_isnumber( ls, n ) == 0 )
			return false;

		t = lua_tonumber( ls, n ) != 0;

		return true;
	}

	template<> inline void ParmTraits<bool>::Push( const type t, lua_State* ls )
	{
		double d = ( t )? 1.0 : 0.0;

		lua_pushnumber( ls, d );

		return;
	}

	template<> inline void ParmTraits<const char*>::Push( const type t, lua_State* ls )
	{
		lua_pushstring( ls, t );

		return;
	}

	template<> inline bool ParmTraits<const char*>::Pop( type& t, lua_State* ls, int n )
	{
		if( lua_isstring( ls, n ) == 0 )
			return false;

		t = lua_tostring( ls, n );

		return true;
	}

	///////////////////////////////////////////////////////////////////// 
	// IsVoid

	template< class T > class IsVoid       { public: typedef char	Ask; };
	template<>          class IsVoid<void> { public: typedef double	Ask; };

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal0

	template
		< 
		class R,
		class TYPE,
		class FUNC
		>
	class Functor0Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef FUNC type_func;

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	protected:
		Functor0Member( type_class* obj, type_func func )
			: m_obj( obj ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L )
		{
			// call function
			R r = (m_obj->*m_func)();

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)();

			// push result onto stack
				// no result

			return 0;
		}
	};

	template
		< 
		class R,
		class TYPE
		>
	class Functor0MemberNonConst : 
		public Functor0Member<R,TYPE,R (TYPE::*)()>
	{
	// construction
	public:
		Functor0MemberNonConst( type_class* obj, type_func func )
			: Functor0Member<R,TYPE,R (TYPE::*)()>( obj, func )
		{
		}
	};

	template
		< 
		class R,
		class TYPE
		>
	class Functor0MemberConst : 
		public Functor0Member<R,TYPE,R (TYPE::*)()const>
	{
	// construction
	public:
		Functor0MemberConst( type_class* obj, type_func func )
			: Functor0Member<R,TYPE,R (TYPE::*)()const>( obj, func )
		{
		}
	};

	template
		< 
		class R
		>
	class Functor0Free
	{
	// types
	public:
		typedef R (*type_func)();

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor0Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L )
		{
			// call function
			R r = (*m_func)();

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L )
		{
			UNREF_P( L );

			// call function
			(*m_func)();

			// push result onto stack
				// no result

			return 0;
		}
	};

	template
		< 
		class R,
		class FUNCTOR
		>
	class ObjInternal0 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR	m_func;
	
	// construction
	public:
		ObjInternal0( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 0 );

			// pop arguments off the stack
				// no arguments

			// delegate
			return m_func( IsVoid<R>::Ask(), L );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal1

	template
		< 
		class ARG1,
		class R,
		class TYPE,
		class FUNC
		>
	class Functor1Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef FUNC type_func;

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	protected:
		Functor1Member( type_class* obj, type_func func )
			: m_obj( obj ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1 )
		{
			// call function
			R r = (m_obj->*m_func)( p1 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class R,
		class TYPE
		>
	class Functor1MemberNonConst : 
		public Functor1Member<ARG1,R,TYPE,R (TYPE::*)(ARG1)>
	{
	// construction
	public:
		Functor1MemberNonConst( type_class* obj, type_func func )
			: Functor1Member<ARG1,R,TYPE,R (TYPE::*)(ARG1)>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class R,
		class TYPE
		>
	class Functor1MemberConst : 
		public Functor1Member<ARG1,R,TYPE,R (TYPE::*)(ARG1)const>
	{
	// construction
	public:
		Functor1MemberConst( type_class* obj, type_func func )
			: Functor1Member<ARG1,R,TYPE,R (TYPE::*)(ARG1)const>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class R
		>
	class Functor1Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor1Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1 )
		{
			// call function
			R r = (*m_func)( p1 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class R,
		class FUNCTOR
		>
	class ObjInternal1 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal1( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ), 
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 1 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal2

	template
		< 
		class ARG1,
		class ARG2,
		class R,
		class TYPE,
		class FUNC
		>
	class Functor2Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef FUNC type_func;

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	protected:
		Functor2Member( type_class* obj, type_func func )
			: m_obj( obj ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class R,
		class TYPE
		>
	class Functor2MemberNonConst : 
		public Functor2Member<ARG1,ARG2,R,TYPE,R (TYPE::*)(ARG1,ARG2)>
	{
	// construction
	public:
		Functor2MemberNonConst( type_class* obj, type_func func )
			: Functor2Member<ARG1,ARG2,R,TYPE,R (TYPE::*)(ARG1,ARG2)>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class R,
		class TYPE
		>
	class Functor2MemberConst : 
		public Functor2Member<ARG1,ARG2,R,TYPE,R (TYPE::*)(ARG1,ARG2)const>
	{
	// construction
	public:
		Functor2MemberConst( type_class* obj, type_func func )
			: Functor2Member<ARG1,ARG2,R,TYPE,R (TYPE::*)(ARG1,ARG2)const>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class R
		>
	class Functor2Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor2Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2 )
		{
			// call function
			R r = (*m_func)( p1, p2 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class R,
		class FUNCTOR
		>
	class ObjInternal2 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal2( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 2 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal3

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class R,
		class TYPE,
		class FUNC
		>
	class Functor3Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef FUNC type_func;

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	protected:
		Functor3Member( type_class* obj, type_func func )
			: m_obj( obj ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class R,
		class TYPE
		>
	class Functor3MemberNonConst : 
		public Functor3Member<ARG1,ARG2,ARG3,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3)>
	{
	// construction
	public:
		Functor3MemberNonConst( type_class* obj, type_func func )
			: Functor3Member<ARG1,ARG2,ARG3,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3)>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class R,
		class TYPE
		>
	class Functor3MemberConst : 
		public Functor3Member<ARG1,ARG2,ARG3,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3)const>
	{
	// construction
	public:
		Functor3MemberConst( type_class* obj, type_func func )
			: Functor3Member<ARG1,ARG2,ARG3,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3)const>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class R
		>
	class Functor3Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor3Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class R,
		class FUNCTOR
		>
	class ObjInternal3 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal3( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 3 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal4

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class R,
		class TYPE,
		class FUNC
		>
	class Functor4Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef FUNC type_func;

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	protected:
		Functor4Member( type_class* obj, type_func func )
			: m_obj( obj ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3, p4 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3, p4 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class R,
		class TYPE
		>
	class Functor4MemberNonConst :
		public Functor4Member<ARG1,ARG2,ARG3,ARG4,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4)>
	{
	// construction
	public:
		Functor4MemberNonConst( type_class* obj, type_func func )
			: Functor4Member<ARG1,ARG2,ARG3,ARG4,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4)>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class R,
		class TYPE
		>
	class Functor4MemberConst :
		public Functor4Member<ARG1,ARG2,ARG3,ARG4,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4)const>
	{
	// construction
	public:
		Functor4MemberConst( type_class* obj, type_func func )
			: Functor4Member<ARG1,ARG2,ARG3,ARG4,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4)const>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class R
		>
	class Functor4Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3,ARG4);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor4Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3, p4 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3, p4 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class R,
		class FUNCTOR
		>
	class ObjInternal4 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal4( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 4 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();
			ARG4 p4 = ARG4();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) ||
				!ParmTraits<ARG4>::Pop(p4,L,3+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3, p4 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal5

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class R,
		class TYPE,
		class FUNC
		>
	class Functor5Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef FUNC type_func;

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	protected:
		Functor5Member( type_class* obj, type_func func )
			: m_obj( obj ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3, p4, p5 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3, p4, p5 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class R,
		class TYPE
		>
	class Functor5MemberNonConst : 
		public Functor5Member<ARG1,ARG2,ARG3,ARG4,ARG5,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4,ARG5)>
	{
	// construction
	public:
		Functor5MemberNonConst( type_class* obj, type_func func )
			: Functor5Member<ARG1,ARG2,ARG3,ARG4,ARG5,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4,ARG5)>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class R,
		class TYPE
		>
	class Functor5MemberConst : 
		public Functor5Member<ARG1,ARG2,ARG3,ARG4,ARG5,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4,ARG5)const>
	{
	// construction
	public:
		Functor5MemberConst( type_class* obj, type_func func )
			: Functor5Member<ARG1,ARG2,ARG3,ARG4,ARG5,R,TYPE,R (TYPE::*)(ARG1,ARG2,ARG3,ARG4,ARG5)const>( obj, func )
		{
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class R
		>
	class Functor5Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor5Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3, p4, p5 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3, p4, p5 );

			// push result onto stack
				// no result

			return 0;
		}
	};

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class R,
		class FUNCTOR
		>
	class ObjInternal5 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal5( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 5 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();
			ARG4 p4 = ARG4();
			ARG5 p5 = ARG5();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) ||
				!ParmTraits<ARG4>::Pop(p4,L,3+1) ||
				!ParmTraits<ARG5>::Pop(p5,L,4+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3, p4, p5 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal6

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class R,
		class TYPE
		>
	class Functor6Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef R (TYPE::*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6);

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	public:
		Functor6Member( type_class* obj, type_func func )
			: m_obj( obj ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3, p4, p5, p6 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3, p4, p5, p6 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class R
		>
	class Functor6Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor6Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3, p4, p5, p6 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3, p4, p5, p6 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class R,
		class FUNCTOR
		>
	class ObjInternal6 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal6( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 6 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();
			ARG4 p4 = ARG4();
			ARG5 p5 = ARG5();
			ARG6 p6 = ARG6();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) ||
				!ParmTraits<ARG4>::Pop(p4,L,3+1) ||
				!ParmTraits<ARG5>::Pop(p5,L,4+1) ||
				!ParmTraits<ARG6>::Pop(p6,L,5+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3, p4, p5, p6 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal7

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class R,
		class TYPE
		>
	class Functor7Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef R (TYPE::*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7);

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	public:
		Functor7Member( type_class* obj, type_func func )
			: m_obj( obj  ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class R
		>
	class Functor7Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor7Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3, p4, p5, p6, p7 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3, p4, p5, p6, p7 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class R,
		class FUNCTOR
		>
	class ObjInternal7 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal7( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 7 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();
			ARG4 p4 = ARG4();
			ARG5 p5 = ARG5();
			ARG6 p6 = ARG6();
			ARG7 p7 = ARG7();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) ||
				!ParmTraits<ARG4>::Pop(p4,L,3+1) ||
				!ParmTraits<ARG5>::Pop(p5,L,4+1) ||
				!ParmTraits<ARG6>::Pop(p6,L,5+1) ||
				!ParmTraits<ARG7>::Pop(p7,L,6+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3, p4, p5, p6, p7 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal8

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class R,
		class TYPE
		>
	class Functor8Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef R (TYPE::*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8);

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	public:
		Functor8Member( type_class* obj, type_func func )
			: m_obj( obj  ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7, p8 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7, p8 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class R
		>
	class Functor8Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor8Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3, p4, p5, p6, p7, p8 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3, p4, p5, p6, p7, p8 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class R,
		class FUNCTOR
		>
	class ObjInternal8 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal8( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 8 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();
			ARG4 p4 = ARG4();
			ARG5 p5 = ARG5();
			ARG6 p6 = ARG6();
			ARG7 p7 = ARG7();
			ARG8 p8 = ARG8();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) ||
				!ParmTraits<ARG4>::Pop(p4,L,3+1) ||
				!ParmTraits<ARG5>::Pop(p5,L,4+1) ||
				!ParmTraits<ARG6>::Pop(p6,L,5+1) ||
				!ParmTraits<ARG7>::Pop(p7,L,6+1) ||
				!ParmTraits<ARG8>::Pop(p8,L,7+1))
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3, p4, p5, p6, p7, p8 );
		}
	};	

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal9

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class ARG9,
		class R,
		class TYPE
		>
	class Functor9Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef R (TYPE::*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9);

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	public:
		Functor9Member( type_class* obj, type_func func )
			: m_obj( obj  ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class ARG9,
		class R
		>
	class Functor9Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor9Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class ARG9,
		class R,
		class FUNCTOR
		>
	class ObjInternal9 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal9( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 9 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();
			ARG4 p4 = ARG4();
			ARG5 p5 = ARG5();
			ARG6 p6 = ARG6();
			ARG7 p7 = ARG7();
			ARG8 p8 = ARG8();
			ARG9 p9 = ARG9();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) ||
				!ParmTraits<ARG4>::Pop(p4,L,3+1) ||
				!ParmTraits<ARG5>::Pop(p5,L,4+1) ||
				!ParmTraits<ARG6>::Pop(p6,L,5+1) ||
				!ParmTraits<ARG7>::Pop(p7,L,6+1) ||
				!ParmTraits<ARG8>::Pop(p8,L,7+1) ||
				!ParmTraits<ARG9>::Pop(p9,L,8+1))
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
		}
	};

	///////////////////////////////////////////////////////////////////// 
	// ObjInternal10

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class ARG9,
		class ARG10,
		class R,
		class TYPE
		>
	class Functor10Member
	{
	// types
	public:
		typedef TYPE type_class;
		typedef R (TYPE::*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10);

	// fields
	private:
		type_class*	m_obj;
		type_func	m_func;
	
	// construction
	public:
		Functor10Member( type_class* obj, type_func func )
			: m_obj( obj  ), m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9, ARG10 p10 )
		{
			// call function
			R r = (m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9, ARG10 p10 )
		{
			UNREF_P( L );

			// call function
			(m_obj->*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class ARG9,
		class ARG10,
		class R
		>
	class Functor10Free
	{
	// types
	public:
		typedef R (*type_func)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10);

	// fields
	private:
		type_func	m_func;
	
	// construction
	public:
		Functor10Free( type_func func )
			: m_func( func )
		{
		}

	// interface
	public:
		inline int operator()( const char&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9, ARG10 p10 )
		{
			// call function
			R r = (*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );

			// push result onto stack
			ParmTraits<R>::Push( r, L );

			return 1;
		}

		inline int operator()( const double&, lua_State* L, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8, ARG9 p9, ARG10 p10 )
		{
			UNREF_P( L );

			// call function
			(*m_func)( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );

			// push result onto stack
				// no result

			return 0;
		}
	};	

	template
		< 
		class ARG1,
		class ARG2,
		class ARG3,
		class ARG4,
		class ARG5,
		class ARG6,
		class ARG7,
		class ARG8,
		class ARG9,
		class ARG10,
		class R,
		class FUNCTOR
		>
	class ObjInternal10 : public ObjInternalBase
	{
	// fields
	private:
		FUNCTOR m_func;
	
	// construction
	public:
		ObjInternal10( LuaConfig* lc, const char* funcname, const char* usage, const FUNCTOR& func )
			: ObjInternalBase( lc, funcname, usage ),
			  m_func( func )
		{
		}

	// inherited 
	public:
		virtual int	Dispatch( lua_State* L )
		{
			// check number of arguments
			CheckArgs( L, 10 );

			// pop arguments off the stack
			ARG1 p1 = ARG1();
			ARG2 p2 = ARG2();
			ARG3 p3 = ARG3();
			ARG4 p4 = ARG4();
			ARG5 p5 = ARG5();
			ARG6 p6 = ARG6();
			ARG7 p7 = ARG7();
			ARG8 p8 = ARG8();
			ARG9 p9 = ARG9();
			ARG10 p10 = ARG10();

			if( !ParmTraits<ARG1>::Pop(p1,L,0+1) ||
				!ParmTraits<ARG2>::Pop(p2,L,1+1) ||
				!ParmTraits<ARG3>::Pop(p3,L,2+1) ||
				!ParmTraits<ARG4>::Pop(p4,L,3+1) ||
				!ParmTraits<ARG5>::Pop(p5,L,4+1) ||
				!ParmTraits<ARG6>::Pop(p6,L,5+1) ||
				!ParmTraits<ARG7>::Pop(p7,L,6+1) ||
				!ParmTraits<ARG8>::Pop(p8,L,7+1) ||
				!ParmTraits<ARG9>::Pop(p9,L,8+1) ||
				!ParmTraits<ARG10>::Pop(p10,L,9+1) )
			{
				// lua error
				lua_error( L, GetFunctionUsage() );
				return 0;
			}

			// delegate
			return m_func( IsVoid<R>::Ask(), L, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 );
		}
	};
	///////////////////////////////////////////////////////////////////// 
	// adapter functions

	template< class TYPE, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(), const char* u = "" )
	{
		typedef Functor0MemberNonConst<R,TYPE> T;
		return Obj( new ObjInternal0<R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)()const, const char* u = "" )
	{
		typedef Functor0MemberConst<R,TYPE> T;
		return Obj( new ObjInternal0<R,T>( lc, n, u, T(t, f) ) );
	}

	template< class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(), const char* u = "" )
	{
		typedef Functor0Free<R> T;
		return Obj( new ObjInternal0<R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1), const char* u = "" )
	{
		typedef Functor1MemberNonConst<ARG1,R,TYPE> T;
		return Obj( new ObjInternal1<ARG1,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1)const, const char* u = "" )
	{
		typedef Functor1MemberConst<ARG1,R,TYPE> T;
		return Obj( new ObjInternal1<ARG1,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1), const char* u = "" )
	{
		typedef Functor1Free<ARG1,R> T;
		return Obj( new ObjInternal1<ARG1,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2), const char* u = "" )
	{
		typedef Functor2MemberNonConst<ARG1,ARG2,R,TYPE> T;
		return Obj( new ObjInternal2<ARG1,ARG2,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2)const, const char* u = "" )
	{
		typedef Functor2MemberConst<ARG1,ARG2,R,TYPE> T;
		return Obj( new ObjInternal2<ARG1,ARG2,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2), const char* u = "" )
	{
		typedef Functor2Free<ARG1,ARG2,R> T;
		return Obj( new ObjInternal2<ARG1,ARG2,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3), const char* u = "" )
	{
		typedef Functor3MemberNonConst<ARG1,ARG2,ARG3,R,TYPE> T;
		return Obj( new ObjInternal3<ARG1,ARG2,ARG3,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3)const, const char* u = "" )
	{
		typedef Functor3MemberConst<ARG1,ARG2,ARG3,R,TYPE> T;
		return Obj( new ObjInternal3<ARG1,ARG2,ARG3,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3), const char* u = "" )
	{
		typedef Functor3Free<ARG1,ARG2,ARG3,R> T;
		return Obj( new ObjInternal3<ARG1,ARG2,ARG3,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4), const char* u = "" )
	{
		typedef Functor4MemberNonConst<ARG1,ARG2,ARG3,ARG4,R,TYPE> T;
		return Obj( new ObjInternal4<ARG1,ARG2,ARG3,ARG4,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4)const, const char* u = "" )
	{
		typedef Functor4MemberConst<ARG1,ARG2,ARG3,ARG4,R,TYPE> T;
		return Obj( new ObjInternal4<ARG1,ARG2,ARG3,ARG4,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class ARG4, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3,ARG4), const char* u = "" )
	{
		typedef Functor4Free<ARG1,ARG2,ARG3,ARG4,R> T;
		return Obj( new ObjInternal4<ARG1,ARG2,ARG3,ARG4,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5), const char* u = "" )
	{
		typedef Functor5MemberNonConst<ARG1,ARG2,ARG3,ARG4,ARG5,R,TYPE> T;
		return Obj( new ObjInternal5<ARG1,ARG2,ARG3,ARG4,ARG5,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5)const, const char* u = "" )
	{
		typedef Functor5MemberConst<ARG1,ARG2,ARG3,ARG4,ARG5,R,TYPE> T;
		return Obj( new ObjInternal5<ARG1,ARG2,ARG3,ARG4,ARG5,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3,ARG4,ARG5), const char* u = "" )
	{
		typedef Functor5Free<ARG1,ARG2,ARG3,ARG4,ARG5,R> T;
		return Obj( new ObjInternal5<ARG1,ARG2,ARG3,ARG4,ARG5,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6), const char* u = "" )
	{
		typedef Functor6Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,R,TYPE> T;
		return Obj( new ObjInternal6<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6) const, const char* u = "" )
	{
		typedef Functor6Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,R,TYPE> T;
		return Obj( new ObjInternal6<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6), const char* u = "" )
	{
		typedef Functor6Free<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,R> T;
		return Obj( new ObjInternal6<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7), const char* u = "" )
	{
		typedef Functor7Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,R,TYPE> T;
		return Obj( new ObjInternal7<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7) const, const char* u = "" )
	{
		typedef Functor7Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,R,TYPE> T;
		return Obj( new ObjInternal7<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7), const char* u = "" )
	{
		typedef Functor7Free<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,R> T;
		return Obj( new ObjInternal7<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8), const char* u = "" )
	{
		typedef Functor8Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,R,TYPE> T;
		return Obj( new ObjInternal8<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8) const, const char* u = "" )
	{
		typedef Functor8Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,R,TYPE> T;
		return Obj( new ObjInternal8<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8), const char* u = "" )
	{
		typedef Functor8Free<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,R> T;
		return Obj( new ObjInternal8<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9), const char* u = "" )
	{
		typedef Functor9Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,R,TYPE> T;
		return Obj( new ObjInternal9<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9) const, const char* u = "" )
	{
		typedef Functor9Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,R,TYPE> T;
		return Obj( new ObjInternal9<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9), const char* u = "" )
	{
		typedef Functor9Free<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,R> T;
		return Obj( new ObjInternal9<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,R,T>( lc, n, u, T(f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class ARG10, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10), const char* u = "" )
	{
		typedef Functor10Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,R,TYPE> T;
		return Obj( new ObjInternal10<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class TYPE, class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class ARG10, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, TYPE* t, R (TYPE::*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10) const, const char* u = "" )
	{
		typedef Functor10Member<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,R,TYPE> T;
		return Obj( new ObjInternal10<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,R,T>( lc, n, u, T(t, f) ) );
	}

	template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8, class ARG9, class ARG10, class R >
	inline Obj Bind( LuaConfig* lc, const char* n, R (*f)(ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10), const char* u = "" )
	{
		typedef Functor10Free<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,R> T;
		return Obj( new ObjInternal10<ARG1,ARG2,ARG3,ARG4,ARG5,ARG6,ARG7,ARG8,ARG9,ARG10,R,T>( lc, n, u, T(f) ) );
	}

	///////////////////////////////////////////////////////////////////// 
	// CallReturn

	template< class T > class CallReturn
	{
	// types
	public:
		enum { RETCOUNT = 1 };
	
	// fields
	private:
		T m_v;

	// construction
	public:
		CallReturn()
			: m_v( T() )
		{
		}

	// interface
	public:
		bool Store( lua_State* L )
		{
			return ParmTraits<T>::Pop(m_v,L,-1);
		}

		const T& Retrieve() const
		{
			return m_v;
		}
	};

	template<> class CallReturn<void>
	{
	// types
	public:
		enum { RETCOUNT = 0 };
	
	// construction
	public:
		CallReturn()
		{
		}

	// interface
	public:
		bool Store( lua_State* L )
		{
			UNREF_P( L );

			return true;
		}
	};

	///////////////////////////////////////////////////////////////////// 
	// Call

	template< class R >
	class Call
	{
	// fields
	public:
		CallReturn<R>	r;

	// construction
	public:
		 Call()
		{
		}

		~Call()
		{
		}

	// interface
	public:
		bool Execute( LuaConfig* lc, const char* luafunc )
		{
			// 
			bool ok = true;
			
			//
			lua_State* L = lc->GetState();

			//
			const int top = lua_gettop( L );

			// retrieve the function
			if( ok )
			{
				lua_getglobal( L, luafunc );

				ok = lua_type( L, -1 ) == LUA_TFUNCTION;
			}

			// call the function
			if( ok )
			{
				ok = ( lua_call( L, 0, r.RETCOUNT ) == 0 );
			}

			// retrieve return value
			if( ok )
			{
				ok = r.Store( L );
			}

			// return stack position
			lua_settop( L, top );

			//
			return ok;
		}

		template< class ARG1 >
		bool Execute( LuaConfig* lc, const char* luafunc, ARG1 p1 )
		{
			// 
			bool ok = true;
			
			//
			lua_State* L = lc->GetState();

			//
			const int top = lua_gettop( L );

			// retrieve the function
			if( ok )
			{
				lua_getglobal( L, luafunc );

				ok = lua_type( L, -1 ) == LUA_TFUNCTION;
			}

			// push the args
			if( ok )
			{
				ParmTraits<ARG1>::Push( p1, L );
			}

			// call the function
			if( ok )
			{
				ok = ( lua_call( L, 1, r.RETCOUNT ) == 0 );
			}

			// retrieve return value
			if( ok )
			{
				ok = r.Store( L );
			}

			// return stack position
			lua_settop( L, top );

			//
			return ok;
		}

		template< class ARG1, class ARG2 >
		bool Execute( LuaConfig* lc, const char* luafunc, ARG1 p1, ARG2 p2 )
		{
			// 
			bool ok = true;
			
			//
			lua_State* L = lc->GetState();

			//
			const int top = lua_gettop( L );

			// retrieve the function
			if( ok )
			{
				lua_getglobal( L, luafunc );

				ok = lua_type( L, -1 ) == LUA_TFUNCTION;
			}

			// push the args
			if( ok )
			{
				ParmTraits<ARG1>::Push( p1, L );
				ParmTraits<ARG2>::Push( p2, L );
			}

			// call the function
			if( ok )
			{
				ok = ( lua_call( L, 2, r.RETCOUNT ) == 0 );
			}

			// retrieve return value
			if( ok )
			{
				ok = r.Store( L );
			}

			// return stack position
			lua_settop( L, top );

			//
			return ok;
		}

		template< class ARG1, class ARG2, class ARG3 >
		bool Execute( LuaConfig* lc, const char* luafunc, ARG1 p1, ARG2 p2, ARG3 p3 )
		{
			// 
			bool ok = true;
			
			//
			lua_State* L = lc->GetState();

			//
			const int top = lua_gettop( L );

			// retrieve the function
			if( ok )
			{
				lua_getglobal( L, luafunc );

				ok = lua_type( L, -1 ) == LUA_TFUNCTION;
			}

			// push the args
			if( ok )
			{
				ParmTraits<ARG1>::Push( p1, L );
				ParmTraits<ARG2>::Push( p2, L );
				ParmTraits<ARG3>::Push( p3, L );
			}

			// call the function
			if( ok )
			{
				ok = ( lua_call( L, 3, r.RETCOUNT ) == 0 );
			}

			// retrieve return value
			if( ok )
			{
				ok = r.Store( L );
			}

			// return stack position
			lua_settop( L, top );

			//
			return ok;
		}

		template< class ARG1, class ARG2, class ARG3, class ARG4 >
		bool Execute( LuaConfig* lc, const char* luafunc, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4 )
		{
			// 
			bool ok = true;
			
			//
			lua_State* L = lc->GetState();

			//
			const int top = lua_gettop( L );

			// retrieve the function
			if( ok )
			{
				lua_getglobal( L, luafunc );

				ok = lua_type( L, -1 ) == LUA_TFUNCTION;
			}

			// push the args
			if( ok )
			{
				ParmTraits<ARG1>::Push( p1, L );
				ParmTraits<ARG2>::Push( p2, L );
				ParmTraits<ARG3>::Push( p3, L );
				ParmTraits<ARG4>::Push( p4, L );
			}

			// call the function
			if( ok )
			{
				ok = ( lua_call( L, 4, r.RETCOUNT ) == 0 );
			}

			// retrieve return value
			if( ok )
			{
				ok = r.Store( L );
			}

			// return stack position
			lua_settop( L, top );

			//
			return ok;
		}

		template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5 >
		bool Execute( LuaConfig* lc, const char* luafunc, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5 )
		{
			// 
			bool ok = true;
			
			//
			lua_State* L = lc->GetState();

			//
			const int top = lua_gettop( L );

			// retrieve the function
			if( ok )
			{
				lua_getglobal( L, luafunc );

				ok = lua_type( L, -1 ) == LUA_TFUNCTION;
			}

			// push the args
			if( ok )
			{
				ParmTraits<ARG1>::Push( p1, L );
				ParmTraits<ARG2>::Push( p2, L );
				ParmTraits<ARG3>::Push( p3, L );
				ParmTraits<ARG4>::Push( p4, L );
				ParmTraits<ARG5>::Push( p5, L );
			}

			// call the function
			if( ok )
			{
				ok = ( lua_call( L, 5, r.RETCOUNT ) == 0 );
			}

			// retrieve return value
			if( ok )
			{
				ok = r.Store( L );
			}

			// return stack position
			lua_settop( L, top );

			//
			return ok;
		}

		template< class ARG1, class ARG2, class ARG3, class ARG4, class ARG5, class ARG6, class ARG7, class ARG8 >
		bool Execute( LuaConfig* lc, const char* luafunc, ARG1 p1, ARG2 p2, ARG3 p3, ARG4 p4, ARG5 p5, ARG6 p6, ARG7 p7, ARG8 p8 )
		{
			// 
			bool ok = true;
			
			//
			lua_State* L = lc->GetState();

			//
			const int top = lua_gettop( L );

			// retrieve the function
			if( ok )
			{
				lua_getglobal( L, luafunc );

				ok = lua_type( L, -1 ) == LUA_TFUNCTION;
			}

			// push the args
			if( ok )
			{
				ParmTraits<ARG1>::Push( p1, L );
				ParmTraits<ARG2>::Push( p2, L );
				ParmTraits<ARG3>::Push( p3, L );
				ParmTraits<ARG4>::Push( p4, L );
				ParmTraits<ARG5>::Push( p5, L );
				ParmTraits<ARG6>::Push( p6, L );
				ParmTraits<ARG7>::Push( p7, L );
				ParmTraits<ARG8>::Push( p8, L );
			}

			// call the function
			if( ok )
			{
				ok = ( lua_call( L, 8, r.RETCOUNT ) == 0 );
			}

			// retrieve return value
			if( ok )
			{
				ok = r.Store( L );
			}

			// return stack position
			lua_settop( L, top );

			//
			return ok;
		}
	};
}
