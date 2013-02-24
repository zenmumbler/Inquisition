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


static void testIntegers() {
	group("integers", []{
		test("relations should be correct", []{
			check_equal(100, 100);
			check_not_equal(99, 100);
			
			check_ge(100, 99);
			check_ge(100, 100);
			
			check_gt(100, 99);
			
			check_le(99, 100);
			check_le(100, 100);
			
			check_lt(99, 100);
		});
		
		test("failed comparisons should yield failures", []{
			MetaRun meta;
			
			test("inverse integer", []{
				check_equal(100, 99);
				check_not_equal(100, 100);
				
				check_ge(99, 100);
				
				check_gt(99, 100);
				check_gt(100, 100);
				
				check_le(100, 99);
				
				check_lt(100, 99);
				check_lt(100, 100);
			});
			
			meta.run();

			auto metaReport = meta.report();
			check_equal(metaReport->passes(),   0);
			check_equal(metaReport->failures(), 8);
			check_equal(metaReport->errors(),   0);
		});
		
		test("templated comparisons should work with all integer types", []{
			check_gt((char)CHAR_MAX, (char)CHAR_MIN);
			check_gt((unsigned char)UCHAR_MAX, (unsigned char)0);
			check_gt((int16_t)INT16_MAX, (int16_t)INT16_MIN);
			check_gt((uint16_t)UINT16_MAX, (uint16_t)0);
			check_gt((int)INT_MAX, (int)INT_MIN);
			check_gt((uint32_t)UINT32_MAX, (uint32_t)0);
			check_gt((long)LONG_MAX, (long)LONG_MIN);
			check_gt((unsigned long)ULONG_MAX, (unsigned long)0);
			check_gt((long long)LONG_LONG_MAX, (long long)LONG_LONG_MIN);
			check_gt((unsigned long long)ULONG_LONG_MAX, (unsigned long long)0);
		});
	});
}


static void testStrings() {
	group("strings", []{
		std::string mk { "monkey" };
		std::string oc { "octopus" };

		test("strings should equate correctly", [=]{
			check_equal("monkey", "monkey");
			check_equal(mk, "monkey");
			check_equal("monkey", mk);
			check_equal(mk, mk);

			check_not_equal("monkey", "octopus");
			check_not_equal(oc, "monkey");
			check_not_equal("monkey", oc);
			check_not_equal(mk, oc);
		});

		test("strings should order correctly", [=]{
			// < and <=
			check_lt("monkey", "octopus");
			check_le("monkey", "octopus");
			check_le("monkey", "monkey");

			check_lt(mk, "octopus");
			check_le(mk, "octopus");
			check_le(mk, "monkey");

			check_lt("monkey", oc);
			check_le("monkey", oc);
			check_le("monkey", mk);

			check_lt(mk, oc);
			check_le(mk, oc);
			check_le(mk, mk);

			// > and >=
			check_gt("octopus", "monkey");
			check_ge("octopus", "monkey");
			check_ge("octopus", "octopus");
			
			check_gt(oc, "monkey");
			check_ge(oc, "monkey");
			check_ge(oc, "octopus");
			
			check_gt("octopus", mk);
			check_ge("octopus", mk);
			check_ge("octopus", oc);
			
			check_gt(oc, mk);
			check_ge(oc, mk);
			check_ge(oc, oc);
		});
	});
}


int main() {
	group("Basics", []{
		test("boolean checks", []{
			check_true(true);
			check_false(false);
		});

		testIntegers();
		testStrings();
	});
	
	auto r = makeReport<SimpleTestReport>(std::ref(std::cout));
	runAll(r);
	
	return r->failures() + r->errors();
}
