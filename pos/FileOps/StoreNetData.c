/*
 * StoreNetData.c
 *
 *  Created on: Jul 3, 2011
 *      Author: cadm
 */

#include "StoreNetData.h"
#include "../Common/FunctionTracing.h"

int StoreNetData(FILE *LogFileHandle,
		struct NetInformationStructure *pNetInformationStructure) {
	int nStatus = 0;

	TRACEF(7,("        StoreNetData(%p,%p)\n", LogFileHandle, pNetInformationStructure));

	int nNetIndex = 0;
	for (nNetIndex = 0; nNetIndex
			< pNetInformationStructure->nNumberOfNetsSupported; nNetIndex++) {
		struct NetStatistics *pStruct = pNetInformationStructure->arNetStatisticsPointerArray[nNetIndex];
		TRACEF(7,("        NetStatistics[%d] %p\n", nNetIndex, pStruct));
		fprintf(LogFileHandle, "  <cpu Name=\"nic%d\">", nNetIndex);
		fprintf(LogFileHandle, " <ipackets>%lu</ipackets>", pStruct->iPackets);
		fprintf(LogFileHandle, " <ierrors>%lu</ierrors>", pStruct->iErrors);
		fprintf(LogFileHandle, " <ioctets>%lu</ioctets>", pStruct->iOctets);
		fprintf(LogFileHandle, " <opackets>%lu</opackets>", pStruct->oPackets);
		fprintf(LogFileHandle, " <oerrors>%lu</oerrors>", pStruct->oErrors);
		fprintf(LogFileHandle, " <ooctets>%lu</ooctets>", pStruct->oOctets);
		//fprintf(LogFileHandle, " <col>%lu</col>", pStruct->Colissions);
		//fprintf(LogFileHandle, " <dfr>%lu</dfr>", pStruct->Defered);
		//fprintf(LogFileHandle, " <nocnpt>%lu</nocnpt>", pStruct->NoCanPut);
		fprintf(LogFileHandle, "  </cpu>\n");
	} // next nNetIndex.

	return (nStatus);
} // end StoreNetData.
