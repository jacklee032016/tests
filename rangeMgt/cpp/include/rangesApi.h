
#ifndef	__RANGES_API_H__
#define	__RANGES_API_H__

//#ifdef __cplusplus
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif


struct _RANGE_NODE;

typedef	struct _RANGE_NODE
{
	int		start;
	int 	end;

	struct _RANGE_NODE *next;
}RANGE_NODE;



EXTERNC int rangesLibInit(void);

EXTERNC int rangesAdd(int start, int end);
EXTERNC int rangesDelete(int start, int end);
EXTERNC RANGE_NODE *rangesGet(int start, int end);
EXTERNC void rangesClear(void);

EXTERNC void rangesFree(RANGE_NODE *nodes);

EXTERNC void rangesDebug(RANGE_NODE *nodes);


//#endif 

#endif

