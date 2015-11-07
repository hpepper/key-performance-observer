/*
 * CpuStatistics.h
 *
 *  Created on: Jun 27, 2011
 */

#ifndef CPUSTATISTICS_H_
#define CPUSTATISTICS_H_

// The Current hw model supports up to 8 logical cpus, but a two socket 6 core would support 2x12=24.
#define MAX_NUMBER_OF_SUPPORTED_CPUS 24

// Documented in 'man proc' under /proc/stat
struct CpuStatistics {
	unsigned long User;
	unsigned long UseNice;
	unsigned long System;
	unsigned long IoWait;
	unsigned long Idle;
	unsigned long ContextSwitches;
	unsigned long InvolentaryContextSwitches;
	unsigned long CrossCalls; // calling functions on a different CPU.
	unsigned long Migration;  // Migrating a process to a different process.
	unsigned long Interrupts; // Number of interrupts.
	unsigned long Irqs;       // Time spent servicing interrupts.
	unsigned long SoftIrqs;   // Time spent servicing soft irqs.
	unsigned long Steal;      // stolen time, which is the time spent in other operating systems when running in a virtualized environment.
	unsigned long Guest;      // time spent running a virtual CPU for guest operating systems under the control of the Linux kernel.
	unsigned long SMutex;     // TODO V find out what this is actually suppose to be called.
	unsigned long SysExec;    // TODO V find out if this is the number of created process'.
	unsigned long RunQueue;   // Number of taks running and ready to run.
};


struct CpuInformationStructure {
	// it is assumed that the first cpu is identified as 'cpu0'.
	struct CpuStatistics *arCpuStatisticsPointerArray[MAX_NUMBER_OF_SUPPORTED_CPUS];
	void *pCpuStatisticsStorage;
	int nNumberOfCpusSupported; // Number of CPUs actually supported must be less than MAX_NUMBER_OF_SUPPORTED_CPUS.

};


struct CpuInformationStructure *CreateCpuInformationStruct();
int DestroyCpuInfromationStruct(struct CpuInformationStructure *pCpuInfoStruct);
void DumpCpuInformationStructure(struct CpuInformationStructure *pStruct);
void DumpCpuStatistics(struct CpuStatistics *pStruct);
int InitCpuStatStruct(struct CpuStatistics *pStruct);
int InitCpuInformationStruct(struct CpuInformationStructure *pStruct, int nNumberOfCpus);

#endif /* CPUSTATISTICS_H_ */
