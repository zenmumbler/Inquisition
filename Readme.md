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
			test("some test", [](TestSetRunner & res){
				std::cout << "running a test\n";
				res.check(1 == 2);
			});

			test("exception1", [](TestSetRunner & res){
				throw "some random string";
			});

			test("exception2", [](TestSetRunner & res){
				throw std::runtime_error("no cookie for you");
			});

			test("ok", [](TestSetRunner & res){
				res.check_equal(5, 5);
				res.check_gt(10, 5);
			});
		});

		runAll();
	}
