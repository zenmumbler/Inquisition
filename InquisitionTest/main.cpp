//
//  Inquisition Test main
//
//  Created by Arthur Langereis
//  Copyright (c) 2013 logicdream. All rights reserved.
//

#include <iostream>

#include "Inquisition.h"

using namespace Inquisition;


class SimpleFixture {
public:
	
};


int main() {
	group("Inquisition", []{
		
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
		
	});
	
	runAll();
}
