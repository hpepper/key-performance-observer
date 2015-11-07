/*
 * StoreCpuData.h
 *
 *  Created on: Jun 28, 2011
 */

#ifndef STORECPUDATA_H_
#define STORECPUDATA_H_

#include "../Statistics/CpuStatistics.h"
#include <stdio.h>

int StoreCpuData(FILE *LogFileHandle, struct CpuInformationStructure *pCpuInformationStructure);

#endif /* STORECPUDATA_H_ */
