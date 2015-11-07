/*
 * GetProcVmstat.c
 *
 *  Created on: Jul 3, 2011
 */

#include "GetProcVmstat.h"
#include "../Common/FunctionTracing.h"
#include "../FileOps/FileRecordHandler.h"

#include <string.h>

int GetProcVmstat(struct VmStatistics *pStruct) {
	int nStatus = 0;

	TRACEF(5, (">>> GetProcVmstat()\n"));

	char szLine[MAX_STATISTICS_LINE] = "";

	int nItemsFoundPattern = 0;

	unsigned long nPgscanDirectDma = 0;
	unsigned long nPgscanDirectDma32 = 0;
	unsigned long nPgscanDirectNormal = 0;
	unsigned long nPgscanDirectMovable = 0;

	// Read the rest of the data.
#define PGSCAN_DIRECT_DMA_BIT_PATTERN 1
#define PGSCAN_DIRECT_DMA32_BIT_PATTERN 2
#define PGSCAN_DIRECT_NORMAL_BIT_PATTERN 4
#define PGSCAN_DIRECT_MOVABLE_BIT_PATTERN 8
#define PGMJFAULT_BIT_PATTERN 16
#define PSWPIN_BIT_PATTERN 32
#define PSWPOUT_BIT_PATTERN 64

	nStatus = OpenFile("/proc/vmstat");

	int nAllThereBitPattern = PGSCAN_DIRECT_DMA_BIT_PATTERN
			| PGSCAN_DIRECT_DMA32_BIT_PATTERN
			| PGSCAN_DIRECT_NORMAL_BIT_PATTERN
			| PGSCAN_DIRECT_MOVABLE_BIT_PATTERN
			| PGMJFAULT_BIT_PATTERN
			| PSWPIN_BIT_PATTERN
			| PSWPOUT_BIT_PATTERN;
	TRACEF(7, ("Bit pattern:%d\n", nAllThereBitPattern));
	int nItemsFound=0;
	while ((nStatus == 0) && (nItemsFoundPattern != nAllThereBitPattern)) {
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);

		if (strncmp("pgscan_direct_dma ", szLine, 18) == 0) {
			nItemsFoundPattern |= PGSCAN_DIRECT_DMA_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 18, "%ld", &(nPgscanDirectDma));
			if (nItemsFound != 1) {
				nStatus = -PGSCAN_DIRECT_DMA_BIT_PATTERN;
			}
		}
		if (strncmp("pgscan_direct_dma32 ", szLine, 20) == 0) {
			nItemsFoundPattern |= PGSCAN_DIRECT_DMA32_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 20, "%ld", &(nPgscanDirectDma32));
			if (nItemsFound != 1) {
				nStatus = -PGSCAN_DIRECT_DMA32_BIT_PATTERN;
			}
		}
                // TODO V validate that this is Ubuntus version of _dma32.
		if (strncmp("pgscan_direct_high ", szLine, 19) == 0) {
			nItemsFoundPattern |= PGSCAN_DIRECT_DMA32_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 19, "%ld", &(nPgscanDirectDma32));
			if (nItemsFound != 1) {
				nStatus = -PGSCAN_DIRECT_DMA32_BIT_PATTERN;
			}
		}
		if (strncmp("pgscan_direct_normal ", szLine, 21) == 0) {
			nItemsFoundPattern |= PGSCAN_DIRECT_NORMAL_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 21, "%ld", &(nPgscanDirectNormal));
			if (nItemsFound != 1) {
				nStatus = -PGSCAN_DIRECT_NORMAL_BIT_PATTERN;
			}
		}
		if (strncmp("pgscan_direct_movable ", szLine, 22) == 0) {
			nItemsFoundPattern |= PGSCAN_DIRECT_MOVABLE_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 22, "%ld", &(nPgscanDirectMovable));
			if (nItemsFound != 1) {
				nStatus = -PGSCAN_DIRECT_MOVABLE_BIT_PATTERN;
			}
		}
		if (strncmp("pgmajfault ", szLine, 11) == 0) {
			nItemsFoundPattern |= PGMJFAULT_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 11, "%ld", &(pStruct->MajorPageFault));
			if (nItemsFound != 1) {
				nStatus = -PGMJFAULT_BIT_PATTERN;
			}
		}
		if (strncmp("pswpin ", szLine, 7) == 0) {
			nItemsFoundPattern |= PSWPIN_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 7, "%ld", &(pStruct->PageSwapIn));
			if (nItemsFound != 1) {
				nStatus = -PSWPIN_BIT_PATTERN;
			}
		}
		if (strncmp("pswpout ", szLine, 8) == 0) {
			nItemsFoundPattern |= PSWPOUT_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 8, "%ld", &(pStruct->PageSwapOut));
			if (nItemsFound != 1) {
				nStatus = -PSWPOUT_BIT_PATTERN;
			}
		}
	} // end while

	pStruct->PageScanned = nPgscanDirectDma + nPgscanDirectDma32 + nPgscanDirectNormal + nPgscanDirectMovable;
	TRACEF(7, ("Bit pattern planned:%d, Bit pattern not found:%d, scans %lu\n", nItemsFoundPattern, nAllThereBitPattern-nItemsFoundPattern, pStruct->PageScanned));

	CloseFile();

	return (nStatus);
}
