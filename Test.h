//
//  Test.h
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#ifndef INQUISITION_TEST_INCLUDED
#define INQUISITION_TEST_INCLUDED

#include <functional>
#include <memory>
#include <vector>
#include <string>

namespace Inquisition {

	using SimpleMethod = std::function<void()>;

	class Test;
	using TestPtr = std::shared_ptr<Test>;
	using TestSet = std::vector<TestPtr>;
	
	
	class Test {
		std::string name_;
		SimpleMethod exec_;
		TestSet subTests_;
		
	public:
		Test(std::string name, SimpleMethod exec);
		void run() const;
		
		const std::string & name() const { return name_; }
		const TestSet & subTests() const { return subTests_; }
		void addChild(const TestPtr & test);
	};
	
	template <typename ...Args>
	TestPtr makeTest(Args... args) {
		return std::make_shared<Test>(std::forward<Args>(args)...);
	}

	namespace detail {
		TestPtr rootTest();
		TestPtr currentTest();
		TestPtr setCurrentTest(const TestPtr & newTest);
		
		class PushCurrentTest {
			TestPtr oldTest;
		public:
			PushCurrentTest(TestPtr & newTest) : oldTest(setCurrentTest(newTest)) {}
			~PushCurrentTest() { setCurrentTest(oldTest); }
		};
	}
}

#endif /* defined(INQUISITION_TEST_INCLUDED) */
