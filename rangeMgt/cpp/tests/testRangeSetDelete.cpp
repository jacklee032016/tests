
#include "catch.hpp"

#include "ranges.hpp"

#include <iostream>

TEST_CASE( "RangeSet and its DELETE operations", "[delete]" ) {

    // For each section, vector v is anew:


	SECTION( "get one from two" ) {
		Range<int> r {1, 6};
		RangeSet<int> rsResult(r);
		cout << "Init: " << rsResult << endl;
		
		RangeSet<int> rsTest(r);
		cout << "Result: " << rsTest << endl;
		rsTest.erase(-3, -1);
		cout << "Result: " << rsTest << endl;
		
		REQUIRE( (rsResult == rsTest)==true );
	}

	SECTION( "remove all" ) {
		RangeSet<int> rsResult;
		cout << "Init: " << rsResult << endl;
		
		Range<int> r {1, 6};
		RangeSet<int> rsTest(r);
		rsTest.erase(-1, 10);
		cout << "Result: " << rsTest << endl;
		
		REQUIRE( (rsResult == rsTest)==true );
	}

	SECTION( "get one from two" ) {
		Range<int> r {1, 4};
		RangeSet<int> rsResult(r);
		cout << "Init: " << rsResult << endl;
		
		Range<int> r2 {1, 6};
		RangeSet<int> rsTest(r2);
		rsTest.erase(4, 10);
		cout << "Result: " << rsTest << endl;
		
		REQUIRE( (rsResult == rsTest)==true );
	}

	SECTION( "get one from two" ) {
		RangeSet<int> rsResult({{1,2}, {3, 6}});
		cout << "Init: " << rsResult << endl;
		
		Range<int> r2 {1, 6};
		RangeSet<int> rsTest(r2);
		rsTest.erase(2, 3);
		cout << "Result: " << rsTest << endl;
		
		REQUIRE( (rsResult == rsTest)==true );
	}
	
}

