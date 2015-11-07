/*
 * VmStatistics.c
 *
 *  Created on: Jul 1, 2011
 */

#include "VmStatistics.h"

#include <stdlib.h>

#include "../Common/FunctionTracing.h"



struct VmStatistics *CreateVmStatStruct() {
	return( malloc(sizeof(struct VmStatistics)));
}


// TODO N Make this a common function, using a void pointer instead.
int DestroyVmStatStruct(struct VmStatistics *pStruct) {
	int nStatus = -1;
	if (pStruct != NULL) {
		free(pStruct);
		pStruct = NULL;
		nStatus=0;
	}
	return(nStatus);
}

void DumpVmStatStruct(struct VmStatistics *pStruct) {
	printf("   FreeMem.......: %lu\n", pStruct->FreeMem);
	printf("   FreeSwap......: %lu\n", pStruct->FreeSwap);
	printf("   MajorPageFault: %lu\n", pStruct->MajorPageFault);
	printf("   PageScanned...: %lu\n", pStruct->PageScanned);
	printf("   MemBuffers....: %lu\n", pStruct->MemBuffers);
	printf("   MemCached.....: %lu\n", pStruct->MemCached);
	printf("   PageSwapIn....: %lu\n", pStruct->PageSwapIn);
	printf("   PageSwapOut...: %lu\n", pStruct->PageSwapOut);
	printf("   SwapCached....: %lu\n", pStruct->SwapCached);
}

int InitVmStatStruct(struct VmStatistics *pStruct) {
	int nStatus = 0;
	pStruct->FreeMem=0;
	pStruct->FreeSwap=0;
	pStruct->MajorPageFault=0;
	pStruct->PageScanned=0;
	pStruct->MemBuffers=0;
	pStruct->MemCached=0;
	pStruct->PageSwapIn=0;
	pStruct->PageSwapOut=0;
	pStruct->SwapCached=0;
	return(nStatus);
}
