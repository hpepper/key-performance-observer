/*
 * GetProcStat.c
 *
 *  Extract the relevant data from the /proc/stat file.
 *
 *  Created on: Jun 15, 2011
 *
 */

#include "GetProcStat.h"

#include "../Common/FunctionTracing.h"
#include "../FileOps/FileRecordHandler.h"

#include <string.h>

/**
 * Col0: User mode
 * Col1: user mode (niced)
 * Col2: sys
 * Col3: idle.
 * Col4: io wait
 * Col5: intr service
 * Col6: softirq service
 * Col7: steal - time spent in other operating systems when running in a virtualized environment
 * Col8: guest - time spent running a virtual CPU  for  guest os
 */
int InterpretCpuLine(struct CpuStatistics *pCpuStatsStruct, char *szLine,
		long nClockTicksPerSecond) {
	int nStatus = -1;
	/*
	 long nUserMode=0;
	 long nUserModeNiced=0;
	 long nSystem=0;
	 long nIdel=0;
	 long nIOWait=0;
	 */
	int nItemsFound = sscanf(szLine, "%ld %ld %ld %ld %ld %ld %ld %ld %ld",
			&(pCpuStatsStruct->User), &(pCpuStatsStruct->UseNice),
			&(pCpuStatsStruct->System), &(pCpuStatsStruct->Idle),
			&(pCpuStatsStruct->IoWait), &(pCpuStatsStruct->Irqs),
			&(pCpuStatsStruct->SoftIrqs), &(pCpuStatsStruct->Steal),
			&(pCpuStatsStruct->Guest));
	if (nItemsFound == 9) {
		nStatus = 0;
	}
	// TRACEF(7,("    InterpretCpuLine() ItemsFound=%d  %ld %ld %ld %ld %ld\n", nItemsFound, nUserMode, nUserModeNiced, nSystem, nIdel, nIOWait));
	return (nStatus);
} // end interpretcpuline.


/**
 * @BUG: TODO V intr and ctxt are reported for cpu0 but the data is actually for all the CPUs combined.
 */
int GetProcStat(struct CpuInformationStructure * pCpuInformationStruct,
		long nClockTicksPerSecond) {
	int nStatus = 0;

	char szLine[MAX_STATISTICS_LINE] = "";
	char szCPUString[PROC_STAT_ITEM_STRING_LENGTH] = "";

	TRACEF(5,(">>> GetProcStat()\n"));

	nStatus = OpenFile("/proc/stat");

	int nCpuIndex = 0;
	// This whole construction, operates on the assumption that cpus are listed in ascending order in the file.
	while ((nStatus == 0) && (nCpuIndex
			< pCpuInformationStruct->nNumberOfCpusSupported)) {
		//  Read the cpuN line
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);
		if (nStatus == 0) {
			snprintf(szCPUString, PROC_STAT_ITEM_STRING_LENGTH, "cpu%d",
					nCpuIndex);
			if (strncmp(szCPUString, szLine, strlen(szCPUString)) == 0) {
				TRACEF(7,("%s: '%s'\n", szCPUString, szLine));
				InterpretCpuLine(
						pCpuInformationStruct->arCpuStatisticsPointerArray[nCpuIndex],
						szLine + strlen(szCPUString), nClockTicksPerSecond);
				nCpuIndex++;
			}
		} // endif status
	} // end while.

	int nItemsFound = 0;
	// Read the rest of the data.
	// is a bit pattern 1=ctxt, 2=intr etc.
#define CTXT_BIT_PATTERN 1
#define INTR_BIT_PATTERN 2
#define PROCS_BIT_PATTERN 4
#define RUN_QUEUE_BIT_PATTERN 8
	int nMissingData = CTXT_BIT_PATTERN + INTR_BIT_PATTERN + PROCS_BIT_PATTERN + RUN_QUEUE_BIT_PATTERN;
	while ( (nStatus == 0) && (nMissingData != 0) ){
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);
		if (strncmp("ctxt", szLine, 4) == 0) {
			// TODO N Probably should do a bit pattern and 0.
			nMissingData -= CTXT_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 4, "%ld",
					&(pCpuInformationStruct->arCpuStatisticsPointerArray[0]->ContextSwitches));
		}
		if (strncmp("intr", szLine, 4) == 0) {
			nMissingData -= INTR_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 4, "%ld",
					&(pCpuInformationStruct->arCpuStatisticsPointerArray[0]->Interrupts));
		}
		if (strncmp("processes", szLine, 9) == 0) {
			nMissingData -= PROCS_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 9, "%ld",
					&(pCpuInformationStruct->arCpuStatisticsPointerArray[0]->SysExec));
		}
		if (strncmp("procs_running", szLine, 13) == 0) {
			nMissingData -= RUN_QUEUE_BIT_PATTERN;
			nItemsFound = sscanf(szLine + 13, "%ld",
					&(pCpuInformationStruct->arCpuStatisticsPointerArray[0]->RunQueue));
		}
	} // end while

	CloseFile();

	return (nStatus);
} // end getprocstat.
