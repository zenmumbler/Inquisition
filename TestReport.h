//
//  TestReport.h
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#ifndef INQUISITION_TESTREPORT_INCLUDED
#define INQUISITION_TESTREPORT_INCLUDED

#include <string>
#include <memory>
#include <iosfwd>
#include "Test.h"

namespace Inquisition {

	class TestReport {
	public:
		virtual ~TestReport() {}
		
		virtual void startReport() = 0;
		virtual void finishReport() = 0;
		
		virtual void enterTest(const TestPtr & test) = 0;
		virtual void leaveTest(const TestPtr & test) = 0;
		
		virtual void pass(const std::string & msg = "", const std::string & innerMsg = "") = 0;
		virtual void failure(const std::string & msg, const std::string & innerMsg = "") = 0;
		virtual void error(const std::string & msg, const std::string & innerMsg = "") = 0;
		
		virtual int passes() const = 0;
		virtual int failures() const = 0;
		virtual int errors() const = 0;
	};

	using TestReportPtr = std::shared_ptr<TestReport>;

	template <typename R, typename ...Args>
	TestReportPtr makeReport(Args... args) {
		return std::make_shared<R>(std::forward<Args>(args)...);
	}


	namespace detail {
		TestReportPtr currentReport();
		TestReportPtr setCurrentReport(const TestReportPtr & newReport);

		class PushCurrentReport {
			TestReportPtr oldReport;
		public:
			PushCurrentReport(TestReportPtr & newReport) : oldReport(setCurrentReport(newReport)) {}
			~PushCurrentReport() { setCurrentReport(oldReport); }
		};
	}

	
	class SimpleTestReport : public TestReport {
		int passes_ = 0;
		int failures_ = 0;
		int errors_ = 0;
		int checkIndex_ = 0;
		std::ostream & dest_;
		std::vector<std::string> nameTree_;
		
		void printNameTree();
		
	public:
		SimpleTestReport(std::ostream & dest);

		virtual void startReport() override;
		virtual void finishReport() override;

		virtual void enterTest(const TestPtr & test) override;
		virtual void leaveTest(const TestPtr & test) override;
		
		virtual void pass(const std::string & msg = "", const std::string & innerMsg = "") override;
		virtual void failure(const std::string & msg, const std::string & innerMsg = "") override;
		virtual void error(const std::string & msg, const std::string & innerMsg = "") override;
		
		virtual int passes() const override { return passes_; }
		virtual int failures() const override { return failures_; }
		virtual int errors() const override { return errors_; }
	};

}

#endif /* defined(INQUISITION_TESTREPORT_INCLUDED) */
