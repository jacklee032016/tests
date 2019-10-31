/*
* C library of Range Management 
* Jack Lee, 10, 2019
*/

#include <stdlib.h>
#include <stdio.h>

#include "rangeTree.h"
#include "extLog.h"

#define	DEBUG_RANGE_RELATION(_cur, _new, _op ) \
	DEBUGF(_RANGE_DEBUG, "(%d, %d) %s (%d, %d)", (_cur)->start, (_cur)->end, (_op), (_new)->start, (_new)->end)


struct RangeSetRuntime	_rsRuntime;

RangeTree _rangeSet = NULL;


#define	IS_LAST_ONE(pos)	( NODE_NEXT(pos) == NULL)
#define	IS_FIRST_ONE(pos)	( NODE_PREV(pos) == NULL)


RR_TYPE rangeCompare(RangeType *src, RangeType *other)
{
	if(other->start == other->end )
	{
		return RR_T_INVALIDATE;
	}
		
	if(src->start >= other->end )
	{/* end is excluded from range, so >=, not > */
		return RR_T_BIGGER;
	}
	else if(src->end <= other->start )
	{/* end is excluded from range, so <=, not < */
		return RR_T_SMALLER;
	}
	else if( (src->end == other->end )
		&& (src->start == other->start) )
	{
		return RR_T_EQUAL;
	}
	else if( (src->end <= other->end && src->start > other->start ) || 
		(src->end < other->end && src->start >= other->start ) )
	{/* excluded both border; borders as leftlap or rightlap */
		return RR_T_UNDER_LAP;
	}
	else if(src->start < other->start && src->end > other->start && src->end <= other->end )
	{/* when right borders are same, it also is leftlap */
		return RR_T_LEFT_LAP;
	}
	else if(src->start >= other->start && src->end > other->end )
	{/* when left borders are same, it is rightlap	*/
		return RR_T_RIGHT_LAP;
	}

	return RR_T_FULL_LAP;

}



static int _checkParams(int start, int end)
{
	if(start >= end )
	{
		ERRORF("Parameters wrong: start(%d) > end(%d)", start, end);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int _addAtPosition(RangePosition pos, int start, int end)
{
	RangeType _newRange = {start, end};
	RangeType *currentRange = NULL;

	int count = 0;
	if(pos == NULL)
	{/* tree is null now */
		_rsRuntime.head = treeInsert(pos, start, end);
		return 1;
	}
	
	currentRange = NODE_VALUE(pos);

	RR_TYPE rrStatus = rangeCompare(currentRange, &_newRange);

	switch (rrStatus)
	{
		/* ignore new one */
		case RR_T_EQUAL:
		case RR_T_FULL_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Equal or FullOverlap");
			return count;
			break;

		/* current > new, and previous of current is smaller, so add directly */
		case RR_T_BIGGER:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Bigger");
			//pos = 
			treeInsert(pos, start, end);

			count++;
			return count;
			break;

		/* merge into existed one, eg. change start of current to new start */
		case RR_T_RIGHT_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "RightOverLap");
			/* check previos one */
			if(IS_FIRST_ONE(pos))
			{
				NODE_UPDATE_START(pos, start);
				return count;
			}

			/* the previous one is smaller than new one */
			NODE_UPDATE_START(pos, start);
			return count;
			break;

		/* remove existed(update start and end???), and continue to add this new one */
		case RR_T_UNDER_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "UnderOverLap");

			pos = treeDelete(pos);
			/* iterated loop */
			count += _addAtPosition(pos, start, end);
			return count;
			break;

		/* remove the existed, and add new one with new start */
		case RR_T_LEFT_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "LeftOverLap");
			int newStart = currentRange->start;
			
			if(IS_LAST_ONE(pos))
			{/* */
				return NODE_UPDATE_END(pos, end);
			}

			/* now right of pos is returned */
			pos = treeDelete(pos);
			
			count += _addAtPosition(pos, newStart, end);
			return count;
			break;

		/* normally this can't happen, because the first available is bigger than new one */
		case RR_T_SMALLER:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Smaller");
//			if(IS_LAST_ONE(pos) )
			{/* cur is last one */
				treeInsert(pos, start, end);
				count++; 
				return count;
			}

			TRACE();
//			pos = NODE_NEXT(pos);
			/* enhance to delete itself */
			count += _addAtPosition(pos, start, end);
			break;
		
		default:
			/* impossible here */
			ERRORF("ADD (%d, %d) is impossible", start, end );
			return count;
			break;
		
	}

	return count;
}

int rangeAdd(int start, int end)
{
	RangePosition pos = NULL;

	if(_checkParams(start, end) == EXIT_FAILURE)
	{
		return 0;
	}

	pos = treeFindFirstAvailable(_rsRuntime.head, start, end);
	if(pos == NULL)
	{/* tree is null */
		DEBUGF(_RANGE_DEBUG, "First available is NULL when adding (%d, %d)", start , end);
		_rsRuntime.head = treeInsert(_rsRuntime.head, start, end);
		return 1;
	}

	DEBUGF(_RANGE_DEBUG, "First available is (%d, %d) when adding :(%d, %d)", pos->range.start, pos->range.end, start , end);

	return _addAtPosition(pos, start, end);
}


static int _eraseAtPosition(RangePosition pos, int start, int end)
{
	RangeType _newRange = {start, end};
	RangeType *currentRange = NULL;
	int count = 0;
	
	if(pos == NULL)
	{
		return 0;
	}
	
	currentRange = NODE_VALUE(pos);
		
	RR_TYPE rrStatus = rangeCompare(currentRange, &_newRange);

	switch (rrStatus)
	{
		/* EQUAL, remove current one and return */
		case RR_T_EQUAL:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Equal");
			treeDelete(pos);
			count++;
			return count;
			break;
			
		/* current is Fulllap the remove, modify the end of current and add a new one, then return */
		case RR_T_FULL_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "FullOverlap");
			int tmp = currentRange->end;
			currentRange->end = start;
			treeInsert(pos, end, tmp);
			count--; /* add one in the process of remove */
			return count;
			break;

		/*current is underlap of remove, remove current one and continue */
		case RR_T_UNDER_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "UnderOverLap");
			pos = treeDelete(pos);
			count++;
			/* continue */
			count += _eraseAtPosition(pos, start, end);

			break;

		/*current is bigger than the remove, so ignore the left */
		case RR_T_BIGGER:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Bigger");
			return count;
			break;

		/* current is smaller than the remove, so continue to the next */
		case RR_T_SMALLER:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Smaller");
			/* continue to next one */
			pos = NODE_NEXT(pos);
			count += _eraseAtPosition(pos, start, end);
		
			break;

		/* rightLap, increase the start of the current, and return */
		case RR_T_RIGHT_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "RightOverLap");
			//_updateStart(currentRange, end);
			int tmpEnd = currentRange->end;
			pos = treeDelete(pos);
			pos = treeInsert(pos, end, tmpEnd);
			break;

		/* leftlap, decrease the end of the current, and continue to the next */
		case RR_T_LEFT_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "LeftOverLap");
			currentRange->end = start;
		
			/* go on to the next */
			pos = NODE_NEXT(pos);
			count += _eraseAtPosition(pos, start, end);
			break;
		
		default:
			/* impossible here */
			ERRORF("ERASE (%d, %d) is impossible", start, end);
			return count;
			break;
	}

	return count;
}

int rangeErase(int start, int end)
{
	RangePosition pos = NULL;

	if(_checkParams(start, end) == EXIT_FAILURE)
	{
		return 0;
	}

	pos = treeFindFirstAvailable(_rsRuntime.head, start, end);

	return _eraseAtPosition(pos, start, end);
}


static RNODE *_createItem(int start, int end)
{
	RNODE *_node = (RNODE *)malloc(sizeof(RNODE));
	ASSERT((_node!=NULL), "Memory failed");

	memset(_node, 0 , sizeof(RNODE));

	_node->range.start = start;
	_node->range.end = end;
	DEBUGF(_RANGE_DEBUG, "Add (%d, %d) into list", start, end);

	return _node;
}


#define	UPDATE_LIST(_node)	\
	do{if(first == NULL){first = (_node);} \
	if(last != NULL) { last->next = (_node);} last = (_node);}while(0)


/* iterate from the first available position */
static RNODE *_getAtPosition(RangePosition pos, int start, int end)
{
	RNODE *first = NULL, *last = NULL, *_node = NULL;
	RangeType _newRange = {start, end};
	RangeType *currentRange = NULL;

	if(pos == NULL)
	{
		return NULL;
	}
	currentRange = NODE_VALUE(pos);
		
	RR_TYPE rrStatus = rangeCompare(currentRange, &_newRange);

	switch (rrStatus)
	{
		/* current is bigger than retrieve, end this search */
		case RR_T_BIGGER:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Bigger");
			return first;
			break;
			
		/* current smaller than retrieve, continue to the next */
		case RR_T_SMALLER: 
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Smaller");
			pos = NODE_NEXT(pos);
			while(pos)
			{
				_node = _getAtPosition(pos, start, end);
				UPDATE_LIST(_node);
				pos = NODE_NEXT(pos);
			}
			break;

		/* current FullOverlap the retrieve, return current one */
		case RR_T_FULL_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "FullOverlap");
			_node = _createItem(currentRange->start, currentRange->end);

			UPDATE_LIST(_node);
			pos = NODE_NEXT(pos);
//			return getAtPosition(pos, start, end);
			return first;
			break;

		/* EQUAL: */
		case RR_T_EQUAL:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "Equal");
			_node = _createItem(currentRange->start, currentRange->end);
			UPDATE_LIST(_node);
			return first;
			break;

		/* current is underlap of the retrieve one, add the current one, and search the next */				
		case RR_T_UNDER_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "UnderOverlap");
			_node = _createItem(currentRange->start, currentRange->end);

			UPDATE_LIST(_node);
			pos = NODE_NEXT(pos);
			while(pos)
			{
				_node = _getAtPosition(pos, start, end);
				UPDATE_LIST(_node);
				pos = NODE_NEXT(pos);
			}
			break;

		/* current is leftlap of the retrieve one, add current, and search the next */
		case RR_T_LEFT_LAP:
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "LeftOverlap");
			
			_node = _createItem(currentRange->start, currentRange->end);

			UPDATE_LIST(_node);
			
			pos = NODE_NEXT(pos);
			while(pos)
			{
				_node = _getAtPosition(pos, start, end);
				UPDATE_LIST(_node);
				pos = NODE_NEXT(pos);
			}
			
			break;

		/* current is rightlap of the retrieve one, add current, and return */	
		case RR_T_RIGHT_LAP:
		{
			DEBUG_RANGE_RELATION(currentRange, &_newRange, "RightOverlap");
			
			_node = _createItem(currentRange->start, currentRange->end);

			UPDATE_LIST(_node);
			pos = NODE_NEXT(pos);
//			return first;
			break;
		}
		
		default:
			/* impossible here */
			ERRORF("GET (%d, %d) is impossible", start, end);
			return first;
			break;
		
	}

	return first;
}

RNODE *rangeGet(int start, int end)
{
	RangePosition pos = NULL;

	if(_checkParams(start, end) == EXIT_FAILURE)
	{
		return NULL;
	}

	pos = treeFindFirstAvailable(_rsRuntime.head, start, end);
#if 0
	/* _getAtPosition will iterate to use it */
	if(pos == NULL)
	{
		return NULL;
	}
#endif
	return _getAtPosition(pos, start, end);
	
}


RNODE *rangeGetAll(void)
{
	RangeTree cur = _rsRuntime.head;
	RNODE *first = NULL, *last = NULL, *_node;

	while(cur)
	{
		_node = _createItem(cur->range.start, cur->range.end);
		UPDATE_LIST(_node);
		cur = NODE_NEXT(cur);
	}

	return first;
}

int rangeClearAll(void)
{
	RangeTree cur = _rsRuntime.head;

	int i = 0;
	while(cur)
	{
		DEBUGF( _RANGE_DEBUG, "remove #%d: %p (%d, %d)", i, cur, cur->range.start, cur->range.end);
		cur = treeDelete(cur);
		i++;
	}

	DEBUGF( _RANGE_DEBUG, "total %d items cleared", i);
	_rsRuntime.head = NULL;
	return i;
}


void rangeSetDebug(void)
{
	RangeTree cur = _rsRuntime.head;

	int i = 0;
	printf("RangeSet:");
	while(cur)
	{
		if(i==0)
		{
			printf("[(%d, %d)", cur->range.start, cur->range.end);
		}
		else
		{
			printf(", (%d, %d)", cur->range.start, cur->range.end);
		}
		cur = NODE_NEXT(cur);
		i++;
	}

	printf("](total %d)"EXT_NEW_LINE, i);
	
}

void rangeListDebug(RNODE *list)
{
	RNODE *cur = list;

	int i = 0;
	printf("RNODE List:");
	while(cur)
	{
		if(i==0)
		{
			printf("[(%d, %d)", cur->range.start, cur->range.end);
		}
		else
		{
			printf(", (%d, %d)", cur->range.start, cur->range.end);
		}
		cur = cur->next;
		i++;
	}

	printf("](total %d)"EXT_NEW_LINE, i);
	
}

void rangeListFree(RNODE *list)
{
	RNODE *cur = list, *next = NULL;

	while(cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
}

int	compareRangeArray(RangeType *rangeArray, uint32_t size)
{
	RangeTree cur = _rsRuntime.head;
	RangeType *r = rangeArray;

	int i = 0;
	while(cur)
	{
		if(cur->range.start != r->start || cur->range.end != r->end )
		{
			ERRORF("#%d: (%d, %d) != (%d, %d)", i, cur->range.start, cur->range.end, r->start, r->end );
			return EXT_FALSE;
		}
		cur = NODE_NEXT(cur);
		r++;
		i++;
	}

//	if(_rsRuntime.size == size)
	if(i == size)
	{
		return EXT_TRUE;
	}

	ERRORF("Size: %d != %d", i, size );
	return EXT_FALSE;
}

int	compareRangeList(RNODE *list, RangeType *rangeArray, uint32_t size)
{
	RangeType *r = rangeArray;
	RNODE *cur = list;

	int i = 0;
	while(cur)
	{
		if( cur->range.start != r->start || cur->range.end != r->end )
		{
			ERRORF("#%d: (%d, %d) != (%d, %d)",i, cur->range.start, cur->range.end, r->start, r->end );
			return EXT_FALSE;
		}
		cur = cur->next;
		r++;
		i++;
	}

	if(i == size)
	{
		return EXT_TRUE;
	}

	ERRORF("Size: %d != %d", i, size );
	return EXT_FALSE;
}


int rangeLibInit(void)
{
	memset(&_rsRuntime, 0, sizeof(struct RangeSetRuntime) );

	return EXIT_SUCCESS;
}

