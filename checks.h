//
//  checks.hpp
//  Inquisition
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#ifndef INQUISITION_CHECKS_INCLUDED
#define INQUISITION_CHECKS_INCLUDED

#include "TestReport.h"
#include "helpers.h"
#include <string>
#include <cmath>

namespace Inquisition {
	namespace detail {
		template <typename F>
		F epsilon();
	}

	template <typename Expr>
	bool checkImpl(Expr expr, const std::string & failMsg) {
		bool success = expr();
		if (success)
			detail::currentReport()->pass();
		else
			detail::currentReport()->failure(failMsg);
		return success;
	}
	
	
	bool checkTrue(bool expr);
	bool checkFalse(bool expr);
	
	template <typename T, typename U>
	bool checkEqual(const T & t, const U & u) {
		return checkImpl([=] { return t == u; }, toString(t) + " is not equal to " + toString(u));
	}

	template <typename F>
	bool checkNearEqual(F f1, F f2) {
		return checkImpl([=] {
			return std::abs(f2 - f1) <= detail::epsilon<F>();
		}, toString(f1) + " is not mostly equal to " + toString(f2));
	}
	
	template <typename F>
	bool checkNotNearEqual(F f1, F f2) {
		return checkImpl([=] {
			return std::abs(f2 - f1) > detail::epsilon<F>();
		}, toString(f1) + " is too similar to " + toString(f2));
	}
	
	template <typename T, typename U>
	bool checkNotEqual(const T & t, const U & u) {
		return checkImpl([=] { return t != u; }, toString(t) + " was expected to differ from " + toString(u));
	}

	template <typename T, typename U>
	bool checkGT(const T & t, const U & u) {
		return checkImpl([=] { return t > u; }, toString(t) + " was expected to be greater than " + toString(u));
	}
	
	template <typename T, typename U>
	bool checkGE(const T & t, const U & u) {
		return checkImpl([=] { return t >= u; }, toString(t) + " was expected to be greater than or equal to " + toString(u));
	}

	template <typename T, typename U>
	bool checkLT(const T & t, const U & u) {
		return checkImpl([=] { return t < u; }, toString(t) + " was expected to be less than " + toString(u));
	}
	
	template <typename T, typename U>
	bool checkLE(const T & t, const U & u) {
		return checkImpl([=] { return t <= u; }, toString(t) + " was expected to be less than or equal to " + toString(u));
	}

	// C-style string overloads
	bool checkEqual(const char * lhs, const char * rhs);
	bool checkNotEqual(const char * lhs, const char * rhs);
	bool checkGT(const char * lhs, const char * rhs);
	bool checkGE(const char * lhs, const char * rhs);
	bool checkLT(const char * lhs, const char * rhs);
	bool checkLE(const char * lhs, const char * rhs);
}


#endif
