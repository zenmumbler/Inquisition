//
//  TestRun.h
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#ifndef INQUISITION_TESTRUN_INCLUDED
#define INQUISITION_TESTRUN_INCLUDED

#include "Test.h"
#include "TestReport.h"

namespace Inquisition {

	class TestRun {
		TestReportPtr report_;

		void eval(const TestPtr & test);
		
	public:
		TestRun(const TestReportPtr & result);
		
		const TestReportPtr & report() const { return report_; }
		
		void run(const TestPtr & root);
	};
}

#endif /* defined(INQUISITION_TESTRUN_INCLUDED) */
