//
//  TestRun.cpp
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include "TestRun.h"

namespace Inquisition {

	TestRun::TestRun(const TestReportPtr & report)
		: report_(report)
	{}
	
	void TestRun::eval(const TestPtr & test) {
		report_->enterTest(test);

		try {
			test->run();
		}
		catch (const std::exception & ex) {
			report_->error("unexpected std::exception", ex.what());
		}
		catch (const std::string & str) {
			report_->error("unexpected string exception", str);
		}
		catch (const char * cp) {
			report_->error("unexpected c-string exception", cp);
		}
		catch (...) {
			report_->error("unexpected other exception");
		}

		for (const auto & sub : test->subTests())
			eval(sub);

		report_->leaveTest(test);
	}
	
	void TestRun::run(const TestPtr & root) {
		detail::PushCurrentReport _(report_);
		report_->startReport();
		eval(root);
		report_->finishReport();
	}
}
