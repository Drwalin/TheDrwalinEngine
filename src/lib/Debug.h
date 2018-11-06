
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef ENGINE_CPP
	int UseDebugVaraibleStorage = 0;
#else
	extern "C" int UseDebugVaraibleStorage;
#endif

namespace Debug
{
	inline void UseDebug( bool enable )
	{
		UseDebugVaraibleStorage = enable ? 1 : 0;
	}
	inline void SwitchDebugState()
	{
		UseDebugVaraibleStorage = UseDebugVaraibleStorage ? 0 : 1;
	}
	inline bool GetDebug()
	{
		return UseDebugVaraibleStorage;
	}
};

template < class T >
inline std::string MethodName( const std::string& prettyFunction )
{
	std::string ret = prettyFunction;
	size_t index = 0;
	
	while( true )
	{
		index = ret.find( "::__cxx11" );
		if( index == std::string::npos )
			break;
		ret.replace( index, 9, "" );
	}
	
	index = ret.find( " [" );
	if( index != std::string::npos )
		ret.resize( index );
	
	return ret;
	
	
	/*
    size_t colons = prettyFunction.find("[");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;

    return prettyFunction.substr(begin,end);
    */
}
#define __METHOD_NAME__ (MethodName<int>(__PRETTY_FUNCTION__))

#define DEBUG( x ) { if( UseDebugVaraibleStorage ) std::cerr << "\n " << __METHOD_NAME__ << "(" << (x) << ")"; }
#define MESSAGE( x ) { std::cerr << "\n " << __METHOD_NAME__ << "( " << (x) << " )"; }

#endif

