/*
 * GetProcStat.h
 *
 *  Created on: Jun 15, 2011
 *
 */

#ifndef GETPROCSTAT_H_
#define GETPROCSTAT_H_

#include "../Statistics/CpuStatistics.h"

#include "GetDataCommon.h"

int GetProcStat(struct CpuInformationStructure *pCpuInformationStruct, long nClockTicksPerSecond);

// Private, but exposed for unit testing.
int InterpretCpuLine(struct CpuStatistics *pCpuStatsStruct, char *szLine, long nClockTicksPerSecond);


#endif /* GETPROCSTAT_H_ */
