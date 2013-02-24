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
	TestPtr metaRoot_;
	TestReportPtr metaReport_;
	TestRun metaRun_;
	detail::PushCurrentTest pusher_;
public:
	MetaRun()
		: metaRoot_ { makeTest("meta", []{}) }
		, metaReport_ { makeReport<SimpleTestReport>(std::ref(std::cout)) }
		, metaRun_ { metaReport_ }
		, pusher_ { metaRoot_ }
	{}

	const TestReportPtr & report() const { return metaReport_; }
	
	void run() { metaRun_.run(metaRoot_); }
};


void testIntegers() {
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


int main() {
	group("Basics", []{
		test("boolean checks", []{
			check_true(true);
			check_false(false);
		});

		testIntegers();
	});
	
	auto r = makeReport<SimpleTestReport>(std::ref(std::cout));
	runAll(r);
	
	return r->failures() + r->errors();
}
