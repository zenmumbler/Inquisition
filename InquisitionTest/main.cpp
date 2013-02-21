//
//  Inquisition Test main
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#include <iostream>
#include <unordered_map>

#include "Inquisition.h"

using namespace Inquisition;


class MetaRun {
	TestSetPtr set_;
	TestRun run_;
public:
	MetaRun() : set_(new TestSet()), run_("meta", set_) {}

	TestRun & testRun() { return run_; }
	TestResult & result() { return run_.result(); }
	TestSetPtr & testSet() { return set_; }
	
	void test(const std::string & name, const TestMethod & method) {
		Inquisition::test(set_, name, method);
	}
	
	void run() { run_.run(); }
};


void integers() {
	test("Integer relations", [](TestRun & res){
		res.check_equal(100, 100);
		res.check_not_equal(99, 100);
		
		res.check_ge(100, 99);
		res.check_ge(100, 100);
		
		res.check_gt(100, 99);
		
		res.check_le(99, 100);
		res.check_le(100, 100);
		
		res.check_lt(99, 100);
	});
	
	test("Integer relation failures", [](TestRun & res){
		MetaRun meta;
		
		meta.test("inverse integer", [](TestRun & mr) {
			mr.check_equal(100, 99);
			mr.check_not_equal(100, 100);
			
			mr.check_ge(99, 100);
			
			mr.check_gt(99, 100);
			mr.check_gt(100, 100);
			
			mr.check_le(100, 99);
			
			mr.check_lt(100, 99);
			mr.check_lt(100, 100);
		});
		
		meta.run();
		
		res.check_equal(meta.result().passes(),   0);
		res.check_equal(meta.result().failures(), 8);
		res.check_equal(meta.result().errors(),   0);
	});
	
	test("Integer limits", [](TestRun & res){
		// test proper template type deduction of comparison methods
		res.check_gt((char)CHAR_MAX, (char)CHAR_MIN);
		res.check_gt((unsigned char)UCHAR_MAX, (unsigned char)0);
		res.check_gt((int16_t)INT16_MAX, (int16_t)INT16_MIN);
		res.check_gt((uint16_t)UINT16_MAX, (uint16_t)0);
		res.check_gt((int)INT_MAX, (int)INT_MIN);
		res.check_gt((uint32_t)UINT32_MAX, (uint32_t)0);
		res.check_gt((long)LONG_MAX, (long)LONG_MIN);
		res.check_gt((unsigned long)ULONG_MAX, (unsigned long)0);
		res.check_gt((long long)LONG_LONG_MAX, (long long)LONG_LONG_MIN);
		res.check_gt((unsigned long long)ULONG_LONG_MAX, (unsigned long long)0);
	});
}


int main() {
	test("Boolean checks", [](TestRun & res){
		res.check_true(true);
		res.check_false(false);
	});
	
	integers();
	
	runAll();
}
