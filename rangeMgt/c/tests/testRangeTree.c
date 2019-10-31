
#include <stdio.h>
#include <stdlib.h>

#include "rangeSet.h"
#include "extLog.h"

#ifndef __EXT_RELEASE__
	#define	RANGE_SET_DEBUG() 			rangeSetDebug()
	#define RANGE_LIST_DEBUG(list)		rangeListDebug(list)
#else
	#define RANGE_SET_DEBUG()			{}
	#define RANGE_LIST_DEBUG(list)			{}
#endif

#define	RANGE_ASSIGN(_rng, _start, _end)	\
	do{(_rng)->start = (_start); (_rng)->end = (_end);}while(0)


#define	TEST_RANGE_COMPARE(_src, _other, _res, _op ) \
	do{ int ret = ((rangeCompare((_src), (_other)) == (_res))); \
		INFOF("\t%s : (%d, %d) " _op " (%d, %d)", (ret==0)?"Failed":"OK", (_src)->start, (_src)->end , (_other)->start, (_other)->end)}while(0)


#define	TEST_RANGE_ARRAY(array, format, msg...) \
	do{if(compareRangeArray((array), EXT_ARRAYSIZE((array)) )== EXT_FALSE){ERRORF(format": FAILED", ##msg);} \
		else{INFOF(format": OK", ##msg)}; }while(0) 

static int testRangeAdd(void)
{
	RNODE *list;

	RangeType resultArray[]=
	{
		{1, 3}, 
		{5, 6},
		{7, 8}
	};

	INFOF(EXT_NEW_LINE EXT_NEW_LINE"***********************************"EXT_NEW_LINE"\t Begin to ADD test cases..."EXT_NEW_LINE);
	/* case 1: add indepdently */	
	INFOF("#1: Begin to test Add increasingly...");
	rangeAdd(1 ,3);
	rangeAdd(5, 6);
	rangeAdd(7, 8);

//	rangeAdd(13, 12); /* test wrong params */

	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray, "#1: Add increasingly");
	list = rangeGetAll();
	RANGE_LIST_DEBUG(list);
	rangeListFree(list);
	rangeClearAll();

	INFOF("#2: Begin to test Add decreasingly...");
	rangeAdd(7, 8);
	rangeAdd(5, 6);
	rangeAdd(1 ,3);
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray, "#2: Add decreasingly");
	rangeClearAll();


	INFOF("#3: Begin to test Add disorderly...");
	rangeAdd(7, 8);
	rangeAdd(1 ,3);
	rangeAdd(5, 6);
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray, "#3: Add disorderly");
	rangeClearAll();

	INFOF("#4: Begin to test Add messedly2...");
	rangeAdd(1 ,3);
	rangeAdd(7, 8);
	rangeAdd(5, 6);
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray, "#4: Add messedly2");
	rangeClearAll();


	/* case 2: add is ignored */	
	INFOF("#5: Begin to test Add FullLap and repeatedly...");
	RangeType resultArray2[]=
	{
		{2, 5}
	};
		
	rangeAdd(3, 4);
	rangeAdd(2, 5);
	rangeAdd(2, 5);
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray2, "#5: Add FullLap and repeatedly");
	rangeClearAll();


	/* case 3: add and merge */	
	INFOF("#6: Begin to test Add and Merge...");
	RangeType resultArray3[]=
	{
		{1, 5}
	};
	rangeAdd(1, 4);
	rangeAdd(3, 5);
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray3, "#6: Add and Merge");
	rangeClearAll();


	/* merge more than 2 */
	INFOF("#7: Begin to test Add and Merge other 2 ranges...");
	RangeType resultArray4[]=
	{
		{0, 8}
	};
	rangeAdd(1, 3);
	rangeAdd(5, 6);
	rangeAdd(0, 8);
	RANGE_SET_DEBUG();
		
	TEST_RANGE_ARRAY(resultArray4, "#7: Add and Merge other 2 ranges");
	rangeClearAll();


	/* merge more than 2 */
	INFOF("#8: Begin to test Add and Merge in the middle...");
	RangeType resultArray5[]=
	{
		{1, 3},
		{8, 20},
		{25, 30}
	};
	
	rangeAdd(1, 3);
	rangeAdd(8, 12);
	rangeAdd(15, 16);
	rangeAdd(25, 30);
	RANGE_SET_DEBUG();
	
	rangeAdd(10, 20);
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray5, "#8: Add and Merge in the middle");
	rangeClearAll();


	INFOF("#9: Begin to test Add and Merge all...");
	RangeType resultArray6[]=
	{
		{1, 30}
	};
	
	rangeAdd(1, 3);
	rangeAdd(8, 15);
	rangeAdd(25, 30);
	RANGE_SET_DEBUG();
	
	rangeAdd(2, 28);
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray6, "#9: Add and Merge all");
	rangeClearAll();

	return EXIT_SUCCESS;
}


static int testRangeDelete(void)
{
	RNODE *list;

	RangeType resultArray[]=
	{
		{1, 6}
	};

	INFOF(EXT_NEW_LINE EXT_NEW_LINE"***********************************"EXT_NEW_LINE"\t Begin to DELETE test cases..."EXT_NEW_LINE);
	/* case 1: add indepdently */	
	INFOF("#1: Begin to test DELETE ignore...");
	rangeAdd(1 ,6);
	rangeErase(-3, -1);
	
	RANGE_SET_DEBUG();
	TEST_RANGE_ARRAY(resultArray, "#1: DELETE ignore");
	list = rangeGetAll();
	RANGE_LIST_DEBUG(list);
	rangeListFree(list);
	rangeClearAll();



	INFOF("#2: Begin to test DELETE: remove all across multiple");
	RangeType resultArray2[]=
	{
	};
	
	rangeAdd(1, 6);
	rangeErase(-1, 10);
	TEST_RANGE_ARRAY(resultArray2, "#2: DELETE remove all across multiple");
	rangeClearAll();


	INFOF("#3: Begin to test DELETE: remove part of one range");
	RangeType resultArray3[]=
	{
		{1, 4},
		{16, 20}
	};

	rangeAdd(1, 6);
	rangeAdd(8, 11);
	rangeAdd(14, 20);
	rangeErase(4, 16);
	TEST_RANGE_ARRAY(resultArray3, "#3: DELETE remove part of one range");
	RANGE_SET_DEBUG();
	rangeClearAll();


#if 1
	INFOF("#4: Begin to test DELETE: remove one range to create two");
	RangeType resultArray4[]=
	{
		{1, 2}, 
		{3, 6}
	};
	
	rangeAdd(1, 6);
	rangeErase(2, 3);
	TEST_RANGE_ARRAY(resultArray4, "#4: DELETE remove one range to create two");
	RANGE_SET_DEBUG();
	rangeClearAll();

	INFOF("#5: Begin to test DELETE: Left, Under and Right lap");
	RangeType resultArray5[]=
	{
		{1, 4}, 
		{16, 18},
		{21, 25}
	};
	
	rangeAdd(1, 6);
	rangeAdd(9, 12);
	rangeAdd(14, 18);
	rangeAdd(21, 25);
	rangeErase(4, 16);
	TEST_RANGE_ARRAY(resultArray5, "#5: DELETE Left and Right lap");
	RANGE_SET_DEBUG();
	rangeClearAll();
#endif

	INFOF("#6: Begin to test DELETE: make some mistake to sure test is OK");
	RangeType resultArray6[]=
	{
		{1, 4}, 
		{16, 19},
		{21, 25}
	};
	
	rangeAdd(1, 6);
	rangeAdd(9, 12);
	rangeAdd(21, 25);
	rangeErase(4, 16);
	TEST_RANGE_ARRAY(resultArray6, "#5: DELETE make some mistake to sure test itself is OK");
	RANGE_SET_DEBUG();
	rangeClearAll();

	return EXIT_SUCCESS;
}


#define	TEST_RANGE_GET(list, array, format, msg...) \
	do{if(compareRangeList((list), (array), EXT_ARRAYSIZE((array)) )== EXT_FALSE){ERRORF(format": FAILED", ##msg);} \
		else{INFOF(format": OK", ##msg)}; }while(0) 

static int testRangeGet(void)
{
	RNODE *list;

	INFOF(EXT_NEW_LINE EXT_NEW_LINE"***********************************"EXT_NEW_LINE"\t Begin to GET test cases..."EXT_NEW_LINE);
	RangeType resultArray[]=
	{
	};

	INFOF("GET #1: get with no result...");
	rangeAdd(1,3);
	rangeAdd(5,7);
	
	list = rangeGet(4, 5);
	RANGE_LIST_DEBUG(list);
	
	TEST_RANGE_GET(list, resultArray, "#1: get with no result");
	rangeClearAll();
	rangeListFree(list);



	INFOF("GET #2: get one from two..." );
	RangeType resultArray2[]=
	{
		{5, 6}
	};
	
	rangeAdd(1,3);
	rangeAdd(5,6);
	
	list = rangeGet(4, 6);
	RANGE_LIST_DEBUG(list);
	
	TEST_RANGE_GET(list, resultArray2, "#2: get one from two");
	rangeClearAll();
	rangeListFree(list);


	INFOF("GET #3: get across multiple" );
	RangeType resultArray3[]=
	{
		{1, 3}, 
		{5, 6}
	};
		
	rangeAdd(1, 3);
	rangeAdd(5, 6);

	list = rangeGet(2, 9);
	RANGE_LIST_DEBUG(list);
	
	TEST_RANGE_GET(list, resultArray3, "#3: get across multiple");
	rangeClearAll();
	rangeListFree(list);

	return EXIT_SUCCESS;
}


static int testRangeCompare(void)
{
	RangeType rResult = {1, 5};

	INFOF("Test Range QUAL");
	RangeType rTest = {1, 5};
	TEST_RANGE_COMPARE( &rResult, &rTest, RR_T_EQUAL, "==" );

	/* Underlap */
	INFOF("Test Range UNDER OVERLAP");
	RangeType rTest2 = {-1, 6};
	TEST_RANGE_COMPARE( &rResult, &rTest2, RR_T_UNDER_LAP, "><");
	
	/* border */
	RANGE_ASSIGN(&rTest2, -1, 5);
	TEST_RANGE_COMPARE( &rResult, &rTest2, RR_T_UNDER_LAP, "><");
	
	RANGE_ASSIGN(&rTest2, 1, 6);
	TEST_RANGE_COMPARE( &rResult, &rTest2, RR_T_UNDER_LAP, "><");


	INFOF("Test Range SMALLER");
//	RangeType rResult = {1, 5};
	RangeType rTestSmall = {6, 9};
	TEST_RANGE_COMPARE( &rResult, &rTestSmall, RR_T_SMALLER, "<");

	/* border of smaller */
	RANGE_ASSIGN(&rTestSmall, 5, 7);
	TEST_RANGE_COMPARE( &rResult, &rTestSmall, RR_T_SMALLER, "<");


	INFOF("Test Range BIGGER");
//	RangeType rResult = {1, 5};
	RangeType rTestBigger = {-3, 0};
	TEST_RANGE_COMPARE( &rResult, &rTestBigger, RR_T_BIGGER, ">");

	/* border of bigger */	
	RANGE_ASSIGN(&rTestBigger, -3, 1);
	TEST_RANGE_COMPARE( &rResult, &rTestBigger, RR_T_BIGGER, ">");


	INFOF("Test Range LeftOverlap" );
	RangeType rTestLeft = {3, 6};	 
	TEST_RANGE_COMPARE(&rResult, &rTestLeft, RR_T_LEFT_LAP, "<=");

	/* border of leftlap */
	RANGE_ASSIGN(&rTestLeft, 2, 5);
	TEST_RANGE_COMPARE(&rResult, &rTestLeft, RR_T_LEFT_LAP, "<=" );


	INFOF("Test Range RightOverlap");
	RangeType rTestRight = {-1, 3};
	TEST_RANGE_COMPARE(&rResult, &rTestRight, RR_T_RIGHT_LAP, ">=");

	RANGE_ASSIGN(&rTestRight, 1, 4);
	TEST_RANGE_COMPARE( &rResult, &rTestRight, RR_T_RIGHT_LAP, ">=");
	 

	INFOF("Test Range FullOverlap" );
	RangeType rTestFull = {2, 4};	 
	TEST_RANGE_COMPARE(&rResult, &rTestFull, RR_T_FULL_LAP, "<>");

	/* border : is not FullLap */
	RANGE_ASSIGN(&rTestFull, 1, 4);	 
	TEST_RANGE_COMPARE( &rResult, &rTestFull, RR_T_RIGHT_LAP, ">=" );

	RANGE_ASSIGN(&rTestFull, 2, 5);	 
	TEST_RANGE_COMPARE( &rResult, &rTestFull, RR_T_LEFT_LAP, "<=");

	/* this is a failed case */
	RANGE_ASSIGN(&rTestFull, 2, 5);	 
	TEST_RANGE_COMPARE( &rResult, &rTestFull, RR_T_FULL_LAP, "<>");

	return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
	argv = argv;
	argc = argc;
	
	rangeLibInit();

	testRangeCompare();

	testRangeAdd();

	testRangeGet();

	testRangeDelete();

	return EXIT_SUCCESS;
}

