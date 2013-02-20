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


class SimpleFixture : Fixture {
public:
	int x = 1;
};


int main() {
	group("Inquisition", []{
		def_fixture<SimpleFixture>("simple");
		
		test("for_each", [](TestRun & res){
			std::cout << "running a test\n";
			res.check_true(1 == 2);
		});
		
		test("exception1", [](TestRun & res){
			throw "some random string";
		});
		
		test("exception2", [](TestRun & res){
			throw std::runtime_error("I hate you");
		});
		
		test("ok", [](TestRun & res){
			res.check_equal(5, 5);
			res.check_gt(10, 5);
			res.check_gt(8, 12);
		});
		
		test("fixture test", [=](TestRun & res){
			auto & sf = fixture<SimpleFixture>("simple");
			res.check_equal(sf.x, 1);
		});
	});
	
	runAll();
}
