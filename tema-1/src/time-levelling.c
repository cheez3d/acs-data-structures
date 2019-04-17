#include "Data.h"
#include "List.h"
#include "Point.h"

static const unsigned DELTA_MIN = 100;
static const unsigned DELTA_MAX = 1000;

struct List * time_levelling(struct List *pointList,
                             Data_print_func_t *out_print_func)
{
	if (List_GetSize(pointList) < 2) { goto end; }
	
	struct ListNode *prev = List_GetFirstNode(pointList);
	struct ListNode *curr = ListNode_GetNext(prev);
	
	while (curr) {
		struct Point *prevPoint = Point_Data_Unwrap(ListNode_GetData(prev));
		struct Point *currPoint = Point_Data_Unwrap(ListNode_GetData(curr));
		
		unsigned delta = currPoint->timestamp - prevPoint->timestamp;
		
		if ((DELTA_MIN <= delta) && (delta <= DELTA_MAX)) {
			currPoint->timestamp += prevPoint->timestamp;
			currPoint->timestamp /= 2;
			
			currPoint->value += prevPoint->value;
			currPoint->value /= 2.0;
		}
		
		prev = curr;
		curr = ListNode_GetNext(curr);
	}
	
end:
	*out_print_func = Point_Data_print_func;
	
	return pointList;
}
