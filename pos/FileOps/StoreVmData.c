/*
 * StoreVmData.c
 *
 *  Created on: Jul 5, 2011
 */

#include "StoreVmData.h"
#include "../Common/FunctionTracing.h"

/**
 * FileHandle should be a stream, then I could use fprintf.
 */
int StoreVmData(FILE *LogFileHandle, struct  VmStatistics *pStruct) {
	int nStatus = 0;

	TRACEF(7,("      StoreVmData(%p,%p)\n", LogFileHandle, pStruct));


	fprintf(LogFileHandle, "  <cpu Name=\"\">");
	fprintf(LogFileHandle, " <scan>%lu</scan>", pStruct->PageScanned);
	fprintf(LogFileHandle, " <rev>%d</rev>", 0);
	fprintf(LogFileHandle, " <maj_fault>%lu</maj_fault>", pStruct->MajorPageFault);
	fprintf(LogFileHandle, " <freemem>%lu</freemem>", pStruct->FreeMem);
	fprintf(LogFileHandle, " <swap_free>%lu</swap_free>", pStruct->FreeSwap);
	fprintf(LogFileHandle, " <buffers>%lu</buffers>", pStruct->MemBuffers);
	fprintf(LogFileHandle, " <cached>%lu</cached>", pStruct->MemCached);
	fprintf(LogFileHandle, " <swapcached>%lu</swapcached>", pStruct->SwapCached);
	fprintf(LogFileHandle, " <swapin>%lu</swapin>", pStruct->PageSwapIn);
	fprintf(LogFileHandle, " <swapout>%lu</swapout>", pStruct->PageSwapOut);
	//fprintf(LogFileHandle, " <>%lu</>", pStruct-);
	fprintf(LogFileHandle, "  </cpu>\n");

	return (nStatus);
} // end StoreCpuData.
