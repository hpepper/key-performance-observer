/*
 * StorageStatistics.h
 *
 *  Created on: Jul 1, 2011
 *
 *  Source: diskstats
 *  Documentation: Kernel Documentations/iostats
 *
 * field 3(sectors read)     - nrd   - sectors read
 * field 7(sectors written)  - nwr   - sectors written
 * field 1(reads completed)  - rd    - read operations
 * field 5(writes completed) - wr    - write operations
 *                           - wcnt  - Elements in wait state.
 *                           - rcnt  - Elements in run state.
 * field 8(ms spent writing) - wtime - cumulative wait, pre-service, time
 * field 4(ms spent reading) - rtime - cumulative wait, service, time
 *
 * field 9()                 - ioinprogress - of I/Os currently in progress
 * field 10(ms doing io)     - iotime -  # of milliseconds spent doing I/O.
 *                                        This field is increases so long as field 9 is nonzero.
 * field 11()                - weightediotime - weighted # of milliseconds spent doing I/Os.
 *
 *
 *  It would actually be hard to define the list of storage devices, since it could be sda, sdb, hda, hdb...
 *  But for this, sda. sdb, sdc, sdd is assumed.
 *
 */

#ifndef STORAGESTATISTICS_H_
#define STORAGESTATISTICS_H_

// This is the max for the current server hw.
#define MAX_NUMBER_OF_SUPPORTED_DISKS 8

struct StorageStatistics {
	unsigned long nrd;
	unsigned long nwr;
	unsigned long rd;
	unsigned long wr;
	unsigned long rcnt;
	unsigned long wcnt;
	unsigned long rtime;
	unsigned long wtime;
	unsigned long ioinprogress;
	unsigned long iotime;
	unsigned long weightediotime;
};

struct StorageInformationStructure {
	struct StorageStatistics *arStorageStatisticsPointerArray[MAX_NUMBER_OF_SUPPORTED_DISKS];
	void *pStorageStatisticsStorage;
	int nNumberOfDisksSupported; // Number of Disks actually supported must be less than MAX_NUMBER_OF_SUPPORTED_DISKS.
	char cDiskType; // 'h' for hdX, 's' for sdX and 'v' for vdX.
};

struct StorageInformationStructure *CreateStorageInformationStruct();
int DestroyStorageInfromationStruct(struct StorageInformationStructure *pStorageInfoStruct);
void DumpStorageInformationStructure(struct StorageInformationStructure *pStruct);
void DumpStorageStatStruct(struct StorageStatistics *pStruct);
int InitStorageStatStruct(struct StorageStatistics *pStruct);
int InitStorageInformationStructure(struct StorageInformationStructure *pStruct, int nNumberOfDisks, char cDiskType);

#endif /* STORAGESTATISTICS_H_ */
