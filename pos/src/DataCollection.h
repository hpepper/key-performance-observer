/*
 * DataCollection.h
 *
 *  Created on: Jun 28, 2011
 */

#ifndef DATACOLLECTION_H_
#define DATACOLLECTION_H_


int DataCollection();
int DestroyDataCollector();
int InitDataCollector(long nClockTicksPerSecond, long nNumberOfCpusConfigured, long nNumberOfNetsConfigured, char cNicType, long nNumberOfDisksConfigured, char cDiskType);
int LoopDataCollection();

#endif /* DATACOLLECTION_H_ */
