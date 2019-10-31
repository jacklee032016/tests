/* 
 * Range management based on C++ Standard Library
 * by Jack Lee(jacklee032016@gmail.com), 2019
 */

#ifndef __RANGE_HPP__
#define __RANGE_HPP__
 

#include <map>

using namespace std;

#define	TRACE()		cout << "Func:" << __FUNCTION__ << "Line: " << __LINE__ << endl

enum class RangeRelation
{
	Invalidate,
	Smaller,
	LeftOverlap,
	FullOverlap,
	RightOverLap,
	Equal,
	Underlap,
	Bigger
};


template <class T>
class Range
{

	public:
		Range(T start, T end):
			_start(start),
			_end(end)
			{
			};

		Range():
			_start(0),
			_end(0)
		{
		};

#if 0
		Range( Range<T> &other):
			_start(other.getStart()),
			_end(other.getEnd())
			{};
#endif			

		RangeRelation compare(Range<T> &other);	

		int updateStart(T start)
		{
			if(start > _end)
			{
				return EXIT_FAILURE;
			}
			
			_start = start;
			return EXIT_SUCCESS;
		};


		int	updateEnd(T end)
		{
			if(end < _start)
			{
				return EXIT_FAILURE;
			}
			
			_end = end;
			return EXIT_SUCCESS;
		};

		T getStart(void)
		{
			return _start;
		}

		T getEnd(void)
		{
			return _end;
		}


		template< class T1>
		friend ostream & operator<<( ostream &out_stream, const Range<T1>& ran );

	private:
		T _start;
		T _end;
};


template< class T>
ostream & operator<<( ostream &out_stream, const Range<T>& ran )
{
   	out_stream << "(" << ran._start << "," << ran._end << ") " ;

	return out_stream;
}

template <class T>
using RANGES_TYPE = map<T, Range<T>>;


template< class T>
ostream & operator<<( ostream &out_stream, const RANGES_TYPE<T>& rt )
{
//   	out_stream << "RT size:" << rt.size() << " " << endl;

	out_stream << "[" ;
	for( typename RANGES_TYPE<T>::const_iterator it = rt.begin(); it != rt.end(); ++it )
	{
		out_stream << it->second ;
	}
	out_stream << "]" ;

	return out_stream;
}

template <class T>
class RangeSet
{

	public:
		RangeSet(){};
		RangeSet(Range<T> &r){add(r);};
		RangeSet(map<T, T>); /* interface only for create result, eg. not following rule of Range Management */

		int size()
		{
			return _ranges.size();
		}

		void clear()
		{
			_ranges.clear();
		}

		int add(Range<T>& r)
		{
			return add(r.getStart(), r.getEnd());
		}

		/* return value of add() and erase(): how many range are added or deleted.
		* for example, 
		* with add(): 
		*           return 1, means one range added; 
		*           return -1, means one range is deleted in this process of add;
		*           return 0, means size is not changed, but some ranges may be modified, or some errors happened;
		* with erase(): 
		*           return 1, means one raange is deleted; 
		*           return -1, means one range is added in this process of delete;
		*           return 0: means size is not changed, but some ranges may be modified;
		*/
		int add(T start, T end);
		int erase(T start, T end);
		RANGES_TYPE<T> *get(T start, T end);

		/* return itself, object return by this function can't be free */
		RANGES_TYPE<T> *get()
		{
			return &_ranges;
		}

		Range<T>& find(T& start);

		bool operator==(RangeSet<T> &other);

		bool operator==(RANGES_TYPE<T> *rs);


		template< class TS>
		friend ostream & operator<<( ostream &out_stream, const RangeSet<TS> &rs );

	private:
		RANGES_TYPE<T>			_ranges;

		int _insert(Range<T> &r);
		int _remove(Range<T> &r);
		
		bool _updateStart(Range<T> *exist, T start);
		bool _updateEnd(Range<T> *exist, T end);

};


template< class T>
ostream & operator<<( ostream &out_stream, const RangeSet<T> &rs )
{
//	out_stream << rs.size() << " items:" << endl;
	
	out_stream << "[" ;
	for( typename map<T, Range<T>>::const_iterator it = rs._ranges.begin(); it != rs._ranges.end(); ++it )
	{
    	out_stream << it->second ;
	}
	out_stream << "]" ;
//	out_stream << "]" << endl;

	return out_stream;
}


#endif

