/*
 * StorageStatistics.c
 *
 *  Created on: Jul 3, 2011
 */



#include "StorageStatistics.h"

#include <stdlib.h>

#include "../Common/FunctionTracing.h"

/**
 * @parm pStorageInfoStruct, this must be null, prior to call.
 * @return Point to StorageInformationStructure if success, otherwise NULL.
 */
struct StorageInformationStructure *CreateStorageInformationStruct() {
	struct StorageInformationStructure *pStorageInfoStruct = malloc(
			sizeof(struct StorageInformationStructure));
	TRACEF(7,("    CreateStorageStatistics() sizeof(struct StorageInformationStructure) = %Zi\n", sizeof(struct StorageInformationStructure)));
	if (pStorageInfoStruct != NULL) {
		// Note this allocates the Pointer array.
		pStorageInfoStruct->pStorageStatisticsStorage = malloc(
				sizeof(struct StorageStatistics) * MAX_NUMBER_OF_SUPPORTED_DISKS);
		if (pStorageInfoStruct->pStorageStatisticsStorage == NULL) {
			free(pStorageInfoStruct);
			pStorageInfoStruct = NULL;
		} else {
			int nCount = 0;
			for (nCount = 0; nCount < MAX_NUMBER_OF_SUPPORTED_DISKS; nCount++) {
				pStorageInfoStruct->arStorageStatisticsPointerArray[nCount]
						= pStorageInfoStruct->pStorageStatisticsStorage + nCount
								* sizeof(struct StorageStatistics);
				// TRACEF(7,("    CreateStorageStatistics() Init arStorageStatisticsPointerArray[%d] = %p\n", nCount, pStorageInfoStruct->pStorageStatisticsStorage+ nCount* sizeof(struct StorageStatistics)));
			} // next ncount.
			pStorageInfoStruct->nNumberOfDisksSupported = 0; // init it so DumpStorageInformationStructure doesn't dump garbage.
		}
	} // end pStorageInfoStruct not null.
	return (pStorageInfoStruct);
} // end initStoragestatistics.


int DestroyStorageInfromationStruct(struct StorageInformationStructure *pStorageInfoStruct) {
	int nStatus = -1;

	if (pStorageInfoStruct != NULL) {
		if (pStorageInfoStruct->pStorageStatisticsStorage != NULL) {
			free(pStorageInfoStruct->pStorageStatisticsStorage);
			nStatus = 0;
		}
		free(pStorageInfoStruct);
	}
	return (nStatus);
} // end destroyStoragestatistics.


void DumpStorageInformationStructure(struct StorageInformationStructure *pStruct) {
	printf("pStruct.....: %p\n", (void *) pStruct);
	printf("Current Storages: %d\n", pStruct->nNumberOfDisksSupported);
	printf("Max Storages..: %d\n", MAX_NUMBER_OF_SUPPORTED_DISKS);

	int nCount = 0;
	for (nCount = 0; nCount < pStruct->nNumberOfDisksSupported; nCount++) {
		printf("StorageStat struct: %p\n",
				pStruct->arStorageStatisticsPointerArray[nCount]);
		DumpStorageStatStruct(pStruct->arStorageStatisticsPointerArray[nCount]);
	} // next ncount.
}

void DumpStorageStatStruct(struct StorageStatistics *pStruct) {
	printf("   Sectores read....: %lu\n", pStruct->nrd);
	printf("   Sectors written..: %lu\n", pStruct->nwr);
	printf("        ............: %lu\n", pStruct->rcnt);
	printf("   Read operations..: %lu\n", pStruct->rd);
	printf("   Time in read.....: %lu\n", pStruct->rtime);
	printf("        ............: %lu\n", pStruct->wcnt);
	printf("   Write operations.: %lu\n", pStruct->wr);
	printf("   Time in write ops: %lu\n", pStruct->wtime);
	printf("   IOs in progress..: %lu\n", pStruct->ioinprogress);
	printf("   ms doing i/o ....: %lu\n", pStruct->iotime);
	printf("   Weighted i/o time: %lu\n", pStruct->weightediotime);
} // end dumpStoragestatistics


int InitStorageStatStruct(struct StorageStatistics *pStruct) {
	int nStatus=0;
	pStruct->nrd = 0;
	pStruct->rcnt = 0;
	pStruct->rd = 0;
	pStruct->rtime = 0;
	pStruct->nwr = 0;
	pStruct->wcnt = 0;
	pStruct->wr = 0;
	pStruct->wtime = 0;
	pStruct->ioinprogress = 0;
	pStruct->iotime = 0;
	pStruct->weightediotime = 0;
	return(nStatus);
}

/**
 * @return 0 - ok
 *         -98 - nNumberOfCpus outside range [1;MAX_NUMBER_OF_SUPPORTED_CPUS].
 */
int InitStorageInformationStructure(struct StorageInformationStructure *pStruct,
		int nNumberOfDisks, char cDiskType) {
	int nStatus = 0;

	if ((nNumberOfDisks > 0) && (nNumberOfDisks <= MAX_NUMBER_OF_SUPPORTED_DISKS)) {
		int nCount = 0;
		nStatus = 0;

		pStruct->nNumberOfDisksSupported = nNumberOfDisks;
		pStruct->cDiskType = cDiskType;
		while ((nStatus == 0) && (nCount < nNumberOfDisks)) {
			// Init the DisksStatistics structure.
			nStatus = InitStorageStatStruct(
					pStruct->arStorageStatisticsPointerArray[nCount]);
			nCount++;
		} // end while.
		nStatus = -98; // arbitrary value to make it likely the InitCpuStatistics() doesn't return a similar number.
	}
	return (nStatus);
}
