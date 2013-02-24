//
//  Inquisition.h
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#ifndef INQUISITION_INCLUDED
#define INQUISITION_INCLUDED

#include "Test.h"
#include "TestRun.h"
#include "checks.h"

namespace Inquisition {
	void test(const TestPtr & parent, const std::string & name, const SimpleMethod & method);
	void test(const std::string & name, const SimpleMethod & method);

	void group(const TestPtr & parent, const std::string & name, const SimpleMethod & init);
	void group(const std::string & name, const std::function<void()> & init);

	void run(const TestPtr & root, const TestReportPtr & report);
	void runAll(const TestReportPtr & report);
} // namespace Inquisition


#endif /* defined(INQUISITION_INCLUDED) */
