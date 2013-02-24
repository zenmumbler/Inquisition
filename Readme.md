Inquisition
===========

A stab at a modern implementation of a C++11 test framework.

Current C++ Unit test frameworks I've looked at use a ton of macros and void* and char* abstractions to inject fixtures etc.
This lib is an attempt to create a reasonably succinct and easy to use unit testing framework that relies on modern C++11 conventions
and not on tons of preprocessor stuff.

Still in the early development stages, actively being worked on (Feb 2013)

Example Usage
-------------

	#include <iostream>
	#include "Inquisition.h"
	
	using namespace Inquisition;

	int main() {
		// group tests together under a common name
		group("mytests", []{

			// each test() call is named and can have many checks in it
			test("basic test", []{
				std::cout << "running a test\n";
				check_false(1 == 2);
			});

			// Inquisition keeps track of groups and tests during creation and evaluation
			// so the definition syntax of tests and checks is nice and succinct
			test("relations", []{
				check_equal(5, 5);
				check_gt(10, 5);
				check_le(10, 4 * 2); // reports failure for check #3
			});
		});

		// all results are collected in a TestReport instance that you provide
		auto r = makeReport<SimpleTestReport>(std::ref(std::cout));
		runAll(r);

		// doing this will return a non-zero result for failed builds, for use in bigger build processes
		return r->failures() + r->errors();
	}

For a more practical example, look at the self-test files in the InquisitionTest dir.
