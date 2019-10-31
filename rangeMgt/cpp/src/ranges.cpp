/* 
 * Range management based on C++ Standard Library
 * by Jack Lee(jacklee032016@gmail.com), 2019
 */

#include <iostream>
#include "ranges.hpp"

using namespace std;

template <class TR>
RangeRelation Range<TR>::compare(Range<TR> & other)
{
//	cout << "start " << other.getStart()<< ":" << other.getEnd() << endl;
	if(other.getStart() == other.getEnd())
	{
		return RangeRelation::Invalidate;
	}
		
	if(this->_start >= other.getEnd())
	{/* end is excluded from range, so >=, not > */
		return RangeRelation::Bigger;
	}
	else if(this->_end <= other.getStart())
	{/* end is excluded from range, so <=, not < */
		return RangeRelation::Smaller;
	}
	else if( (this->_end == other.getEnd())
		&& (this->_start == other.getStart()) )
	{
		return RangeRelation::Equal;
	}
	else if( (this->_end <= other.getEnd() && this->_start > other.getStart()) || 
		(this->_end < other.getEnd() && this->_start >= other.getStart()) )
	{/* excluded both border; borders as leftlap or rightlap */
		return RangeRelation::Underlap;
	}
	else if(this->_start < other.getStart() && this->_end > other.getStart() && this->_end <= other.getEnd() )
	{/* when right borders are same, it also is leftlap */
		return RangeRelation::LeftOverlap;
	}
	else if(this->_start >= other.getStart() && this->_end > other.getEnd() )
	{/* when left borders are same, it is rightlap  */
		return RangeRelation::RightOverLap;
	}

	return RangeRelation::FullOverlap;
	
}

template <class T>
RangeSet<T>::RangeSet(map<T, T> m)
{
	for (auto& in : m)	
	{
		_ranges.insert({in.first, {in.first, in.second}});
	}
}


template <class T>
int RangeSet<T>::add(T start, T end)
{

#define	IS_LAST_ONE()	( next == _ranges.end() )

	int count = 0;
	Range<T> _newRange {start, end};
#ifndef __EXT_RELEASE__
	cout << "ADD: Size #" << size()<<":" << *this << "; adding :("<< start << "," << end << ")..." << endl;
#endif
	if(size() == 0)
	{
		_insert(_newRange);
		return 1;
	}
	
#ifndef __EXT_RELEASE__
	int i = 0;
#endif
	for (auto cur = _ranges.begin(); cur !=_ranges.end(); ++cur)	
	{
		Range<T> *currentRange = &cur->second;

		RangeRelation res1 = currentRange->compare(_newRange);
		auto next = cur;
		next++;

#ifndef __EXT_RELEASE__
		cout << "\tNo# " << i++ << ":" << *currentRange << endl;
#endif
		switch (res1)
		{
			case RangeRelation::Equal:
			case RangeRelation::FullOverlap:
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "Equal or FullOverlap " << _newRange << endl;
#endif
				return count;
				break;

			case RangeRelation::Bigger:
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "Bigger  " << _newRange << endl;
#endif
				if(IS_LAST_ONE())
				{
					count += _insert(_newRange);
					return count;
				}	
				break;

			case RangeRelation::RightOverLap:
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "RightOverLap  " << _newRange << endl;
#endif
				_updateStart( currentRange, start);
				return count;
				break;

			case RangeRelation::Underlap:
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "UnderOverLap  " << _newRange << endl;
#endif
				if(IS_LAST_ONE() )
				{/* replace this one */
					if(_remove(*currentRange ) != 1)
					{
						return count;
					}					
					count += _insert(_newRange);
					return count;
				}

				if(_remove(*currentRange ) != 1)
				{/**/
					return count;
				}

				/* iterated loop */
				count += add(start, end);
				return count;
				break;

			case RangeRelation::LeftOverlap:
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "LeftOverLap  " << _newRange << endl;
#endif
				if(IS_LAST_ONE())
				{
#ifndef __EXT_RELEASE__
					cout << " update end of " << currentRange << "with " <<  end << endl;
#endif
					return _updateEnd(currentRange, end);
				}
				
				if(_remove(*currentRange) != 1)
				{/**/
					return count;
				}
				
				count += add(currentRange->getStart(), end);
				return count;
				break;

			case RangeRelation::Smaller:
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "Smaller < " << _newRange << endl;
#endif
				if(IS_LAST_ONE() )
				{/* cur is last one */
					count += _insert(_newRange);
					return count;
				}
				break;
			
			default:
				/* impossible here */
				cout << "Add " << _newRange << " is impossible" << endl;
				return count;
				break;
			
		}
	}
	
	return count;
}


template <class T>
int RangeSet<T>::erase(T start, T end)
{
	int count = 0;
	Range<T> _removeRange {start, end};
#ifndef __EXT_RELEASE__
	cout << "ERASE: Size #" << size()<<":" << *this << "; erasing :("<< start << "," << end << ")..." << endl;

	int i = 0;
#endif
	for (auto cur = _ranges.begin(); cur !=_ranges.end(); ++cur)	
	{
		Range<T> *currentRange = &cur->second;

		RangeRelation res1 = currentRange->compare(_removeRange);

#ifndef __EXT_RELEASE__
		cout << "\tNo# " << i++ << ":" << *currentRange << endl;
#endif

		switch (res1)
		{
			/* EQUAL, remove current one and return */
			case RangeRelation::Equal:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << " Equal " << _removeRange << endl;
#endif
				if(_remove(*currentRange) == 1)
				{
					count++;
				}
				return count;
				break;
			}
				
			/* current is Fulllap the remove, modify the end of current and add a new one, then return */
			case RangeRelation::FullOverlap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << " FullOverlap " << _removeRange << endl;
#endif
				Range<T> tmp = {end, currentRange->getEnd()};
				_updateEnd(currentRange, start);
				_insert(tmp);
				count--; /* add one in the process of remove */
				return count;
				break;
			}	

			/*current is underlap of remove, remove current one and continue */
			case RangeRelation::Underlap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "UnderOverLap	" << _removeRange << endl;
#endif
				if(_remove(*currentRange ) != 1)
				{
					return count;
				}
				count++;/* removed one */
				break;
			}	

			/*current is bigger than the remove, so ignore the left */
			case RangeRelation::Bigger:
			{
				return count;
				break;
			}

			/* current is smaller than the remove, so continue to the next */
			case RangeRelation::Smaller:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "Smaller < " << _removeRange << endl;
#endif
				break;
			}

			/* rightLap, increase the start of the current, and return */
			case RangeRelation::RightOverLap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "RightOverLap	" << _removeRange << endl;
#endif
				_updateStart(currentRange, end);  /* ??? remove and add a new one with correct end */
				break;
			}

			/* leftlap, decrease the end of the current, and continue to the next */
			case RangeRelation::LeftOverlap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "LeftOverLap  " << _removeRange << endl;
#endif
				_updateEnd(currentRange, start);
				break;
			}

			
			default:
			{
				/* impossible here */
				cout << "Remove " << _removeRange << " is impossible" << endl;
				return count;
				break;
			}
			
		}
	}
	
	return count;
}


template <class T>
RANGES_TYPE<T>* RangeSet<T>::get(T start, T end)
{
	RANGES_TYPE<T>* _resultRs = new RANGES_TYPE<T>;

	Range<T> _retrieveRange {start, end};
#ifndef __EXT_RELEASE__
	cout << "GET: Size #" << size()<<":" << *this << "; get :("<< start << "," << end << ")..." << endl;
	int i = 0;
#endif		
	for (auto cur = _ranges.begin(); cur !=_ranges.end(); ++cur)	
	{
		Range<T> *currentRange = &cur->second;
		RangeRelation res1 = currentRange->compare(_retrieveRange);
	
#ifndef __EXT_RELEASE__
		cout << "\tNo# " << i++ << ":" << *currentRange << endl;
#endif	
		switch (res1)
		{
			/* current is bigger than retrieve, end this search */
			case RangeRelation::Bigger:
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "Bigger  " << _retrieveRange << endl;
#endif
				return _resultRs;
				break;
				
			/* current smaller than retrieve, continue to the next */
			case RangeRelation::Smaller: 
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "Smaller < " << _retrieveRange << endl;
#endif
				break;

			/* current FullOverlap the retrieve, return the range of retrieve's size ???  */
			case RangeRelation::FullOverlap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "FullOverlap " << _retrieveRange << endl;
#endif
				_resultRs->insert({currentRange->getStart(), *currentRange});
				return _resultRs;
				break;
			}
			/* EQUAL: */
			case RangeRelation::Equal:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "Equal " << _retrieveRange << endl;
#endif
				_resultRs->insert({currentRange->getStart(), *currentRange});
				return _resultRs;
				break;
			}

			/* current is underlap of the retrieve one, add the current one, and search the next */				
			case RangeRelation::Underlap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "UnderOverLap	" << _retrieveRange << endl;
#endif
				_resultRs->insert({currentRange->getStart(), *currentRange});
				break;
			}

			/* current is leftlap of the retrieve one, add last part of current, and search the next */
			case RangeRelation::LeftOverlap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "LeftOverLap  " << _retrieveRange << endl;
#endif
				_resultRs->insert({currentRange->getStart(), *currentRange});				
				break;
			}	

			/* current is rightlap of the retrieve one, add first part of current, and return */	
			case RangeRelation::RightOverLap:
			{
#ifndef __EXT_RELEASE__
				cout << "\t" << *currentRange << "RightOverLap	" << _retrieveRange << endl;
#endif
				_resultRs->insert({currentRange->getStart(), *currentRange});
				return _resultRs;
				break;
			}
			
			default:
				/* impossible here */
				cout << "GET " << _retrieveRange << " is impossible" << endl;
				return _resultRs;
				break;
			
		}
	}
	
	return _resultRs;
}

template <class T>
Range<T>& RangeSet<T>::find(T& start)
{
    return _ranges.at(start);
}


template <class T>
bool RangeSet<T>::operator==(RangeSet<T> &other)
{
	return this->operator==(other.get());
}

template <class T>
bool RangeSet<T>::operator==(RANGES_TYPE<T> *rs)
{
	if(this->size() != rs->size() )
		return false;

#ifndef __EXT_RELEASE__
	cout << "RANGE_TYPE: This: '"<< *this << "'; RangeSet:'" << *rs << "'" << endl;
#endif
	auto _rsItr = rs->begin();
//	auto range=_ranges.begin();
	for (auto r: _ranges)
	{
		if(_rsItr == rs->end() )
		{
			return false;
		}
		
		if(r.second.compare(_rsItr->second) != RangeRelation::Equal )
		{
			return false;
		}

		_rsItr++;
	}

	return true;
}


template <class T>
int RangeSet<T>::_insert(Range<T> &r)
{
//	typename std::map<T, Range<T>>::iterator ret;
	_ranges.insert({r.getStart(), r});

	return 1;
}


template <class T>
int RangeSet<T>::_remove(Range<T> &exist)
{
	int num = _ranges.erase(exist.getStart());
	if(num != 1)
	{
		cout << exist << " element(s) remove failed" << endl;
	}

	return num;
}


template <class T>
bool RangeSet<T>::_updateStart(Range<T> *exist, T start)
{
	_remove(*exist);
	
	exist->updateStart(start);
	return _insert(*exist);
}

template <class T>
bool RangeSet<T>::_updateEnd(Range<T> *exist, T end)
{
#ifndef __EXT_RELEASE__
	cout << " update end of " << *exist << "with " <<  end << endl;
#endif
	exist->updateEnd(end);
	return true;
}


template class Range<int>;

template class RangeSet<int>;
	

