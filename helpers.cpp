//
//  helpers.cpp
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include "helpers.h"

namespace Inquisition {
	std::string to_string(const bool b) {
		return b ? "true" : "false";
	}

	std::string to_string(const char * cs) {
		return cs;
	}
	
	std::string to_string(std::string s) {
		return s;
	}
}
