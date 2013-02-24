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
}
