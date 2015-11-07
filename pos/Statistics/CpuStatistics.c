/*
 * CpuStatistics.c
 *
 *  Created on: Jun 27, 2011
 */

#include "CpuStatistics.h"

#include <stdlib.h>

#include "../Common/FunctionTracing.h"

/**
 * @parm pCpuInfoStruct, this must be null, prior to call.
 * @return Point to CpuInformationStructure if success, otherwise NULL.
 */
struct CpuInformationStructure *CreateCpuInformationStruct() {
	struct CpuInformationStructure *pCpuInfoStruct = malloc(
			sizeof(struct CpuInformationStructure));
	TRACEF(7,("    CreateCpuStatistics() sizeof(struct CpuInformationStructure) = %Zi\n", sizeof(struct CpuInformationStructure)));
	if (pCpuInfoStruct != NULL) {
		// Note this allocates the Pointer array.
		pCpuInfoStruct->pCpuStatisticsStorage = malloc(
				sizeof(struct CpuStatistics) * MAX_NUMBER_OF_SUPPORTED_CPUS);
		if (pCpuInfoStruct->pCpuStatisticsStorage == NULL) {
			free(pCpuInfoStruct);
			pCpuInfoStruct = NULL;
		} else {
			int nCount = 0;
			for (nCount = 0; nCount < MAX_NUMBER_OF_SUPPORTED_CPUS; nCount++) {
				pCpuInfoStruct->arCpuStatisticsPointerArray[nCount]
						= pCpuInfoStruct->pCpuStatisticsStorage + nCount
								* sizeof(struct CpuStatistics);
				// TRACEF(7,("    CreateCpuStatistics() Init arCpuStatisticsPointerArray[%d] = %p\n", nCount, pCpuInfoStruct->pCpuStatisticsStorage+ nCount* sizeof(struct CpuStatistics)));
			} // next ncount.
			pCpuInfoStruct->nNumberOfCpusSupported = 0; // init it so DumpCpuInformationStructure doesn't dump garbage.
		}
	} // end pCpuInfoStruct not null.
	return (pCpuInfoStruct);
} // end initcpustatistics.


int DestroyCpuInfromationStruct(struct CpuInformationStructure *pCpuInfoStruct) {
	int nStatus = -1;

	if (pCpuInfoStruct != NULL) {
		/*
		 if (pCpuInfoStruct->arCpuStatisticsPointerArray !=NULL) {
		 free(pCpuInfoStruct->arCpuStatisticsPointerArray);
		 }
		 */
		if (pCpuInfoStruct->pCpuStatisticsStorage != NULL) {
			free(pCpuInfoStruct->pCpuStatisticsStorage);
			nStatus = 0;
		}
		free(pCpuInfoStruct);
	}
	return (nStatus);
} // end destroycpustatistics.


void DumpCpuInformationStructure(struct CpuInformationStructure *pStruct) {
	printf("pStruct.....: %p\n", (void *) pStruct);
	printf("Current CPUs: %d\n", pStruct->nNumberOfCpusSupported);
	printf("Max CPUs..: %d\n", MAX_NUMBER_OF_SUPPORTED_CPUS);

	int nCount = 0;
	for (nCount = 0; nCount < pStruct->nNumberOfCpusSupported; nCount++) {
		printf("CpuStat struct: %p\n",
				pStruct->arCpuStatisticsPointerArray[nCount]);
		DumpCpuStatistics(pStruct->arCpuStatisticsPointerArray[nCount]);
	} // next ncount.
}

void DumpCpuStatistics(struct CpuStatistics *pStruct) {
	printf("   User............: %lu\n", pStruct->User);
	printf("   UseNice.........: %lu\n", pStruct->UseNice);
	printf("   System..........: %lu\n", pStruct->System);
	printf("   IoWait..........: %lu\n", pStruct->IoWait);
	printf("   Idle............: %lu\n", pStruct->Idle);
	printf("   ContextSwitches.: %lu\n", pStruct->ContextSwitches);
	printf("   InvolCntxtSwtchs: %lu\n", pStruct->InvolentaryContextSwitches);
	printf("   CrossCalls......: %lu\n", pStruct->CrossCalls);
	printf("   Migration.......: %lu\n", pStruct->Migration);
	printf("   Interrupts......: %lu\n", pStruct->Interrupts);
	printf("   SMutex..........: %lu\n", pStruct->SMutex);
	printf("   SysExec.........: %lu\n", pStruct->SysExec);
	printf("   RunQueue........: %lu\n", pStruct->RunQueue);
} // end dumpcpustatistics

int InitCpuStatStruct(struct CpuStatistics *pStruct) {
	int nStatus = 0;
	pStruct->User = 0;
	pStruct->UseNice = 0;
	pStruct->System = 0;
	pStruct->IoWait = 0;
	pStruct->Idle = 0;
	pStruct->ContextSwitches = 0;
	pStruct->InvolentaryContextSwitches = 0;
	pStruct->CrossCalls = 0;
	pStruct->Migration = 0;
	pStruct->Interrupts = 0;
	pStruct->Irqs = 0;
	pStruct->SoftIrqs = 0;
	pStruct->Steal = 0;
	pStruct->Guest = 0;
	pStruct->SMutex = 0;
	pStruct->SysExec = 0;
	pStruct->RunQueue = 0;

	return (nStatus);
}

/**
 * @return 0 - ok
 *         -98 - nNumberOfCpus outside range [1;MAX_NUMBER_OF_SUPPORTED_CPUS].
 */
int InitCpuInformationStruct(struct CpuInformationStructure *pStruct,
		int nNumberOfCpus) {
	int nStatus = 0;

	if ((nNumberOfCpus > 0) && (nNumberOfCpus <= MAX_NUMBER_OF_SUPPORTED_CPUS)) {
		int nCount = 0;
		nStatus = 0;

		pStruct->nNumberOfCpusSupported = nNumberOfCpus;

		while ((nStatus == 0) && (nCount < nNumberOfCpus)) {
			// Init the CpuStatistics structure.
			nStatus = InitCpuStatStruct(
					pStruct->arCpuStatisticsPointerArray[nCount]);
			nCount++;
		} // end while.
	} else { // endif #cpus <= max.
		nStatus = -98; // arbitrary value to make it likely the InitCpuStatistics() doesn't return a similar number.
	}
	return (nStatus);
}
