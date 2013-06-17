//
//  checks.hpp
//  Inquisition
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#ifndef INQUISITION_CHECKS_INCLUDED
#define INQUISITION_CHECKS_INCLUDED

#include <string>
#include "TestReport.h"
#include "helpers.h"

namespace Inquisition {
	template <typename Expr>
	bool checkImpl(Expr expr, const std::string & failMsg) {
		bool success = expr();
		if (success)
			detail::currentReport()->pass();
		else
			detail::currentReport()->failure(failMsg);
		return success;
	}
	
	
	bool check_true(bool expr);
	bool check_false(bool expr);
	
	template <typename T, typename U>
	bool check_equal(const T & t, const U & u) {
		return checkImpl([=] { return t == u; }, to_string(t) + " was expected to be equal to " + to_string(u));
	}
	
	template <typename T, typename U>
	bool check_not_equal(const T & t, const U & u) {
		return checkImpl([=] { return t != u; }, to_string(t) + " was expected to differ from " + to_string(u));
	}

	template <typename T, typename U>
	bool check_gt(const T & t, const U & u) {
		return checkImpl([=] { return t > u; }, to_string(t) + " was expected to be greater than " + to_string(u));
	}
	
	template <typename T, typename U>
	bool check_ge(const T & t, const U & u) {
		return checkImpl([=] { return t >= u; }, to_string(t) + " was expected to be greater than or equal to " + to_string(u));
	}

	template <typename T, typename U>
	bool check_lt(const T & t, const U & u) {
		return checkImpl([=] { return t < u; }, to_string(t) + " was expected to be less than " + to_string(u));
	}
	
	template <typename T, typename U>
	bool check_le(const T & t, const U & u) {
		return checkImpl([=] { return t <= u; }, to_string(t) + " was expected to be less than or equal to " + to_string(u));
	}

	// C-style string overloads
	bool check_equal(const char * lhs, const char * rhs);
	bool check_not_equal(const char * lhs, const char * rhs);
	bool check_gt(const char * lhs, const char * rhs);
	bool check_ge(const char * lhs, const char * rhs);
	bool check_lt(const char * lhs, const char * rhs);
	bool check_le(const char * lhs, const char * rhs);
}


#endif
