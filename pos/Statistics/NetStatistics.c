/*
 * NetStatistics.c
 *
 *  Created on: Jul 3, 2011
 */


#include "NetStatistics.h"

#include <stdlib.h>

#include "../Common/FunctionTracing.h"

struct NetInformationStructure *CreateNetInformationStruct() {
	struct NetInformationStructure *pNetInfoStruct = malloc(
			sizeof(struct NetInformationStructure));
	//TRACEF(7,("    CreateNetStatistics() sizeof(struct NetInformationStructure) = %Zi\n", sizeof(struct NetInformationStructure)));
	if (pNetInfoStruct != NULL) {
		// Note this allocates the Pointer array.
		pNetInfoStruct->pNetStatisticsStorage = malloc(
				sizeof(struct NetStatistics) * MAX_NUMBER_OF_SUPPORTED_NICS);
		if (pNetInfoStruct->pNetStatisticsStorage == NULL) {
			free(pNetInfoStruct);
			pNetInfoStruct = NULL;
		} else {
			int nCount = 0;
			int nFailedStructureAllocationAt = -1;
			while ( ( nFailedStructureAllocationAt == -1 ) && (nCount < MAX_NUMBER_OF_SUPPORTED_NICS) ) {
				pNetInfoStruct->arNetStatisticsPointerArray[nCount]
						= pNetInfoStruct->pNetStatisticsStorage + nCount
								* sizeof(struct NetStatistics);
				// Fail if the pointer is NULL. Capture the nCount, so all previous successfull allocations can be released.
				if ( pNetInfoStruct->arNetStatisticsPointerArray[nCount] == NULL ) {
					nFailedStructureAllocationAt = nCount; // remove the previous [0;nCount[.
				}
				nCount++;
			} // end while-
			pNetInfoStruct->nNumberOfNetsSupported = 0; // init it so DumpNetInformationStructure doesn't dump garbage.
			if ( nFailedStructureAllocationAt != -1) {
				// So not all the structures for the array could be allocated, so now we are cleaning up.
				for (nCount=0; nCount < nFailedStructureAllocationAt; nCount++) {
					free(pNetInfoStruct->arNetStatisticsPointerArray[nCount]);
				} // next ncount.
				free(pNetInfoStruct->pNetStatisticsStorage);
				free(pNetInfoStruct);
				pNetInfoStruct=NULL;
			}
		}
	} // end pNetInfoStruct not null.
	return (pNetInfoStruct);
} // end NetInformationStructure.

int DestroyNetInfromationStruct(struct NetInformationStructure *pNetInfoStruct) {
	int nStatus = -1;

	if (pNetInfoStruct != NULL) {
		if (pNetInfoStruct->pNetStatisticsStorage != NULL) {
			free(pNetInfoStruct->pNetStatisticsStorage);
			nStatus = 0;
		}
		free(pNetInfoStruct);
	}
	return (nStatus);
} // end destroyNetstatistics.


void DumpNetInformationStructure(struct NetInformationStructure *pStruct) {
	printf("pStruct.....: %p\n", (void *) pStruct);
	printf("Current NICs: %d\n", pStruct->nNumberOfNetsSupported);
	printf("Max NICs..: %d\n", MAX_NUMBER_OF_SUPPORTED_NICS);
}

void DumpNetStatStruct(struct NetStatistics *pStruct) {
	printf("   Col............: %lu\n", pStruct->Colissions);
	printf("   Defered........: %lu\n", pStruct->Defered);
	printf("   NoCanPut.......: %lu\n", pStruct->NoCanPut);
	printf("   iDrop..........: %lu\n", pStruct->iDrop);
	printf("   iErrors........: %lu\n", pStruct->iErrors);
	printf("   iOctets........: %lu\n", pStruct->iOctets);
	printf("   iPacketes......: %lu\n", pStruct->iPackets);
	printf("   oDrop..........: %lu\n", pStruct->oDrop);
	printf("   oErrors........: %lu\n", pStruct->oErrors);
	printf("   oOctets........: %lu\n", pStruct->oOctets);
	printf("   oPackets.......: %lu\n", pStruct->oPackets);
}

int InitNetStatStruct(struct NetStatistics *pStruct) {
	int nStatus=0;
	pStruct->Colissions=0;
	pStruct->Defered=0;
	pStruct->NoCanPut=0;
	pStruct->iDrop=0;
	pStruct->iErrors=0;
	pStruct->iOctets=0;
	pStruct->iPackets=0;
	pStruct->oDrop=0;
	pStruct->oErrors=0;
	pStruct->oOctets=0;
	pStruct->oPackets=0;
	return(nStatus);
}

/**
 * @return 0 - ok
 *         -98 - nNumberOfNics outside range [1;MAX_NUMBER_OF_SUPPORTED_NETS].
 */
int InitNetInformationStruct(struct NetInformationStructure *pStruct,
		int nNumberOfNets, char cNicType) {
	int nStatus = 0;

	if ((nNumberOfNets > 0) && (nNumberOfNets <= MAX_NUMBER_OF_SUPPORTED_NICS)) {
		int nCount = 0;
		nStatus = 0;

		pStruct->nNumberOfNetsSupported = nNumberOfNets;
		pStruct->cNicType = cNicType;

		while ((nStatus == 0) && (nCount < nNumberOfNets)) {
			// Init the NetStatistics structure.
			nStatus = InitNetStatStruct(
					pStruct->arNetStatisticsPointerArray[nCount]);
			nCount++;
		} // end while.
	} else { // endif #Nets <= max.
		nStatus = -98; // arbitrary value to make it likely the InitNetStatistics() doesn't return a similar number.
	}
	return (nStatus);
}
