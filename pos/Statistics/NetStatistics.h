/*
 * NetStatistic.h
 *
 *  Created on: Jul 1, 2011
 *
 *  Source /proc/net/dev
 *  Described in man proc.
 *  EntryName will be: nic0-nic3.
 *
 *  ipackets  -  Field 2
 *  ierrors   -  Field 3
 *  ioctets   -  Field 1
 *  idrop     -  Field 4
 *
 *  opackets  -  Field 10
 *  oerrors   -  Field 11
 *  ooctets   -  Field 9
 *  odrop     -  Field 12
 *
 *  col       -  Field 14
 *  dfr
 *  nocnpt
 *
 */

#ifndef NETSTATISTIC_H_
#define NETSTATISTIC_H_

// The Current hw model supports up to 4 NICs, but we may need an extra card, which would provide up to 8 NICs.
// TODO V Will the vlan vnics have their seperat statistics or will the statistics be counted through e.g. vnic0?
#define MAX_NUMBER_OF_SUPPORTED_NICS 8


struct NetStatistics {
	unsigned long iPackets;
	unsigned long iErrors;
	unsigned long iOctets;
	unsigned long iDrop;
	unsigned long oPackets;
	unsigned long oErrors;
	unsigned long oOctets;
	unsigned long oDrop;
	unsigned long Colissions;
	unsigned long Defered;
	unsigned long NoCanPut;
};


struct NetInformationStructure {
	// it is assumed that the first nic is identified as 'nic0'.
	struct NetStatistics *arNetStatisticsPointerArray[MAX_NUMBER_OF_SUPPORTED_NICS];
	void *pNetStatisticsStorage;
	int nNumberOfNetsSupported; // Number of NICs actually supported must be less than or equal to MAX_NUMBER_OF_SUPPORTED_NICS.
	char cNicType; // 'E' for ethX, 'B' for brX.
};

struct NetInformationStructure *CreateNetInformationStruct();
int DestroyNetInfromationStruct(struct NetInformationStructure *pNetInfoStruct);
int DestroyNetStatStruct(struct NetStatistics *pStruct);
void DumpNetStatStruct(struct NetStatistics *pStruct);
int InitNetStatStruct(struct NetStatistics *pStruct);
int InitNetInformationStruct(struct NetInformationStructure *pStruct, int nNumberOfNics, char cNicType);


#endif /* NETSTATISTIC_H_ */
