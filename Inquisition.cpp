//
//  Inquisition.cpp
//
//  Created by Arthur Langereis on 2/18/13.
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
		: label_(label), testSet_(testSet), curTest_(testSet_->end())
	{}
	
	void TestRun::run() {
		for (curTest_ = testSet_->cbegin(); curTest_ < testSet_->cend(); curTest_++) {
			//result_.testCases++;
			
			try {
				(**curTest_)(*this);
			}
			catch (const std::exception & ex) {
				error("unexpected std::exception in test body: ", &ex);
			}
			catch (const std::string & str) {
				error("unexpected string exception in test body: " + str);
			}
			catch (const char * cp) {
				error("unexpected c-string exception in test body: " + std::string(cp));
			}
			catch (...) {
				error("unexpected other exception in test body");
			}
		}
	}
	
	void TestRun::addSubRun(TestRun subRun) {
		subRuns_.push_back(std::move(subRun));
	}
		
	void TestRun::pass() {
		result_.pass((*curTest_)->name());
	}
	
	void TestRun::failure(const std::string & msg, const std::exception * ex) {
		result_.failure((*curTest_)->name(), msg);
	}
	
	void TestRun::error(const std::string & msg, const std::exception * ex) {
		result_.error((*curTest_)->name(), msg, ex);
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
	
	void TestResult::pass(const std::string & testName) {
		
	}

	void TestResult::failure(const std::string & testName, const std::string & msg, const std::exception * ex) {
		
	}

	void TestResult::error(const std::string & testName, const std::string & msg, const std::exception * ex) {
		
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

	void test(const std::string & name, const TestMethod & method) {
		test<TestCase>(detail::currentSet(), name, method);
	}
	
	void group(const std::string & name, const std::function<void()> & init) {
		test<TestGroup>(detail::currentSet(), name, init);
	}
	
	void run(const TestSetRef & ts) {
		TestRun res("root", ts);
		res.run();
	}
	
	void runAll() {
		run(detail::currentSet());
	}

} // namespace Inquisition
