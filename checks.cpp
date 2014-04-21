//
//  checks.cpp
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include "checks.h"

namespace Inquisition {
	bool checkTrue(bool expr) { return checkImpl([=] { return expr; }, "expression was expected to be true"); }
	bool checkFalse(bool expr) { return checkImpl([=] { return !expr; }, "expression was expected to be false"); }
	
	// floating point near-equality comparisons (kept high for operand result error)
	namespace detail {
		template <>
		float epsilon<float>() { return 5.0e-7f; }
		
		template <>
		double epsilon<double>() { return 2.22e-16; }
	}

	// C-style string overloads for comparisons, defer to the std::string implementation
	
	bool checkEqual(const char * lhs, const char * rhs) {
		return checkEqual(std::string { lhs }, rhs);
	}
	
	bool checkNotEqual(const char * lhs, const char * rhs) {
		return checkNotEqual(std::string { lhs }, rhs);
	}
	
	bool checkGT(const char * lhs, const char * rhs) {
		return checkGT(std::string { lhs }, rhs);
	}
	
	bool checkGE(const char * lhs, const char * rhs) {
		return checkGE(std::string { lhs }, rhs);
	}
	
	bool checkLT(const char * lhs, const char * rhs) {
		return checkLT(std::string { lhs }, rhs);
	}
	
	bool checkLE(const char * lhs, const char * rhs) {
		return checkLE(std::string { lhs }, rhs);
	}
}
