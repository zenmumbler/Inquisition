//
//  checks.cpp
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include "checks.h"

namespace Inquisition {
	bool check_true(bool expr) { return checkImpl([=] { return expr; }, "expression was expected to be true"); }
	bool check_false(bool expr) { return checkImpl([=] { return !expr; }, "expression was expected to be false"); }

	// C-style string overloads for comparisons, defer to the std::string implementation
	
	bool check_equal(const char * lhs, const char * rhs) {
		return check_equal(std::string { lhs }, rhs);
	}
	
	bool check_not_equal(const char * lhs, const char * rhs) {
		return check_not_equal(std::string { lhs }, rhs);
	}
	
	bool check_gt(const char * lhs, const char * rhs) {
		return check_gt(std::string { lhs }, rhs);
	}
	
	bool check_ge(const char * lhs, const char * rhs) {
		return check_ge(std::string { lhs }, rhs);
	}
	
	bool check_lt(const char * lhs, const char * rhs) {
		return check_lt(std::string { lhs }, rhs);
	}
	
	bool check_le(const char * lhs, const char * rhs) {
		return check_le(std::string { lhs }, rhs);
	}
}
