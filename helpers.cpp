//
//  helpers.cpp
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include "helpers.h"

namespace Inquisition {
	std::string toString(std::nullptr_t) {
		return std::string("nullptr", 7);
	}

	std::string toString(const bool b) {
		return b ? "true" : "false";
	}

	std::string toString(const char * cs) {
		return cs;
	}
	
	std::string toString(std::string s) {
		return s;
	}
}
