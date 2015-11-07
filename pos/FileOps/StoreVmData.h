/*
 * StoreVmData.h
 *
 *  Created on: Jul 5, 2011
 */

#ifndef STOREVMDATA_H_
#define STOREVMDATA_H_

#include "../Statistics/VmStatistics.h"
#include <stdio.h>

int StoreVmData(FILE *LogFileHandle, struct VmStatistics *pStruct);


#endif /* STOREVMDATA_H_ */
