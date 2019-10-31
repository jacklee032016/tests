
#include "catch.hpp"

#include "ranges.hpp"

#include <iostream>

TEST_CASE( "RangeSet and its GET operations", "[get]" ) {

    // For each section, vector v is anew:


	SECTION( "get with no result" ) {
		RangeSet<int> rsResult;
		cout << "Init: " << rsResult << endl;
		
		RangeSet<int> rsTest({{1,3}, {5,7}});
		cout << "Result: " << rsTest << endl;
		RANGES_TYPE<int> *rsGet = rsTest.get(4, 5);
		
		REQUIRE( (rsResult == rsGet)==true );
	}

	SECTION( "get one from two" ) {
		Range<int> r {5, 6};
		RangeSet<int> rsResult(r);
		cout << "Init: " << rsResult << endl;
		
		RangeSet<int> rsTest({{1,3}, {5,6}});
		cout << "Result: " << rsTest << endl;
		RANGES_TYPE<int> *rsGet = rsTest.get(4, 6);
		
		REQUIRE( (rsResult == rsGet)==true );
	}

	SECTION( "get with no result" ) {
//		Range<int> r {5, 6};
		RangeSet<int> rsResult({{1,3}, {5,6}});
		cout << "Init: " << rsResult << endl;
		
		RangeSet<int> rsTest({{1,3}, {5,6}});
		cout << "Result: " << rsTest << endl;
		RANGES_TYPE<int> *rsGet = rsTest.get(2, 9);
		
		REQUIRE( (rsResult == rsGet)==true );
	}


	/* border check */
	
}

