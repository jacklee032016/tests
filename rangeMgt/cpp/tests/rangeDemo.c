/* 
 * Range management based on C++ Standard Library
 * by Jack Lee(jacklee032016@gmail.com), 2019
 */
 
#include <stdlib.h>

#include "rangesApi.h"


static int testAdd(void)
{
	rangesAdd(1, 3);

	rangesAdd(5, 6);
	
	rangesAdd(7, 9);

	return EXIT_SUCCESS;
}


static int testDelete(void)
{
	rangesDelete(1, 3);
    
	rangesDelete(1, 3);

	return EXIT_SUCCESS;
}

static int testGet(void)
{
	RANGE_NODE *ranges = rangesGet(0, 20);
	rangesDebug(ranges);
	rangesFree(ranges);
	

	ranges = rangesGet(5, 6);
	rangesDebug(ranges);
	rangesFree(ranges);

	return EXIT_SUCCESS;
}


int main()
{
	testAdd();

	testGet();

	testDelete();
	
	return EXIT_SUCCESS;
}

