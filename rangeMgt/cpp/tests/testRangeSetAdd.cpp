
#include "catch.hpp"

#include "ranges.hpp"

#include <iostream>

TEST_CASE( "RangeSet and its operations", "[rangeSet]" ) {

    // For each section, vector v is anew:


	SECTION( "add independently" ) {
		RangeSet<int> rsResult0 ({{1,3}, {5,6} });
		cout << "Init: " << rsResult0 << endl;
		
		RangeSet<int> rsTest;
		rsTest.add(1 ,3);
		rsTest.add(5, 6);
		cout << "Result: " << rsTest << endl;	
		
		REQUIRE( (rsResult0 == rsTest)==true );
	}

	SECTION( "add and ignore" ) {
	   	Range<int> r{2, 5};
	   	RangeSet<int> rsResult (r);
		cout << "Init: " << rsResult << endl;
			
		RangeSet<int> rsTest;
		rsTest.add(2, 5);
		rsTest.add(3, 4);
		cout << "Result: " << rsTest << endl;

		REQUIRE( (rsResult == rsTest)==true );
	}

	SECTION( "add and merge" ) {
		Range<int> r{1, 5};
		RangeSet<int> rsResult (r);
		cout << "Init: " << rsResult << endl;

		RangeSet<int> rsTest;
		rsTest.add(1, 4);
		rsTest.add(3, 5);
		cout << "Result: " << rsTest << endl;
	
		REQUIRE( (rsResult == rsTest)==true );
	}

	SECTION( "compare add2" ) {
		Range<int> r{0, 8};
		RangeSet<int> rsResult (r);
		cout << "Init: " << rsResult << endl;
			
		RangeSet<int> rsTest;
		rsTest.add(1, 3);
		rsTest.add(5, 6);
		rsTest.add(0, 8);
		cout << "Result: " << rsTest << endl;
		
		REQUIRE( (rsResult == rsTest)==true );
	}
#if 0
#endif

	SECTION( "add and merge at right side when next one is not the end " ) {
		RangeSet<int> rsResult ({{1, 3}, {8, 20}, {25, 30}});
		cout << "Init: " << rsResult << endl;

		RangeSet<int> rsTest ({{1, 3}, {8, 15}, {25, 30}});
		cout << "Before: " << rsTest << endl;
		rsTest.add(10, 20);
		cout << "Result: " << rsTest << endl;
		
		REQUIRE( (rsResult == rsTest)==true );
 	}

	SECTION( "add and merge at right size when next one is not the end " ) {
		Range<int> r ({1, 30});
		RangeSet<int> rsResult (r);
		cout << "Init: " << rsResult << endl;

		RangeSet<int> rsTest ({{1, 3}, {8, 15}, {25, 30}});
		cout << "Before: " << rsTest << endl;
		rsTest.add(2, 28);
		cout << "Result: " << rsTest << endl;
		
		REQUIRE( (rsResult == rsTest)==true );
 	}

	/* border check */
	
}

