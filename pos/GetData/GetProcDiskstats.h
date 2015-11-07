/*
 * GetProcDiskstats.h
 *
 *  Created on: Jul 1, 2011
 *
 *  Starts on the 14.th coloumn.
 *  Host: sda
 *  Guest: vda
 *
 *  TODO V I need to test this on a machine with 4 Disks in RAID.
 */

#ifndef GETPROCDISKSTATS_H_
#define GETPROCDISKSTATS_H_

#include "../Statistics/StorageStatistics.h"

#include "GetDataCommon.h"

int InterpretStorageLine(struct StorageStatistics *pStorageStatsStruct, char *szLine);
int GetProcDiskstats(struct StorageInformationStructure * pStorageInformationStruct);


#endif /* GETPROCDISKSTATS_H_ */
