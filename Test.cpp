//
//  Test.cpp
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/23/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include "Test.h"

namespace Inquisition {
	namespace detail {
		static std::weak_ptr<Test> currentTest_s;
		
		TestPtr rootTest() {
			static TestPtr rootTest_s = makeTest("root", []{});
			return rootTest_s;
		}
		
		TestPtr currentTest() {
			if (currentTest_s.expired())
				currentTest_s = rootTest();
			
			return currentTest_s.lock();
		}
		
		TestPtr setCurrentTest(const TestPtr & newTest) {
			auto previous = currentTest();
			currentTest_s = newTest;
			return previous;
		}
	}

	
	Test::Test(std::string name, SimpleMethod exec)
		: name_(std::move(name)), exec_(std::move(exec))
	{}
	
	void Test::run() const {
		exec_();
	}
	
	void Test::addChild(const TestPtr & test) {
		subTests_.push_back(test);
	}
}
