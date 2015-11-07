/*
 * GetProcNetDev.h
 *
 *  Created on: Jul 3, 2011
 *      Author: cadm
 */

#ifndef GETPROCNETDEV_H_
#define GETPROCNETDEV_H_

#include "../Statistics/NetStatistics.h"

#include "GetDataCommon.h"

int InterpretNetLine(struct NetStatistics *pNetStatsStruct, char *szLine);
int GetProcNetDev(struct NetInformationStructure * pNetInformationStruct);

#endif /* GETPROCNETDEV_H_ */
