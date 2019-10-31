
#include "catch.hpp"

#include "ranges.hpp"
#include <iostream>

TEST_CASE( "Range and its comparason", "[range]" ) {

    // For each section, vector v is anew:

 	
	SECTION( "compare invalidate" ) {
		Range<int> r1 {1, 5};
		//RANGES_TYPE<int> _ranges;
//		map<int, Range<int>>	_ranges;
//		auto r2 =_ranges.begin();
//		auto _end =_ranges.end();
			
//		REQUIRE( r1.compare(r2->second) == RangeRelation::Invalidate);
//		REQUIRE( r1.compare(_end->second) == RangeRelation::Invalidate);
		
	}

	SECTION( "compare equal" ) {
		Range<int> rResult {1, 5};

		Range<int> rTest1 {1, 5};
		cout << rResult << " == " << rTest1 << endl;
		REQUIRE( rResult.compare(rTest1) == RangeRelation::Equal);

	}
	
	SECTION( "compare underlay" ) {
		Range<int> rResult {1, 5};

		Range<int> rTest {-1, 6};
		cout << rResult << " >< " << rTest << endl;
		REQUIRE( rResult.compare(rTest) == RangeRelation::Underlap);

		/* border */
		Range<int> rTest2 {-1, 5};
		cout << rResult << " >< " << rTest2 << endl;
		REQUIRE( rResult.compare(rTest2) == RangeRelation::Underlap);
		
		Range<int> rTest3 {1, 6};
		cout << rResult << " >< " << rTest3 << endl;
		REQUIRE( rResult.compare(rTest3) == RangeRelation::Underlap);
	}

     SECTION( "compare Smaller" ) {
		Range<int> rResult {1, 5};
		
        Range<int> rTest {6, 9};
		cout << rResult << " < " << rTest << endl;
        REQUIRE( rResult.compare(rTest) == RangeRelation::Smaller );

		/* border of smaller */
		Range<int> rTest2 {5, 7};		
		cout << rResult << "<" << rTest2 << endl;
		REQUIRE( rResult.compare(rTest2) == RangeRelation::Smaller );

    }

	SECTION( "compare Bigger" ) {
		Range<int> rResult {1, 5};
		 
		Range<int> rTest {-3, 0};
		cout << rResult << ">" << rTest << endl;
		REQUIRE( rResult.compare(rTest) == RangeRelation::Bigger );

		/* border of bigger */	
		Range<int> rTest2 {-3, 1};
		cout << rResult << ">" << rTest2 << endl;
		REQUIRE( rResult.compare(rTest2) == RangeRelation::Bigger );
	}


	SECTION( "compare LeftOverlap" ) {
		Range<int> rResult {1, 5};
		 
		Range<int> rTest {3, 6};	 
		cout << rResult << "<=" << rTest << endl;
		REQUIRE( rResult.compare(rTest) == RangeRelation::LeftOverlap );

		/* border of leftlap */
		Range<int> rTest2 {2, 5};	 
		cout << rResult << "<=" << rTest2 << endl;
		REQUIRE( rResult.compare(rTest2) == RangeRelation::LeftOverlap );
	}

	SECTION( "compare RightOverlap" ) {
		  Range<int> rResult {1, 5};
		  
		  Range<int> rTest {-1, 3};
		  cout << rResult << ">=" << rTest << endl;
		  REQUIRE( rResult.compare(rTest) == RangeRelation::RightOverLap );
	
		  Range<int> rTest2 {1, 4};
		  cout << rResult << ">=" << rTest2 << endl;
		  REQUIRE( rResult.compare(rTest2) == RangeRelation::RightOverLap );
	  }
	 
	SECTION( "compare FullOverlap" ) {
		Range<int> rResult {1, 5};
		 
		Range<int> rTest {2, 4};	 
		cout << rResult << "<>" << rTest << endl;
		REQUIRE( rResult.compare(rTest) == RangeRelation::FullOverlap );

		/* border : is not FullLap */
		Range<int> rTest2 {1, 4};	 
		cout << rResult << "<>" << rTest2 << endl;
		REQUIRE( rResult.compare(rTest2) == RangeRelation::RightOverLap );

		Range<int> rTest3 {2, 5};	 
		cout << rResult << "<>" << rTest3 << endl;
		REQUIRE( rResult.compare(rTest3) == RangeRelation::LeftOverlap );
	}

  	 
}

