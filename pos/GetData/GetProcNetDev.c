/*
 * GetProcNetDev.c
 *
 *  Created on: Jul 3, 2011
 */

#include "GetProcNetDev.h"

#include "../Common/FunctionTracing.h"
#include "../FileOps/FileRecordHandler.h"

#include <string.h>

/**
 *  |bytes    packets errs drop fifo colls carrier compressed
 *
 * Col0: r-bytes
 * Col1: r-packets
 * Col2: r-errs
 * Col3: r-drop
 * Col4: r-fifo
 * Col5: r-frame
 * Col6: r-compressed
 * Col7: r-multicast
 * Col8: w-bytes
 * Col9: w-packets
 * Col10: w-errs
 * Col11: w-drop
 * Col12: w-fifo
 * Col13: w-colls
 * Col14: w-carrier
 * Col15: w-compressed
 */
int InterpretNetLine(struct NetStatistics *pNetStatsStruct, char *szLine) {
	int nStatus = -1;
	unsigned long nrFifo;
	unsigned long nrFrame;
	unsigned long nrCompressed;
	unsigned long nrMultiCast;

	int nItemsFound = sscanf(szLine,
			"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
			&(pNetStatsStruct->iOctets), &(pNetStatsStruct->iPackets),
			&(pNetStatsStruct->iErrors), &(pNetStatsStruct->iDrop), &nrFifo,
			&nrFrame, &nrCompressed, &nrMultiCast, &(pNetStatsStruct->oOctets),
			&(pNetStatsStruct->oPackets), &(pNetStatsStruct->oErrors),
			&(pNetStatsStruct->oDrop));
	if (nItemsFound == 12) {
		nStatus = 0;
	}
	// TRACEF(7,("    InterpretNetLine() ItemsFound=%d  %ld %ld %ld %ld %ld\n", nItemsFound, nUserMode, nUserModeNiced, nSystem, nIdel, nIOWait));
	return (nStatus);
} // end interpretNetline.


/**
 * TODO V Verify that an errors is detected if 2 nics are expected and only one found.
 */
int GetProcNetDev(struct NetInformationStructure * pNetInformationStruct) {
	int nStatus = 0;

	char szLine[MAX_STATISTICS_LINE] = "";
	char szNetString[PROC_STAT_ITEM_STRING_LENGTH] = "";

	TRACEF(5,(">>> GetProcNetDev()\n"));

	nStatus = OpenFile("/proc/net/dev");

	int nNetIndex = 0;
	// This whole construction, operates on the assumption that Nets are listed in ascending order in the file.
	while ((nStatus == 0) && (nNetIndex
			< pNetInformationStruct->nNumberOfNetsSupported)) {
		//  Read the NetN line
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);
		if (nStatus == 0) {
			// TODO V Find a more effecient way of doing this.
			if (pNetInformationStruct->cNicType == 'E') {
				// TODO C support nic named 'emX'.
				snprintf(szNetString, PROC_STAT_ITEM_STRING_LENGTH, "  eth%d:", nNetIndex);
			} else {
				// TODO N It defaults to 'B' but that might be wrong, handle it better, later.
				snprintf(szNetString, PROC_STAT_ITEM_STRING_LENGTH, "   br%d:",
						nNetIndex);
			}
			if (strncmp(szNetString, szLine, strlen(szNetString)) == 0) {
				TRACEF(7,("%s: '%s'\n", szNetString, szLine));
				InterpretNetLine(
						pNetInformationStruct->arNetStatisticsPointerArray[nNetIndex],
						szLine + strlen(szNetString));
				nNetIndex++;
			}
			TRACEF(7,("GetProcNetDev() nStatus=%d, nNetIndex=%d\n", nStatus, nNetIndex));
		} // endif status
	} // end while.
	CloseFile();

	return (nStatus);
} // end getprocstat.
