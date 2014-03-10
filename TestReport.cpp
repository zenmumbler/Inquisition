//
//  TestReport.cpp
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include <iostream>
#include "TestReport.h"

namespace Inquisition {
	
	namespace detail {
		static std::weak_ptr<TestReport> currentReport_s;
		
		TestReportPtr currentReport() {
			return currentReport_s.lock();
		}
		
		TestReportPtr setCurrentReport(const TestReportPtr & newReport) {
			auto previous = currentReport();
			currentReport_s = newReport;
			return previous;
		}
	}


	SimpleTestReport::SimpleTestReport(std::ostream & dest)
		: dest_(dest)
	{}

	void SimpleTestReport::startReport() {
		dest_ << "Test Report\n------------------\n";
	}

	void SimpleTestReport::finishReport() {
		dest_ << "------------------\n";
		dest_ << "passes   : " << passes_ << '\n';
		dest_ << "failures : " << failures_ << '\n';
		dest_ << "errors   : " << errors_ << '\n';
	}

	void SimpleTestReport::enterTest(const TestPtr & test) {
		nameTree_.push_back(test->name());
		checkIndex_ = 0;
	}

	void SimpleTestReport::leaveTest(const TestPtr & test) {
		nameTree_.pop_back();
	}
	
	void SimpleTestReport::printNameTree() {
		for (const std::string & n : nameTree_)
			dest_ << n << '.';
	}

	void SimpleTestReport::pass(const std::string & msg, const std::string & innerMsg) {
		checkIndex_++;
		passes_++;
//		dest_ << "PASS (check #" << checkIndex_ << ") ";
//		printNameTree();
//		dest_ << msg << ' ' << innerMsg << '\n';
	}
	
	void SimpleTestReport::failure(const std::string & msg, const std::string & innerMsg) {
		checkIndex_++;
		failures_++;
		dest_ << "FAIL (check #" << checkIndex_ << ") ";
		printNameTree();
		dest_ << ": " << msg << ' ' << innerMsg << '\n';
	}
	
	void SimpleTestReport::error(const std::string & msg, const std::string & innerMsg) {
		errors_++;
		dest_ << "ERROR ";
		printNameTree();
		dest_ << ": " << msg << ' ' << innerMsg << '\n';
	}

}
