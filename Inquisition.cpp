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
		
		static TestSetPtr currentSet() {
			static TestSetPtr rootSet { new TestSet() };
			if (currentTestSet.expired())
				currentTestSet = rootSet;
			
			return currentTestSet.lock();
		}
		
		static std::shared_ptr<TestSet> setCurrentSet(const TestSetPtr & newSet) {
			auto previous = currentSet();
			currentTestSet = newSet;
			return previous;
		}
		
		class PushCurrentTestSet {
			TestSetPtr oldSet;
		public:
			PushCurrentTestSet(TestSetPtr & newSet) : oldSet(setCurrentSet(newSet)) {}
			~PushCurrentTestSet() { setCurrentSet(oldSet); }
		};


		// -- fixtures
		
		std::unordered_map<std::string, std::unique_ptr<FixtureBase>> FixtureBase::fixtures_;
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
	
	TestRun::TestRun(const std::string & label, const TestSetPtr & testSet)
		: label_(label), testSet_(testSet), curTest_(testSet_->end()), result_{label_}
	{}
	
	static void recurseSumResult(const TestRun & lvl, TestResult & res) {
		res += lvl.result();
		for (const TestRun & sub : lvl.subRuns())
			recurseSumResult(sub, res);
	}
	
	TestResult TestRun::compoundResult() {
		TestResult comp;
		recurseSumResult(*this, comp);
		return comp;
	}
	
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
	
	void TestRun::pass(const std::string & msg, const std::string & innerMsg) {
		result_.pass((*curTest_)->name(), msg, innerMsg);
	}
	
	void TestRun::failure(const std::string & msg, const std::string & innerMsg) {
		result_.failure((*curTest_)->name(), msg, innerMsg);
	}
	
	void TestRun::error(const std::string & msg, const std::string & innerMsg) {
		result_.error((*curTest_)->name(), msg, innerMsg);
	}
	
	
	//  _____         _   ____                 _ _
	// |_   _|__  ___| |_|  _ \ ___  ___ _   _| | |_
	//   | |/ _ \/ __| __| |_) / _ \/ __| | | | | __|
	//   | |  __/\__ \ |_|  _ <  __/\__ \ |_| | | |_
	//   |_|\___||___/\__|_| \_\___||___/\__,_|_|\__|
	//

	TestResult TestResult::operator +(const TestResult & rhs) {
		TestResult sum { rhs };
		sum.passes_ += passes_;
		sum.failures_ += failures_;
		sum.errors_ += errors_;
		
		return std::move(sum);
	}
	
	TestResult & TestResult::operator +=(const TestResult & rhs) {
		passes_ += rhs.passes_;
		failures_ += rhs.failures_;
		errors_ += rhs.errors_;
		
		return *this;
	}
	
	void TestResult::pass(const std::string & testName, const std::string & msg, const std::string & innerMsg) {
		passes_++;
		std::cout << "PASS " << label_ << '.' << testName << ": " << msg << ' ' << innerMsg << '\n';
	}

	void TestResult::failure(const std::string & testName, const std::string & msg, const std::string & innerMsg) {
		failures_++;
		std::cout << "FAIL " << label_ << '.' << testName << ": " << msg << ' ' << innerMsg << '\n';
	}

	void TestResult::error(const std::string & testName, const std::string & msg, const std::string & innerMsg) {
		errors_++;
		std::cout << "ERROR " << label_ << '.' << testName << ": " << msg << ' ' << innerMsg << '\n';
	}


	//     _    ____ ___
	//    / \  |  _ \_ _|
	//   / _ \ | |_) | |
	//  / ___ \|  __/| |
	// /_/   \_\_|  |___|
	//

	void test(TestSetPtr set, const std::string & name, const TestMethod & method) {
		detail::test<TestCase>(set, name, method);
	}

	void test(const std::string & name, const TestMethod & method) {
		detail::test<TestCase>(detail::currentSet(), name, method);
	}
	
	void group(const std::string & name, const std::function<void()> & init) {
		detail::test<TestGroup>(detail::currentSet(), name, init);
	}
	
	TestRun run(const TestSetPtr & ts) {
		TestRun res("root", ts);
		res.run();
		return std::move(res);
	}
	
	TestRun runAll() {
		return run(detail::currentSet());
	}

} // namespace Inquisition
