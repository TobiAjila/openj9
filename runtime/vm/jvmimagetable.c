#include "j9.h"
#include "j9comp.h"
#include "j9protos.h"

void * 
imageTableStartDo(ImageTableHeader* table)
{
	if (table->tableSize == 0) {
		return NULL;
	}

	table->iteratorPtr = WSRP_GET(table->tableHead, UDATA*);
	return (void *) *table->iteratorPtr;
}

void *
imageTableNextDo(ImageTableHeader* table)
{
	UDATA *tail = WSRP_GET(table->tableHead, UDATA*);
	if (table->iteratorPtr == tail) {
		return NULL;
	}

	table->iteratorPtr += 1;
	return (void*) *table->iteratorPtr;
}

UDATA * 
findEntryLocationInTable(ImageTableHeader* table, UDATA entry)
{
	UDATA* tail = WSRP_GET(table->tableTail, UDATA*);
	UDATA* head = WSRP_GET(table->tableHead, UDATA*);

	while (tail >= head) {
		if (*tail == entry) {
			return tail;
		}
		tail -= 1;
	}

	return NULL;
}