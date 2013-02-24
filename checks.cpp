//
//  checks.cpp
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include "checks.h"

namespace Inquisition {
	void check_true(bool expr) { checkImpl([=] { return expr; }, "expression was expected to be true"); }
	void check_false(bool expr) { checkImpl([=] { return !expr; }, "expression was expected to be false"); }

	// C-style string overloads for comparisons, defer to the std::string implementation
	
	void check_equal(const char * lhs, const char * rhs) {
		check_equal(std::string { lhs }, rhs);
	}
	
	void check_not_equal(const char * lhs, const char * rhs) {
		check_not_equal(std::string { lhs }, rhs);
	}
	
	void check_gt(const char * lhs, const char * rhs) {
		check_gt(std::string { lhs }, rhs);
	}
	
	void check_ge(const char * lhs, const char * rhs) {
		check_ge(std::string { lhs }, rhs);
	}
	
	void check_lt(const char * lhs, const char * rhs) {
		check_lt(std::string { lhs }, rhs);
	}
	
	void check_le(const char * lhs, const char * rhs) {
		check_le(std::string { lhs }, rhs);
	}
}
