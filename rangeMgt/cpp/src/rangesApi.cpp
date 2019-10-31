/*
* Interface for C language of Range Management library
*/

#include <iostream>
#include <cstring> 

#include "ranges.hpp"

#include "rangesApi.h"

using namespace std;


#define APPEND_ELEMENT(header, element, type)	\
	if ((header) == NULL){					\
		(header)  = (element);				\
		(element)->next   = NULL;			\
	}									\
	else	{ type *cur = (header), *_next = (header)->next; \
		while(_next){cur = _next; _next = cur->next;};		\
		cur->next = (element);				\
	}


template <class T>
class RangeMgr
{

	public:
		static RangeMgr *instance()
		{
        	if (!_instance)
				_instance = new RangeMgr;
			return _instance;
		}
		

		template< class T1>
		friend ostream & operator<<( ostream &out_stream, const RangeMgr<T1>& aColl );


		int add(T start, T end)
		{
			_check(start, end);
			return _rangeSet.add(_start, _end);
		}
		
		int erase(T start, T end)
		{
			_check(start, end);
			return _rangeSet.erase(_start, _end);
		}
		
		RANGE_NODE *get(T start, T end);
		void clear(void)
		{
			_rangeSet.clear();
		}

	private:

		RangeMgr(){};

		bool	_check(T start, T end)
		{
			if(start > end)
			{
				cout << " start(" << start << ") >= end(" << end << ")" ;
				_start = end;
				_end = start;
				return false;
			}
			_start = start;
			_end = end;
		
			return true;
		}
		
		T _start;
		T _end;
		
		
		RangeSet<T>			_rangeSet;

		static RangeMgr *_instance;
};


template< class T>
RangeMgr<T> *RangeMgr<T>::_instance = 0;


template< class T>
ostream & operator<<( ostream &out_stream, const RangeMgr<T>& rmgr )
{
	return out_stream;
}

template <class T>
RANGE_NODE* RangeMgr<T>::get(T start, T end)
{
	RANGE_NODE *nodes = NULL, *last = NULL;
	_check(start, end);
	RANGES_TYPE<T> *ranges =  _rangeSet.get(_start, _end);
	
	if(ranges->size()> 0 )
	{
		delete ranges;
		return nodes;
	}

//	for(auto _rsItr : ranges)
	for(auto _rsItr = ranges->begin(); _rsItr !=ranges->end(); ++_rsItr)		
	{
		RANGE_NODE *_node = (RANGE_NODE *)malloc(sizeof(RANGE_NODE));
		memset(_node, 0 , sizeof(RANGE_NODE));
		
		Range<int> r = _rsItr->second;
		_node->start = _rsItr->first;
		_node->end = r.getEnd();

		if(nodes == NULL)
		{
			nodes = _node;
		}
		
		if(last != NULL)
		{
			last->next = _node;
		}

		last = _node;
	}

	delete ranges;
	return nodes;
}


template class RangeMgr<int>;


int rangesLibInit(void)
{
	RangeMgr<int>::instance();

	return EXIT_SUCCESS;
}

int rangesAdd(int start, int end)
{
	return RangeMgr<int>::instance()->add(start, end);
}

int rangesDelete(int start, int end)
{
	return RangeMgr<int>::instance()->erase(start, end);
}

RANGE_NODE *rangesGet(int start, int end)
{
	return RangeMgr<int>::instance()->get(start, end);

}

void rangesClear(void)
{
	return RangeMgr<int>::instance()->clear();
}

void rangesFree(RANGE_NODE *nodes)
{
	RANGE_NODE *current = NULL, *next = NULL;

	current = nodes;
	while(current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	
}

void rangesDebug(RANGE_NODE *nodes)
{
	RANGE_NODE *current = NULL;
	int i = 0;

	current = nodes;
	while(current)
	{
		if(i==0)
		{
			printf("[");
		}
		else
		{
			printf(",");
		}
		printf("(%d,%d)", current->start, current->end);
		current = current->next;
		i++;
	}
	
	if(i>0)
	{
		printf("](total %d)\n\n", i);
	}
	
}

