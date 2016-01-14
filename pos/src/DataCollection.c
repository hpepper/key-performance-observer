/*
 * DataCollection.c
 *
 *  Created on: Jun 28, 2011
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "DataCollection.h"
#include "../GetData/GetProcDiskstats.h"
#include "../GetData/GetProcMeminfo.h"
#include "../GetData/GetProcNetDev.h"
#include "../GetData/GetProcStat.h"
#include "../GetData/GetProcVmstat.h"

#include "../Statistics/CpuStatistics.h"
#include "../Statistics/NetStatistics.h"
#include "../Statistics/StorageStatistics.h"
#include "../Statistics/VmStatistics.h"

#include "../FileOps/StoreCpuData.h"
#include "../FileOps/StoreNetData.h"
#include "../FileOps/StoreStorageData.h"
#include "../FileOps/StoreVmData.h"

// TODO V Put this in a structure if more data is needed.
long f_nClockTicksPerSecond = 0;
struct CpuInformationStructure *f_pCpuInformationStructure = NULL;
struct NetInformationStructure *f_pNetInformationStructure = NULL;
struct StorageInformationStructure *f_pStorageInformationStructure = NULL;
struct VmStatistics *f_pVmInformationStructure = NULL;

#define DATA_LOG_FILE_PATH_AND_NAME_MAX_LENGTH 255

char szDataLogFile[DATA_LOG_FILE_PATH_AND_NAME_MAX_LENGTH] = "";

FILE *f_LogFileHandle;
time_t f_nTimeOfNextFileCycle = 0;
time_t f_nFileCycleTimeInSeconds = 120;

int LoopDataCollection() {
	int nStatus = 0;

	time_t nNow = time(NULL);

	// Is it time for a file change.
	if (nNow > f_nTimeOfNextFileCycle) {
		// TODO Investigate this, it seems like I could be operating in an invalid f_LogFileHandle.
		fprintf(f_LogFileHandle, "</pos>\n");
		fclose(f_LogFileHandle);

		snprintf(szDataLogFile, DATA_LOG_FILE_PATH_AND_NAME_MAX_LENGTH,
				"/var/tmp/pos_%lu.xml", nNow);
		// TODO V Handle the return code here.
		f_LogFileHandle = fopen(szDataLogFile, "w");
		fprintf(f_LogFileHandle, "<pos>\n");

		// Calculate new next time
		f_nTimeOfNextFileCycle = nNow + f_nFileCycleTimeInSeconds;

	}
	DataCollection();

	return (nStatus);
}

int DataCollection() {
	int nStatus = 0;

	// Get a timestamp.
	fprintf(f_LogFileHandle, "<DataSet TimeStamp=\"%lu\">\n", time(NULL));

	// Get the context switch data etc.
	nStatus = GetProcStat(f_pCpuInformationStructure, f_nClockTicksPerSecond);

	nStatus = GetProcNetDev(f_pNetInformationStructure);

	nStatus = GetProcDiskstats(f_pStorageInformationStructure);

	nStatus = GetProcVmstat(f_pVmInformationStructure);
	nStatus = GetProcMeminfo(f_pVmInformationStructure);

	// Write the information to a file.
	nStatus = StoreCpuData(f_LogFileHandle, f_pCpuInformationStructure);
	nStatus = StoreNetData(f_LogFileHandle, f_pNetInformationStructure);
	nStatus = StoreStorageData(f_LogFileHandle, f_pStorageInformationStructure);
	nStatus = StoreVmData(f_LogFileHandle, f_pVmInformationStructure);

	// TODO V Possibly have all the data list and what they should be called, in a config file, that is read at boot time.

	fprintf(f_LogFileHandle, "</DataSet>\n");
	return (nStatus);
} // end datacollection.


int DestroyDataCollector() {
	int nStatus = 0;
	fprintf(f_LogFileHandle, "</pos>\n");
	fclose(f_LogFileHandle);

	DestroyCpuInfromationStruct(f_pCpuInformationStructure);
	DestroyNetInfromationStruct(f_pNetInformationStructure);
	DestroyStorageInfromationStruct(f_pStorageInformationStructure);
	DestroyVmStatStruct(f_pVmInformationStructure);

	return (nStatus);
}

int InitDataCollector(long nClockTicksPerSecond, long nNumberOfCpusConfigured,
		long nNumberOfNetsConfigured, char cNicType, long nNumberOfDisksConfigured, char cDiskType) {
	int nStatus = -1;

	f_nClockTicksPerSecond = nClockTicksPerSecond;

	f_pCpuInformationStructure = CreateCpuInformationStruct();
	if (f_pCpuInformationStructure != NULL) {
		InitCpuInformationStruct(f_pCpuInformationStructure,
				nNumberOfCpusConfigured);
		nStatus = 0;
	}

	f_pNetInformationStructure = CreateNetInformationStruct();
	if (f_pNetInformationStructure != NULL) {
		InitNetInformationStruct(f_pNetInformationStructure,
				nNumberOfNetsConfigured, cNicType);
		nStatus = 0;
	}

	f_pStorageInformationStructure = CreateStorageInformationStruct();
	if (f_pStorageInformationStructure != NULL) {
		InitStorageInformationStructure(f_pStorageInformationStructure,
				nNumberOfDisksConfigured, cDiskType);
		nStatus = 0;
	}

	f_pVmInformationStructure = CreateVmStatStruct();
	if (f_pVmInformationStructure != NULL) {
		InitVmStatStruct(f_pVmInformationStructure);
		nStatus = 0;
	}
	// TODO V Open the file stream for writing. At intervals close this file and open a new.
	// This is done just so that i can call close directly, in the loop.
	//   Not having to add logic that is only used ones, taking up resources.
	f_LogFileHandle = fopen("/tmp/pos_dummy_you_can_delete", "w");

	// At what time must the next file cycle occur.
	//   Setting it to 0 ensures that a new file will be opened.
	f_nTimeOfNextFileCycle = 0;

	return (nStatus);
}
