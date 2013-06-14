//
//  helpers.h
//
//  Created by Arthur Langereis on 2/24/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#ifndef INQUISITION_HELPERS_INCLUDED
#define INQUISITION_HELPERS_INCLUDED

#include <string>
#include <type_traits>

namespace Inquisition {
	template<typename T>
	std::string to_string(const T t, typename std::enable_if<std::is_arithmetic<T>::value >::type* = nullptr) {
		return std::to_string(t);
	}

	std::string to_string(const bool b);
	
	std::string to_string(const char * cs);
	std::string to_string(std::string s);
}

#endif /* defined(INQUISITION_HELPERS_INCLUDED) */
