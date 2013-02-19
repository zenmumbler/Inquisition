//
//  Inquisition.h
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#ifndef INQUISITION_INCLUDED
#define INQUISITION_INCLUDED

#include <functional>
#include <memory>
#include <vector>
#include <string>

namespace Inquisition {
	
	class TestCase;
	class TestSetRunner;

	using TestMethod = std::function<void(TestSetRunner &)>;
	
	
	class BasicTest {
		std::string name_;
		
	public:
		BasicTest(std::string name) : name_(name) {}
		virtual ~BasicTest() {}
		virtual void operator()(TestSetRunner & res) = 0;
		
		const std::string & name() const { return name_; }
	};

	using TestSet = std::vector<std::unique_ptr<BasicTest>>;
	using TestSetRef = std::shared_ptr<TestSet>;
	
	class TestCase : public BasicTest {
		TestMethod method_;
		
	public:
		TestCase(std::string name, TestMethod method);
		void operator()(TestSetRunner & res) override;
	};
	
	
	class TestGroup : public BasicTest {
		TestSetRef subTests_;
		
	public:
		TestGroup(std::string name, const std::function<void()> & init);
		virtual void setup() {}
		virtual void tearDown() {}
		
		void operator()(TestSetRunner & res) override;
	};

	
	class TestSetRunner {
		int testCases_ = 0;
		int checks_ = 0;
		int failures_ = 0;
		int errors_ = 0;
		int passes_ = 0;

		std::string label_;
		const TestSetRef & testSet_;
		TestSet::const_iterator curTest_;
		std::vector<TestSetRunner> subRuns_;
		std::vector<const std::string> messages_;

		void addMessage(const std::string & msg);
		void failure(const std::string & msg);
		void error(const std::string & msg, const std::exception * ex = nullptr);

		template <typename T>
		void checkImpl(T expr, const std::string & failMsg) {
			try {
				checks_++;
				if (expr())
					passes_++;
				else
					failure(failMsg);
			}
			catch(const std::exception & ex) {
				error("Unexpected exception", &ex);
			}
			catch(...) {
				error("Unexpected exception");
			}
		}
		
	public:
		TestSetRunner(const std::string & label, const TestSetRef & testSet);

		int tests() const    { return testCases_; }
		int checks() const   { return checks_; }
		int failures() const { return failures_; }
		int errors() const   { return errors_; }
		int passes() const   { return passes_; }
		const std::string & label() const { return label_; }
		
		void run();
		void addSubRun(TestSetRunner subRun);
		
		void printResult() const;

		void check(bool expr) {
			checkImpl([=] { return expr; }, "");
		}
		
		template <typename T, typename U>
		void check_equal(const T & t, const U & u) {
			checkImpl([=] { return t == u; }, std::to_string(t) + " is not equal to " + std::to_string(u));
		}
		
		template <typename T, typename U>
		void check_gt(const T & t, const U & u) {
			checkImpl([=] { return t > u; }, std::to_string(t) + " is not greater than " + std::to_string(u));
		}
	};

	
	void test(const std::string & name, const TestMethod & method);
	void group(const std::string & name, const std::function<void()> & init);

	void run(const TestSetRef & test);
	void runAll();

} // namespace Inquisition


#endif /* defined(INQUISITION_INCLUDED) */
