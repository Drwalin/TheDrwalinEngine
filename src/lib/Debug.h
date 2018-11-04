
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

template < class T >
inline std::string methodName( const std::string& prettyFunction )
{
    size_t colons = prettyFunction.find("::");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;

    return prettyFunction.substr(begin,end);
}
#define __METHOD_NAME__ (methodName<int>(__PRETTY_FUNCTION__))

#define DEBUG( x ) std::cerr << "\n " << __METHOD_NAME__ << "(" << (x) << ")";
#define DEBUG_( x ) std::cerr << "\n " << __PRETTY_FUNCTION__ << "(" << (x) << ")";

#endif

