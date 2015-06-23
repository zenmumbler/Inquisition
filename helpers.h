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
	// defer numerical types to std
	template<typename T>
	std::string toString(const T t, typename std::enable_if_t<std::is_arithmetic<T>::value>* = nullptr) {
		return std::to_string(t);
	}

	// Container types and built-in arrays
	template <typename C>
	// requires Container<C>
	std::string toString(const C& ct, decltype(std::begin(ct))* = nullptr) {
		using T = decltype(*std::begin(ct));
		
		bool rest = false;
		std::string s {'[', 1};
		for (const T& t : ct) {
			if (rest)
				s += ',';
			else
				rest = true;
			s += toString(t);
		}
		return s + ']';
	}


	// pointers
	std::string toString(std::nullptr_t);

	template <typename T>
	std::string toString(const T* p) {
		return p ? std::string("(Pointer)", 9) : std::string("nullptr", 7);
	}


	std::string toString(const bool b);
	
	std::string toString(const char * cs);
	std::string toString(std::string s);
}

#endif /* defined(INQUISITION_HELPERS_INCLUDED) */
