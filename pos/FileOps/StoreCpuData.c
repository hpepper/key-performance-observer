/*
 * StoreCpuData.c
 *
 *  Created on: Jun 28, 2011
 */


#include "StoreCpuData.h"
#include "../Common/FunctionTracing.h"

/**
 * FileHandle should be a stream, then I could use fprintf.
 * TODO N These store operations source files could be autogenerated by the createrrd.pl tool.
 */
int StoreCpuData(FILE *LogFileHandle,
		struct CpuInformationStructure *pCpuInformationStructure) {
	int nStatus = 0;

	TRACEF(7,("        StoreCpuData(%p,%p)\n", LogFileHandle, pCpuInformationStructure));

	int nCpuIndex = 0;
	for (nCpuIndex = 0; nCpuIndex
			< pCpuInformationStructure->nNumberOfCpusSupported; nCpuIndex++) {
		struct CpuStatistics *pStruct = pCpuInformationStructure->arCpuStatisticsPointerArray[nCpuIndex];
		TRACEF(7,("        CpuStatistics[%d] %p\n", nCpuIndex, pStruct));
		fprintf(LogFileHandle, "  <cpu Name=\"cpu_stat%d\">", nCpuIndex);
		fprintf(LogFileHandle, " <idle>%lu</idle>", pStruct->Idle);
		fprintf(LogFileHandle, " <user>%lu</user>", pStruct->User);
		fprintf(LogFileHandle, " <sys>%lu</sys>", pStruct->System);
        //fprintf(LogFileHandle, " <xcal>%lu</xcal>", pStruct->CrossCalls);
		//fprintf(LogFileHandle, " <migr>%lu</migr>", pStruct->Migration);
		//fprintf(LogFileHandle, " <smtx>%lu</smtx>", pStruct->SMutex);
		//fprintf(LogFileHandle, " <uNice>%lu</uNice>", pStruct->UseNice);
		fprintf(LogFileHandle, " <iowait>%lu</iowait>", pStruct->IoWait);
		fprintf(LogFileHandle, " <irqsvc>%lu</irqsvc>", pStruct->Irqs);
		fprintf(LogFileHandle, " <sftisvc>%lu</sftisvc>", pStruct->SoftIrqs);
		//fprintf(LogFileHandle, " <steal>%lu</steal>", pStruct->Steal);
		fprintf(LogFileHandle, " <guest>%lu</guest>", pStruct->Guest);
		fprintf(LogFileHandle, "  </cpu>\n");
	} // next nCpuIndex.

    // These statistics are global and not per CPU, in linux.
	fprintf(LogFileHandle, "  <cpu Name=\"\">");
	fprintf(LogFileHandle, " <csw>%lu</csw>", pCpuInformationStructure->arCpuStatisticsPointerArray[0]->ContextSwitches);
	fprintf(LogFileHandle, " <icsw>%lu</icsw>", pCpuInformationStructure->arCpuStatisticsPointerArray[0]->InvolentaryContextSwitches);
	fprintf(LogFileHandle, " <sysexec>%lu</sysexec>", pCpuInformationStructure->arCpuStatisticsPointerArray[0]->SysExec);
	fprintf(LogFileHandle, " <runque>%lu</runque>", pCpuInformationStructure->arCpuStatisticsPointerArray[0]->RunQueue);
	fprintf(LogFileHandle, " <intr>%lu</intr>", pCpuInformationStructure->arCpuStatisticsPointerArray[0]->Interrupts);
	fprintf(LogFileHandle, "  </cpu>\n");

	return (nStatus);
} // end StoreCpuData.
