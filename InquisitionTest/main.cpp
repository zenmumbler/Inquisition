//
//  Inquisition Test main
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#include <iostream>
#include "Inquisition.h"

using namespace Inquisition;


class MetaRun {
	TestPtr metaRoot_ { makeTest("meta", []{}) };
	TestReportPtr metaReport_ { makeReport<SimpleTestReport>(std::ref(std::cout)) };
	TestRun metaRun_ { metaReport_ };
	detail::PushCurrentTest pusher_ { metaRoot_ };

public:
	const TestReportPtr & report() const { return metaReport_; }
	void run() { metaRun_.run(metaRoot_); }
};


#include "test_integers_inl.h"
#include "test_floats_inl.h"
#include "test_strings_inl.h"


int main() {
	group("basics", []{
		test("simple boolean checks should match", []{
			check_true(true);
			check_false(false);
		});

		testIntegers();
		testFloats();
		testStrings();
	});
	
	auto r = makeReport<SimpleTestReport>(std::ref(std::cout));
	runAll(r);
	
	return r->failures() + r->errors();
}
