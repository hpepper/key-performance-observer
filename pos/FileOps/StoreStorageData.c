/*
 * StoreStorageData.c
 *
 *  Created on: Jul 5, 2011
 */


#include "StoreStorageData.h"
#include "../Common/FunctionTracing.h"

int StoreStorageData(FILE *LogFileHandle,
		struct StorageInformationStructure *pInfoStruct) {
	int nStatus = 0;

	TRACEF(7,("        StoreStorageData(%p,%p)\n", LogFileHandle, pInfoStruct));

	int nStorageIndex = 0;
	for (nStorageIndex = 0; nStorageIndex
			< pInfoStruct->nNumberOfDisksSupported; nStorageIndex++) {
		struct StorageStatistics *pStruct = pInfoStruct->arStorageStatisticsPointerArray[nStorageIndex];
		TRACEF(7,("        StorageStatistics[%d] %p\n", nStorageIndex, pStruct));
		fprintf(LogFileHandle, "  <cpu Name=\"dsk%d\">", nStorageIndex);
		fprintf(LogFileHandle, " <nrd>%lu</nrd>", pStruct->nrd);
		fprintf(LogFileHandle, " <nwr>%lu</nwr>", pStruct->nwr);
		fprintf(LogFileHandle, " <rd>%lu</rd>", pStruct->rd);
		fprintf(LogFileHandle, " <wr>%lu</wr>", pStruct->wr);
		//fprintf(LogFileHandle, " <wcnt>%lu</wcnt>", pStruct->wcnt);
		//fprintf(LogFileHandle, " <rcnt>%lu</rcnt>", pStruct->rcnt);
		fprintf(LogFileHandle, " <wtime>%lu</wtime>", pStruct->wtime);
		fprintf(LogFileHandle, " <rtime>%lu</rtime>", pStruct->rtime);
		fprintf(LogFileHandle, " <ioinprogress>%lu</ioinprogress>", pStruct->ioinprogress);
		fprintf(LogFileHandle, " <iotime>%lu</iotime>", pStruct->iotime);
		fprintf(LogFileHandle, " <weightediotime>%lu</weightediotime>", pStruct->weightediotime);
		fprintf(LogFileHandle, "  </cpu>\n");
	} // next nStorageIndex.

	return (nStatus);
} // end StoreStorageData.
