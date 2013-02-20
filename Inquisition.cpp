//
//  Inquisition.cpp
//
//  by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#include "Inquisition.h"
#include <iostream>

namespace Inquisition {

	namespace detail {
		static std::weak_ptr<TestSet> currentTestSet;
		
		static TestSetRef currentSet() {
			static TestSetRef rootSet { new TestSet() };
			if (currentTestSet.expired())
				currentTestSet = rootSet;
			
			return currentTestSet.lock();
		}
		
		static std::shared_ptr<TestSet> setCurrentSet(const TestSetRef & newSet) {
			auto previous = currentSet();
			currentTestSet = newSet;
			return previous;
		}
		
		class PushCurrentTestSet {
			TestSetRef oldSet;
		public:
			PushCurrentTestSet(TestSetRef & newSet) : oldSet(setCurrentSet(newSet)) {}
			~PushCurrentTestSet() { setCurrentSet(oldSet); }
		};
	}

	
	//  ____            _     _____         _
	// | __ )  __ _ ___(_) __|_   _|__  ___| |_
	// |  _ \ / _` / __| |/ __|| |/ _ \/ __| __|
	// | |_) | (_| \__ \ | (__ | |  __/\__ \ |_
	// |____/ \__,_|___/_|\___||_|\___||___/\__|
	//
	
	void BasicTest::pass(const std::string & msg, const std::string & innerMsg) {
		run_.lock()->result().pass(name(), msg, innerMsg);
	}
	
	void BasicTest::failure(const std::string & msg, const std::string & innerMsg) {
		run_.lock()->result().failure(name(), msg, innerMsg);
	}
	
	void BasicTest::error(const std::string & msg, const std::string & innerMsg) {
		run_.lock()->result().error(name(), msg, innerMsg);
	}

	
	//  _____         _    ____
	// |_   _|__  ___| |_ / ___|__ _ ___  ___
	//   | |/ _ \/ __| __| |   / _` / __|/ _ \
	//   | |  __/\__ \ |_| |__| (_| \__ \  __/
	//   |_|\___||___/\__|\____\__,_|___/\___|
	//

	TestCase::TestCase(std::string name, TestMethod method)
		: BasicTest(std::move(name)), method_(std::move(method))
	{}
	
	void TestCase::operator()(TestRun & res) {
		method_(res);
	}

	
	//  _____         _    ____
	// |_   _|__  ___| |_ / ___|_ __ ___  _   _ _ __
	//   | |/ _ \/ __| __| |  _| '__/ _ \| | | | '_ \
	//   | |  __/\__ \ |_| |_| | | | (_) | |_| | |_) |
	//   |_|\___||___/\__|\____|_|  \___/ \__,_| .__/
	//                                         |_|

	TestGroup::TestGroup(std::string name, const std::function<void()> & init)
		: BasicTest(std::move(name)), subTests_(new TestSet())
	{
		detail::PushCurrentTestSet _(subTests_);
		init();
	}
	
	void TestGroup::operator()(TestRun & res) {
		TestRun groupRun(res.label() + "." + name(), subTests_);
		groupRun.run();
		res.addSubRun(std::move(groupRun));
	}

	
	//  _____         _   ____
	// |_   _|__  ___| |_|  _ \ _   _ _ __
	//   | |/ _ \/ __| __| |_) | | | | '_ \
	//   | |  __/\__ \ |_|  _ <| |_| | | | |
	//   |_|\___||___/\__|_| \_\\__,_|_| |_|
	//
	
	TestRun::TestRun(const std::string & label, const TestSetRef & testSet)
		: label_(label), testSet_(testSet), curTest_(testSet_->end()), result_{label_}
	{}
	
	void TestRun::run() {
		for (curTest_ = testSet_->cbegin(); curTest_ < testSet_->cend(); curTest_++) {
			checkIndex_ = 0;
			
			try {
				(**curTest_)(*this);
			}
			catch (const std::exception & ex) {
				error("unexpected std::exception in test body", ex.what());
			}
			catch (const std::string & str) {
				error("unexpected string exception in test body", str);
			}
			catch (const char * cp) {
				error("unexpected c-string exception in test body", cp);
			}
			catch (...) {
				error("unexpected other exception in test body");
			}
		}
	}
	
	void TestRun::addSubRun(TestRun subRun) {
		subRuns_.push_back(std::move(subRun));
	}

//	void TestRun::printResult() const {
//		std::cout << "\n-----------------------------\n";
//		std::cout << label_ << " results";
//		std::cout << "\n-----------------------------\n";
//		std::cout << "test cases : " << testCases_ << '\n';
//		std::cout << "checks     : " << checks_ << '\n';
//		std::cout << "passes     : " << passes_ << '\n';
//		std::cout << "failures   : " << failures_ << '\n';
//		std::cout << "errors     : " << errors_;
//		std::cout << "\n-----------------------------\n";
//		std::cout << "Verdict    : " << ((failures_ + errors_ > 0) ? "FAILURE" : "SUCCESS");
//		std::cout << "\n-----------------------------\n";
//		
//		if (messages_.size()) {
//			std::cout << "problems:\n";
//			for (const auto & msg : messages_)
//				std::cout << msg << '\n';
//			std::cout << "-----------------------------\n";
//		}
//	}
	
	
	//  _____         _   ____                 _ _
	// |_   _|__  ___| |_|  _ \ ___  ___ _   _| | |_
	//   | |/ _ \/ __| __| |_) / _ \/ __| | | | | __|
	//   | |  __/\__ \ |_|  _ <  __/\__ \ |_| | | |_
	//   |_|\___||___/\__|_| \_\___||___/\__,_|_|\__|
	//
	
	void TestResult::pass(const std::string & testName, const std::string & msg, const std::string & innerMsg) {
		std::cout << "PASS " << label_ << '.' << testName << ": " << msg << ' ' << innerMsg << '\n';
	}

	void TestResult::failure(const std::string & testName, const std::string & msg, const std::string & innerMsg) {
		std::cout << "FAIL " << label_ << '.' << testName << ": " << msg << ' ' << innerMsg << '\n';
	}

	void TestResult::error(const std::string & testName, const std::string & msg, const std::string & innerMsg) {
		std::cout << "ERROR " << label_ << '.' << testName << ": " << msg << ' ' << innerMsg << '\n';
	}


	//     _    ____ ___
	//    / \  |  _ \_ _|
	//   / _ \ | |_) | |
	//  / ___ \|  __/| |
	// /_/   \_\_|  |___|
	//

	template <typename T, typename ...Args>
	void test(TestSetRef set, Args&&... args) {
		set->push_back(std::unique_ptr<BasicTest>(new T(std::forward<Args>(args)...)));
	}

	template <typename T>
	void test(TestSetRef set) {
		set->push_back(std::unique_ptr<BasicTest>(new T()));
	}
	
	void test(const std::string & name, const TestMethod & method) {
		test<TestCase>(detail::currentSet(), name, method);
	}
	
	void group(const std::string & name, const std::function<void()> & init) {
		test<TestGroup>(detail::currentSet(), name, init);
	}
	
	template <typename T, typename ...Args>
	void ctest(Args... args) {
		test<T>(detail::currentSet(), std::forward<Args>(args)...);
	}
	
	template <typename T>
	void ctest() {
		test<T>(detail::currentSet());
	}
	
	void run(const TestSetRef & ts) {
		TestRun res("root", ts);
		res.run();
	}
	
	void runAll() {
		run(detail::currentSet());
	}

} // namespace Inquisition
