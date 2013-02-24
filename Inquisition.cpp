//
//  Inquisition.cpp
//
//  by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#include "Inquisition.h"
#include <iostream>

namespace Inquisition {

	void test(const TestPtr & parent, const std::string & name, const SimpleMethod & method) {
		parent->addChild(makeTest(name, method));
	}

	void test(const std::string & name, const SimpleMethod & method) {
		test(detail::currentTest(), name, method);
	}
	
	void group(const TestPtr & parent, const std::string & name, const SimpleMethod & init) {
		auto g = makeTest(name, []{});
		parent->addChild(g);
		
		detail::PushCurrentTest _(g);
		init();
	}
	
	void group(const std::string & name, const SimpleMethod & init) {
		group(detail::currentTest(), name, init);
	}
	
	void run(const TestPtr & root, const TestReportPtr & report) {
		TestRun res { report };
		res.run(root);
	}
	
	void runAll(const TestReportPtr & report) {
		run(detail::rootTest(), report);
	}

} // namespace Inquisition
