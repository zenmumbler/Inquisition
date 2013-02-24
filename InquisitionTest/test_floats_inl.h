//
//  test_floats_inl.h
//  InquisitionTest
//
//  Created by Arthur Langereis on 2/24/13.
//  Copyright (c) 2013 Arthur Langereis. All rights reserved.
//

#include <cfloat>

static void testFloats() {
	group("floats", []{
		test("relations should be correct", []{
			// doubles
			check_equal(100., 100.);
			check_not_equal(99., 100.);
			
			check_ge(100., 99.);
			check_ge(100., 100.);
			
			check_gt(100., 99.);
			
			check_le(99., 100.);
			check_le(100., 100.);
			
			check_lt(99., 100.);

			// floats
			check_equal(100.f, 100.f);
			check_not_equal(99.f, 100.f);
			
			check_ge(100.f, 99.f);
			check_ge(100.f, 100.f);
			
			check_gt(100.f, 99.f);
			
			check_le(99.f, 100.f);
			check_le(100.f, 100.f);
			
			check_lt(99.f, 100.f);
		});
		
		test("failed comparisons should yield failures", []{
			MetaRun meta;
			
			test("inverse float", []{
				// doubles
				check_equal(100., 99.);
				check_not_equal(100., 100.);
				
				check_ge(99., 100.);
				
				check_gt(99., 100.);
				check_gt(100., 100.);
				
				check_le(100., 99.);
				
				check_lt(100., 99.);
				check_lt(100., 100.);

				// floats
				check_equal(100.f, 99.f);
				check_not_equal(100.f, 100.f);
				
				check_ge(99.f, 100.f);
				
				check_gt(99.f, 100.f);
				check_gt(100.f, 100.f);
				
				check_le(100.f, 99.f);
				
				check_lt(100.f, 99.f);
				check_lt(100.f, 100.f);
			});
			
			meta.run();
			
			auto metaReport = meta.report();
			check_equal(metaReport->passes(),   0);
			check_equal(metaReport->failures(), 16);
			check_equal(metaReport->errors(),   0);
		});
		
		test("templated comparisons should handle limits correctly", []{
			check_gt((float)FLT_MAX, (float)FLT_MIN);
			check_gt((double)DBL_MAX, (double)DBL_MIN);
			check_gt((long double)LDBL_MAX, (long double)LDBL_MIN);
		});
	});
}
