/*
* API interface of range management library
* Jack Lee,  10, 2019
*/

#ifndef __RANGE_SET_H__
#define __RANGE_SET_H__

#include <stdint.h>

#ifndef __EXT_RELEASE__
#define	_RANGE_DEBUG		EXT_DBG_OFF
#else
#define	_RANGE_DEBUG		EXT_DBG_ON
#endif

typedef	enum _RR_TYPE
{
	RR_T_INVALIDATE,
	RR_T_SMALLER,
	RR_T_LEFT_LAP,
	RR_T_FULL_LAP,
	RR_T_RIGHT_LAP,
	RR_T_EQUAL,
	RR_T_UNDER_LAP,
	RR_T_BIGGER
}RR_TYPE;


typedef struct
{
	int		start;
	int 	end;
}RangeType;


struct _RNODE;

typedef	struct _RNODE
{
	RangeType 			range;
	struct _RNODE 		*next;
}RNODE;



/*************************** APIs *****************************/
int rangeAdd(int start, int end);
int rangeErase(int start, int end);
RNODE *rangeGet(int start, int end);

RNODE *rangeGetAll(void);
void rangeListFree(RNODE *list);
int rangeClearAll(void);
int rangeLibInit(void);

/* debug interface */
int	compareRangeArray(RangeType *rangeArray, uint32_t size);
int	compareRangeList(RNODE *list, RangeType *rangeArray, uint32_t size);
void rangeSetDebug(void);
void rangeListDebug(RNODE *list);

/* explose this function only for unit test */
RR_TYPE rangeCompare(RangeType *src, RangeType *other);


#endif


