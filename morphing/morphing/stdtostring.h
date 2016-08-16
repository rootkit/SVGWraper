//
//  stdtostring.h
//  morphing
//
//  Created by xyz on 16/8/16.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef morphing_stdtostring_h
#define morphing_stdtostring_h

#include <string>
#include <sstream>

using namespace std;

namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


#endif
