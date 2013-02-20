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
	class TestRun;

	using TestMethod = std::function<void(TestRun &)>;
	
	
	class BasicTest {
		std::string name_;
		
	public:
		BasicTest(std::string name) : name_(name) {}
		virtual ~BasicTest() {}
		virtual void operator()(TestRun & res) = 0;
		
		const std::string & name() const { return name_; }
	};

	using TestSet = std::vector<std::unique_ptr<BasicTest>>;
	using TestSetRef = std::shared_ptr<TestSet>;
	
	class TestCase : public BasicTest {
		TestMethod method_;
		
	public:
		TestCase(std::string name, TestMethod method);
		void operator()(TestRun & res) override;
	};
	
	
	class TestGroup : public BasicTest {
		TestSetRef subTests_;
		
	public:
		TestGroup(std::string name, const std::function<void()> & init);
		virtual void setup() {}
		virtual void tearDown() {}
		
		void operator()(TestRun & res) override;
	};
	
	
	class TestResult {
		int passes_ = 0;
		int failures_ = 0;
		int errors_ = 0;
		const std::string label_;
		
		std::vector<const std::string> messages;

	public:
		void pass(const std::string & testName);
		void failure(const std::string & testName, const std::string & msg, const std::exception * ex = nullptr);
		void error(const std::string & testName, const std::string & msg, const std::exception * ex = nullptr);
	};

	
	class TestRun {
		std::string label_;
		const TestSetRef testSet_;
		TestSet::const_iterator curTest_;
		TestResult result_;
		std::vector<TestRun> subRuns_;

		void pass();
		void failure(const std::string & msg, const std::exception * ex = nullptr);
		void error(const std::string & msg, const std::exception * ex = nullptr);

	public:
		TestRun(const std::string & label, const TestSetRef & testSet);

		const std::string & label() const { return label_; }
		const TestResult & result() const { return result_; }
		
		void run();
		void addSubRun(TestRun subRun);

		template <typename T>
		void checkImpl(T expr, const std::string & failMsg) {
			try {
				if (expr())
					pass();
				else
					failure(failMsg);
			}
			catch(const std::exception & ex) {
				failure("unexpected std::exception during check", &ex);
			}
			catch (const std::string & str) {
				failure("unexpected string exception in test body: " + str);
			}
			catch (const char * cp) {
				failure("unexpected c-string exception in test body: " + std::string(cp));
			}
			catch(...) {
				failure("unexpected other exception");
			}
		}
		
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
