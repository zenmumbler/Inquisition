Inquisition
===========

A stab at a modern implementation of a C++11 test framework.

Current C++ Unit test frameworks I've looked at use a ton of macros and void* and char* abstractions to inject fixtures etc.
This lib is an attempt to create a reasonably succinct and easy to use unit testing framework that relies on modern C++11 conventions
and not on tons of preprocessor stuff.

Still in the early development stages, but actively being worked on (Feb 2013)

Example Usage
-------------

	#include <iostream>
	#include "Inquisition.h"
	
	using namespace Inquisition;

	int main() {
		group("mytests", []{
			test("basic test", [](TestRun & res){
				std::cout << "running a test\n";
				res.check_false(1 == 2);
			});

			test("exception = error", [](TestRun & res){
				throw std::runtime_error("no cookie for you"); // reports error in test body
			});

			test("relations", [](TestRun & res){
				res.check_equal(5, 5);
				res.check_gt(10, 5);
				res.check_le(10, 4 * 2); // reports failure for check #3
			});
		});

		runAll();
	}

For a more practical example, look at the self-test files in the InquisitionTest dir.
