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
		int checkIndex_ = 0;
		std::weak_ptr<TestRun> run_;
		
		void pass(const std::string & msg, const std::string & innerMsg = "");
		void failure(const std::string & msg, const std::string & innerMsg = "");
		void error(const std::string & msg, const std::string & innerMsg = "");
		
	public:
		BasicTest(std::string name) : name_(name) {}
		virtual ~BasicTest() {}

		virtual void operator()(TestRun & res) = 0;
		
		const std::string & name() const { return name_; }
		
		// -- checks

		template <typename Expr>
		void checkImpl(Expr expr, const std::string & failMsg) {
			try {
				checkIndex_++;
				
				if (expr())
					pass("check #" + std::to_string(checkIndex_));
				else
					failure("check #" + std::to_string(checkIndex_), failMsg);
			}
			catch(const std::exception & ex) {
				failure("unexpected std::exception in check #" + std::to_string(checkIndex_), ex.what());
			}
			catch (const std::string & str) {
				failure("unexpected string exception in check #" + std::to_string(checkIndex_), str);
			}
			catch (const char * cp) {
				failure("unexpected c-string exception in check #" + std::to_string(checkIndex_), cp);
			}
			catch(...) {
				failure("unexpected other exception in check #" + std::to_string(checkIndex_));
			}
		}
		
		// -- checks
		
		void check_true(bool expr) { checkImpl([=] { return expr; }, "expression was expected to be true"); }
		void check_false(bool expr) { checkImpl([=] { return !expr; }, "expression was expected to be false"); }
		
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
		void operator()(TestRun & res) override;
	};


	template <typename Fix>
	class FixtureTest : public BasicTest {
	public:
		FixtureTest(const std::string & name) : BasicTest(name) {}
		Fix fix;
	};


	class TestResult {
		int passes_ = 0;
		int failures_ = 0;
		int errors_ = 0;
		const std::string label_;

	public:
		TestResult() {}
		TestResult(std::string label) : label_(label) {}

		void pass(const std::string & testName, const std::string & msg, const std::string & innerMsg = "");
		void failure(const std::string & testName, const std::string & msg, const std::string & innerMsg = "");
		void error(const std::string & testName, const std::string & msg, const std::string & innerMsg = "");
	};

	
	class TestRun {
		std::string label_;
		const TestSetRef testSet_;
		TestSet::const_iterator curTest_;
		TestResult result_;
		std::vector<TestRun> subRuns_;

	public:
		TestRun(const std::string & label, const TestSetRef & testSet);

		const std::string & label() const { return label_; }
		TestResult & result() { return result_; }
		void addSubRun(TestRun subRun);
		
		void run();
	};
	
	
	void test(const std::string & name, const TestMethod & method);
	void group(const std::string & name, const std::function<void()> & init);

	void run(const TestSetRef & test);
	void runAll();

} // namespace Inquisition


#endif /* defined(INQUISITION_INCLUDED) */
