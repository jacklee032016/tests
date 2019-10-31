/*
*
*/

#ifndef __RANGE_SEARCH_TREE_H__
#define __RANGE_SEARCH_TREE_H__

#include <stdint.h>

#include "rangeSet.h"


struct TreeNode;


struct	RangeSetRuntime
{
	struct	TreeNode		*head;
	struct  TreeNode		*middle;
	struct  TreeNode		*last;

	uint32_t				size;
	
};

struct TreeNode
{
	RangeType					range;

	struct RangeSetRuntime		*runtime;	
	
	struct TreeNode				*left;
	struct TreeNode				*right;
};


typedef struct	TreeNode *RangePosition;
typedef struct	TreeNode *RangeTree;


#define	NODE_PREV(node)		((node)->left)
#define	NODE_NEXT(node)		((node)->right)

/* return pointer */
#define	NODE_VALUE(node)	(&(node)->range)

#define	NODE_UPDATE_START(node, _start)		(node)->range.start = _start
#define	NODE_UPDATE_END(node, _end)			(node)->range.end = _end


RangeTree treeMakeEmpty( RangeTree _tree);

RangePosition treeFind(RangeTree tree, int start);
RangePosition treeFindMin(RangeTree    tree );
RangePosition treeFindMax(RangeTree    tree );

RangeTree treeInsert(RangeTree tree, int start, int end);
RangeTree treeDelete(RangeTree tree);

//RangeType* treeRetrieve(RangePosition pos);

RangePosition treeFindFirstAvailable(RangeTree tree, int start, int end);

extern	RangeTree _rangeSet;

extern	struct RangeSetRuntime	_rsRuntime;


#endif


