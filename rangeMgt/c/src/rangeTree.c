/*
* C library of Range Management 
* Jack Lee, 10, 2019
*/

#include <stdlib.h>
#include <stdio.h>

#include "rangeTree.h"
#include "extLog.h"


RangeTree treeMakeEmpty(RangeTree _tree)
{
	if( _tree != NULL )
	{
		treeMakeEmpty( _tree->left );
		treeMakeEmpty( _tree->right );
		free(_tree );
	}
	return NULL;
}

RangePosition treeFindFirstAvailable(RangeTree tree, int start, int end)
{
	if( tree == NULL )
	{
		return NULL;
	}
	
	if(start <= tree->range.start )
	{
		if( (NODE_PREV(tree) == NULL) || (start >= NODE_PREV(tree)->range.end) )
		{/* after the left one */
			return tree;
		}

		/* continue to left */
		return treeFindFirstAvailable(tree->left, start, end);
	}
	else if(start >= tree->range.start )
	{	
#if 0		
		if(end <= tree->range.end)
		{/* within current one */
			return tree;
		}
		
		if((NODE_NEXT(tree)==NULL) || (end <= NODE_NEXT(tree)->range.start))
		{/* before the next one */
			return tree;
		}
#else
		if(start < tree->range.end )
		{/* only if current is leftLap */
			return tree;
		}
#endif
		return treeFindFirstAvailable(tree->right, start, end);
	}

	return tree;
}


RangePosition treeFind(RangeTree tree, int start)
{
	if( tree == NULL )
	{
		return NULL;
	}
	
	if(start < tree->range.start )
	{
		return treeFind(tree->left, start );
	}
	else if(start > tree->range.start )
	{
		return treeFind(tree->right, start);
	}

	return tree;
}

/* find min from current pos */
RangePosition treeFindMin(RangeTree tree)
{
	if( tree == NULL )
	{
		return NULL;
	}
	else if( tree->left == NULL )
	{
		return tree;
	}

	return treeFindMin(tree->left );
}


RangePosition treeFindMax(RangeTree tree )
{
	if(tree != NULL )
	{
		while(tree->right != NULL )
		{
			tree = tree->right;
		}
	}

	return tree;
}

static RangeTree _treeCreate( int start, int end)
{
	RangeTree tree;
	tree = malloc( sizeof(struct TreeNode ) );
	
	if(tree == NULL )
	{
		ABORT("Out of space!!!" );
	}
	else
	{
		memset(tree, 0, sizeof(struct TreeNode));
		tree->runtime = &_rsRuntime;
		tree->runtime->size++;
		
		tree->range.start = start;
		tree->range.end = end;
		tree->left = tree->right = NULL;
	
		DEBUGF(_RANGE_DEBUG, "Allocate %p for (%d, %d)", tree, start, end);
	}

	return tree;
}


/* if tree is null, return new; if tree is null, return the same pointer */
RangeTree treeInsert(RangeTree tree, int start, int end)
{
	RangeTree tmp = tree;
	
	DEBUGF(_RANGE_DEBUG, "current (%d, %d), add (%d, %d)", (tree)?tree->range.start:0, (tree)?tree->range.end:0, start, end);

	if(tree == NULL )
	{/* Create and return a one-node tree */
		tree = _treeCreate(start, end);
	}
	else if(start < tree->range.start )
	{/* decreasingly add */
		if(tree->left && tree->left->range.start < start )
		{
			tmp = _treeCreate(start, end);
			tmp->left = tree->left;
			tree->left->right = tmp;

			tree->left = tmp;
			tmp->right = tree;

			tree = tmp;
		}
		else
		{
			tmp = tree;
			tree->left = treeInsert(tree->left, start, end);
#if 1
			tree->left->right = tmp; /* add these 2 lines */
			if(tree->left->left == NULL)
			{
	//			_rangeSet = tree->left;
				tree->runtime->head = tree->left;
			}
		}
#endif		
	}
 	else if(start > tree->range.start)
 	{/* increasing add */
 		if(tree->right && tree->right->range.start > start)
 		{
 			tmp = _treeCreate(start, end);
			/* insert new one */
			tmp->right = tree->right;
			tree->right->left = tmp;

			tree->right = tmp;
			tmp->left = tree;
			
			tree = tmp;
 		}
		else
		{
			tmp = tree;
			tree->right = treeInsert( tree->right, start, end );
	//		if(tree->right->left == NULL)
			{
	//			tree->runtime->head = tree;
			}
			tree->right->left = tmp;
		}
 	}
	/* Else X is in the tree already; we'll do nothing */

	return tree;
}

/* when tree is between min or max, it return the next one; otherwise, tree keep its value of pointer; when null is return, tree is null */
RangeTree treeDelete(RangeTree tree)
{
	RangePosition tmp = NULL;

	if(tree == NULL )
	{
		ERRORF("Element not found" );
		return NULL;
	}

	DEBUGF(_RANGE_DEBUG, "Remove (%d, %d)", tree->range.start, tree->range.end);

	tree->runtime->size--;
	tmp = tree->left;
	if(tmp)
	{
		tmp->right = tree->right;
		if(tree->right)
			tree->right->left = tmp;

		
		if(tmp->right)
		{/* return right of current node when next exists */
			tmp = tmp->right;
		}
	}
	else
	{
		tmp = tree->right;
		if(tmp )
		{
			tmp->left = NULL;
			tmp->runtime->head = tmp;
		}
		else
		{/* last item */
			_rsRuntime.head = NULL;
		}
	}
	
	free(tree);
	tree = NULL;

	return tmp;
}

