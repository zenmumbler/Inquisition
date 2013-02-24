//
//  test_strings_inl.h
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/24/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

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
	
	group("string failures", []{
		test("failed equations should yield failures", [=]{
			MetaRun meta;

			std::string mk { "monkey" };
			std::string oc { "octopus" };

			test("inverse string equation checks", [=]{
				check_equal("monkey", "octopus");
				check_equal(mk, "octopus");
				check_equal("octopus", mk);
				check_equal(mk, oc);
				
				check_not_equal("monkey", "monkey");
				check_not_equal(oc, "octopus");
				check_not_equal("octopus", oc);
				check_not_equal(mk, mk);
			});

			test("inverse string order checks", [=]{
				// > and >=
				check_gt("monkey", "octopus");
				check_gt("monkey", "monkey");
				check_ge("monkey", "octopus");
				
				check_gt(mk, "octopus");
				check_gt(mk, "monkey");
				check_ge(mk, "octopus");
				
				check_gt("monkey", oc);
				check_gt("monkey", mk);
				check_ge("monkey", oc);
				
				check_gt(mk, oc);
				check_gt(mk, mk);
				check_ge(mk, oc);
				
				// < and <=
				check_lt("octopus", "monkey");
				check_lt("octopus", "octopus");
				check_le("octopus", "monkey");
				
				check_lt(oc, "monkey");
				check_lt(oc, "octopus");
				check_le(oc, "monkey");
				
				check_lt("octopus", mk);
				check_lt("octopus", oc);
				check_le("octopus", mk);
				
				check_lt(oc, mk);
				check_lt(oc, oc);
				check_le(oc, mk);
			});

			meta.run();
			
			auto metaReport = meta.report();
			check_equal(metaReport->passes(),   0);
			check_equal(metaReport->failures(), 32);
			check_equal(metaReport->errors(),   0);
		});
	});
}
