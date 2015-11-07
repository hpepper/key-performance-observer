/*
 * GetProcMeminfo.c
 *
 *  Created on: Jul 5, 2011
 */


#include "GetProcMeminfo.h"
#include "../Common/FunctionTracing.h"
#include "../FileOps/FileRecordHandler.h"

#include <string.h>

int GetProcMeminfo(struct VmStatistics *pStruct) {
	int nStatus = 0;

	TRACEF(5, (">>> GetProcMeminfo()\n"));

	char szLine[MAX_STATISTICS_LINE] = "";

	int nItemsFoundPattern = 0;


	// Read the rest of the data.
#define MEMFREE_BIT_PATTERN 1
#define BUFFERS_BIT_PATTERN 2
#define CACHED_BIT_PATTERN 4
#define SWAPCACHED_BIT_PATTERN 8
#define SWAPFREE_BIT_PATTERN 16

	nStatus = OpenFile("/proc/meminfo");

	int nAllThereBitPattern = MEMFREE_BIT_PATTERN
			| BUFFERS_BIT_PATTERN
			| CACHED_BIT_PATTERN
			| SWAPCACHED_BIT_PATTERN
			| SWAPFREE_BIT_PATTERN;
	TRACEF(7, ("Bit pattern:%d\n", nAllThereBitPattern));
	int nItemsFound=0;
	while ((nStatus == 0) && (nItemsFoundPattern != nAllThereBitPattern)) {
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);

		if (strncmp("MemFree:", szLine, 8) == 0) {
			nItemsFoundPattern |= MEMFREE_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 8, "%ld", &(pStruct->FreeMem));
			if (nItemsFound != 1) {
				nStatus = -MEMFREE_BIT_PATTERN;
			}
		}
		if (strncmp("Buffers:", szLine, 8) == 0) {
			nItemsFoundPattern |= BUFFERS_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 8, "%ld", &(pStruct->MemBuffers));
			if (nItemsFound != 1) {
				nStatus = -BUFFERS_BIT_PATTERN;
			}
		}
		if (strncmp("Cached:", szLine, 7) == 0) {
			nItemsFoundPattern |= CACHED_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 7, "%ld", &(pStruct->MemCached));
			if (nItemsFound != 1) {
				nStatus = -CACHED_BIT_PATTERN;
			}
		}
		if (strncmp("SwapCached:", szLine, 11) == 0) {
			nItemsFoundPattern |= SWAPCACHED_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 11, "%ld", &(pStruct->SwapCached));
			if (nItemsFound != 1) {
				nStatus = -SWAPCACHED_BIT_PATTERN;
			}
		}
		if (strncmp("SwapFree:", szLine, 9) == 0) {
			nItemsFoundPattern |= SWAPFREE_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 9, "%ld", &(pStruct->FreeSwap));
			if (nItemsFound != 1) {
				nStatus = -SWAPFREE_BIT_PATTERN;
			}
		}
	} // end while

	TRACEF(7, ("Bit pattern:%d\n", nItemsFoundPattern));

	CloseFile();

	return (nStatus);
}
