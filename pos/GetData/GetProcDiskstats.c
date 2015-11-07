/*
 * GetProcDiskstats.c
 *
 *  Created on: Jul 4, 2011
 */

#include "GetProcDiskstats.h"

#include "../Common/FunctionTracing.h"
#include "../FileOps/FileRecordHandler.h"

#include <string.h>

int InterpretStorageLine(struct StorageStatistics *pStorageStatsStruct, char *szLine) {
	int nStatus = -1;
	unsigned long nField2;
	unsigned long nField6;

	int nItemsFound = sscanf(szLine, "%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
			&(pStorageStatsStruct->rd),
			&nField2,
			&(pStorageStatsStruct->nrd),
			&(pStorageStatsStruct->rtime),
			&(pStorageStatsStruct->wr),
			&nField6,
			&(pStorageStatsStruct->nwr),
			&(pStorageStatsStruct->wtime),
			&(pStorageStatsStruct->ioinprogress),
			&(pStorageStatsStruct->iotime),
			// TODO C Should this be normalized or does rrd take care of that? would it be the same choise for rtime and wtime?
			&(pStorageStatsStruct->weightediotime)
			);
	if ( nItemsFound == 11 ) {
		nStatus = 0;
	}
	// TRACEF(7,("    InterpretStorageLine() ItemsFound=%d  %ld %ld %ld %ld %ld\n", nItemsFound, nUserMode, nUserModeNiced, nSystem, nIdel, nIOWait));
	return (nStatus);
} // end interpretStorageline.


/**
 * TODO V Verify that an errors is detected if 2 disks are expected and only one found.
 */
int GetProcDiskstats(struct StorageInformationStructure * pStorageInformationStruct) {
	int nStatus = 0;
	int nDiskNameOffset=13;

	char szLine[MAX_STATISTICS_LINE] = "";
	char szStorageString[PROC_STAT_ITEM_STRING_LENGTH] = "";

	TRACEF(5,(">>> GetProcStorageDev()\n"));

	nStatus = OpenFile("/proc/diskstats");

	int nStorageIndex = 0;
	// This whole construction, operates on the assumption that Storages are listed in ascending order in the file.
	while ((nStatus == 0) && (nStorageIndex
			< pStorageInformationStruct->nNumberOfDisksSupported)) {
		//  Read the StorageN line
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);
		if (nStatus == 0) {
			snprintf(szStorageString, PROC_STAT_ITEM_STRING_LENGTH, "%cd%c ", pStorageInformationStruct->cDiskType,
					'a' + (char)nStorageIndex);
			TRACEF(7,("   GetProcDiskstats() find '%s' in \"%s\"\n", szStorageString, szLine+nDiskNameOffset));
			if (strncmp(szStorageString, szLine+nDiskNameOffset, strlen(szStorageString)) == 0) {
				TRACEF(7,("%s: '%s'\n", szStorageString, szLine));
				InterpretStorageLine(
						pStorageInformationStruct->arStorageStatisticsPointerArray[nStorageIndex],
						szLine + nDiskNameOffset + strlen(szStorageString));
				nStorageIndex++;
			}
			TRACEF(7,("GetProcStorageDev() nStatus=%d, nStorageIndex=%d\n", nStatus, nStorageIndex));
		} // endif status
	} // end while.
	CloseFile();

	return (nStatus);
} // end getprocstat.
