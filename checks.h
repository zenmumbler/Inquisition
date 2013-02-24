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

namespace Inquisition {
	template <typename Expr>
	void checkImpl(Expr expr, const std::string & failMsg) {
		if (expr())
			detail::currentReport()->pass();
		else
			detail::currentReport()->failure(failMsg);
	}
	
	
	void check_true(bool expr);
	void check_false(bool expr);
	
	template <typename T, typename U>
	void check_equal(const T & t, const U & u) {
		checkImpl([=] { return t == u; }, std::to_string(t) + " was expected to be equal to " + std::to_string(u));
	}
	
	template <typename T, typename U>
	void check_not_equal(const T & t, const U & u) {
		checkImpl([=] { return t != u; }, std::to_string(t) + " was expected to differ from " + std::to_string(u));
	}
	
	template <typename T, typename U>
	void check_gt(const T & t, const U & u) {
		checkImpl([=] { return t > u; }, std::to_string(t) + " was expected to be greater than " + std::to_string(u));
	}
	
	template <typename T, typename U>
	void check_ge(const T & t, const U & u) {
		checkImpl([=] { return t >= u; }, std::to_string(t) + " was expected to be greater than or equal to " + std::to_string(u));
	}
	
	template <typename T, typename U>
	void check_lt(const T & t, const U & u) {
		checkImpl([=] { return t < u; }, std::to_string(t) + " was expected to be less than " + std::to_string(u));
	}
	
	template <typename T, typename U>
	void check_le(const T & t, const U & u) {
		checkImpl([=] { return t <= u; }, std::to_string(t) + " was expected to be less than or equal to " + std::to_string(u));
	}
}


#endif
