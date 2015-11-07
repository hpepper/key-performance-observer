/*
 * GetDeviceInfo.c
 *
 *  Created on: Jul 6, 2011
 */

#include "GetDeviceInfo.h"

#include "../Common/FunctionTracing.h"
#include "../FileOps/FileRecordHandler.h"

#include <string.h>

/**
 * Added szFileName as a parm so that the unit test can feed the various test aspects.
 */
int GetNumberOfNics(char *szFileName, struct NicInformation *pStruct) {
	int nStatus = -1;

	char szLine[MAX_STATISTICS_LINE] = "";

	int nFoundDevicesBitPattern = 0;
#define ETH0_BIT_PATTERN 1
#define ETH1_BIT_PATTERN 2
#define ETH2_BIT_PATTERN 4
#define ETH3_BIT_PATTERN 8
#define BR0_BIT_PATTERN 16
#define BR1_BIT_PATTERN 32
#define BR2_BIT_PATTERN 64
#define BR3_BIT_PATTERN 128

	int nEtherBitPattern=ETH0_BIT_PATTERN
			| ETH1_BIT_PATTERN
			| ETH2_BIT_PATTERN
			| ETH3_BIT_PATTERN;

	int nBridgeBitPattern = BR0_BIT_PATTERN
			| BR1_BIT_PATTERN
			| BR2_BIT_PATTERN
			| BR3_BIT_PATTERN;
	int nMaxDevicesBitPattern =  nEtherBitPattern | nBridgeBitPattern;


	TRACEF(5,(">>> GetNumberOfNics()\n"));

	pStruct->nEth = 0;
	pStruct->nBridge = 0;

	nStatus = OpenFile(szFileName);
	while ((nStatus == 0) && (nFoundDevicesBitPattern != nMaxDevicesBitPattern)){
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);
		if (nStatus == 0) {
			if (strncmp("  eth0:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= ETH0_BIT_PATTERN;
			}
			if (strncmp("  eth1:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= ETH1_BIT_PATTERN;
			}
			if (strncmp("  eth2:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= ETH2_BIT_PATTERN;
			}
			if (strncmp("  eth3:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= ETH3_BIT_PATTERN;
			}
			if (strncmp("   br0:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= BR0_BIT_PATTERN;
			}
			if (strncmp("   br1:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= BR1_BIT_PATTERN;
			}
			if (strncmp("   br2:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= BR2_BIT_PATTERN;
			}
			if (strncmp("   br3:", szLine, 7) == 0) {
				nFoundDevicesBitPattern |= BR3_BIT_PATTERN;
			}
		} // endif status
		TRACEF(7,("   GetNumberOfNics() Status=%d, FoundDevicesBitPattern=%d\n", nStatus,nFoundDevicesBitPattern));

	} // end while.

	CloseFile();

	int nFoundEtherBitPattern = nFoundDevicesBitPattern & nEtherBitPattern;
	int nFoundBridgeBitPattern = nFoundDevicesBitPattern & nBridgeBitPattern;
	if ( nFoundEtherBitPattern == ETH0_BIT_PATTERN ) {
		pStruct->nEth = 1;
	} else  if ( nFoundEtherBitPattern == (ETH0_BIT_PATTERN | ETH1_BIT_PATTERN) ) {
		pStruct->nEth = 2;
	} else  if ( nFoundEtherBitPattern == (ETH0_BIT_PATTERN | ETH1_BIT_PATTERN | ETH2_BIT_PATTERN)) {
		pStruct->nEth = 3;
	} else  if ( nFoundEtherBitPattern == (ETH0_BIT_PATTERN | ETH1_BIT_PATTERN | ETH2_BIT_PATTERN | ETH3_BIT_PATTERN)) {
		pStruct->nEth = 4;
	}
	if ( nFoundBridgeBitPattern == BR0_BIT_PATTERN ) {
		pStruct->nBridge = 1;
	} else  if ( nFoundBridgeBitPattern == (BR0_BIT_PATTERN | BR1_BIT_PATTERN) ) {
		pStruct->nBridge = 2;
	} else  if ( nFoundBridgeBitPattern == (BR0_BIT_PATTERN | BR1_BIT_PATTERN | BR2_BIT_PATTERN)) {
		pStruct->nBridge = 3;
	} else  if ( nFoundBridgeBitPattern == (BR0_BIT_PATTERN | BR1_BIT_PATTERN | BR2_BIT_PATTERN | BR3_BIT_PATTERN)) {
		pStruct->nBridge = 4;
	}

	if ((pStruct->nEth>0) || (pStruct->nBridge>0) ) {
		nStatus=0;
	}
	return(nStatus);
}



/**
 * Added szFileName as a parm so that the unit test can feed the various test aspects.
 */
int GetNumberOfDisks(char *szFileName, struct HardDiskInformation *pStruct) {
	int nStatus = -1;

	char szLine[MAX_STATISTICS_LINE] = "";

	int nDiskNameOffset=13;

	int nFoundDevicesBitPattern = 0;
#define HD0_BIT_PATTERN 1
#define HD1_BIT_PATTERN 2
#define HD2_BIT_PATTERN 4
#define HD3_BIT_PATTERN 8
#define SD0_BIT_PATTERN 16
#define SD1_BIT_PATTERN 32
#define SD2_BIT_PATTERN 64
#define SD3_BIT_PATTERN 128
#define VD0_BIT_PATTERN 256
#define VD1_BIT_PATTERN 512
#define VD2_BIT_PATTERN 1024
#define VD3_BIT_PATTERN 2048

	int nHdBitPattern=HD0_BIT_PATTERN
			| HD1_BIT_PATTERN
			| HD2_BIT_PATTERN
			| HD3_BIT_PATTERN;

	int nSdBitPattern = SD0_BIT_PATTERN
			| SD1_BIT_PATTERN
			| SD2_BIT_PATTERN
			| SD3_BIT_PATTERN;
	int nVdBitPattern = VD0_BIT_PATTERN
			| VD1_BIT_PATTERN
			| VD2_BIT_PATTERN
			| VD3_BIT_PATTERN;
	int nMaxDevicesBitPattern =  nHdBitPattern | nSdBitPattern | nVdBitPattern;


	TRACEF(5,(">>> GetNumberOfDisks()\n"));

	pStruct->nHD = 0;
	pStruct->nSD = 0;
	pStruct->nVD = 0;

	nStatus = OpenFile(szFileName);
	while ((nStatus == 0) && (nFoundDevicesBitPattern != nMaxDevicesBitPattern)){
		nStatus = ReadRecord(szLine, MAX_STATISTICS_LINE);
		if (nStatus == 0) {
			if (strncmp("hda ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= HD0_BIT_PATTERN;
			}
			if (strncmp("hdb ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= HD1_BIT_PATTERN;
			}
			if (strncmp("hdc ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= HD2_BIT_PATTERN;
			}
			if (strncmp("hdd ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= HD3_BIT_PATTERN;
			}
			if (strncmp("sda ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= SD0_BIT_PATTERN;
			}
			if (strncmp("sdb ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= SD1_BIT_PATTERN;
			}
			if (strncmp("sdc ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= SD2_BIT_PATTERN;
			}
			if (strncmp("sdd ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= SD3_BIT_PATTERN;
			}
			if (strncmp("vda ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= VD0_BIT_PATTERN;
			}
			if (strncmp("vdb ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= VD1_BIT_PATTERN;
			}
			if (strncmp("vdc ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= VD2_BIT_PATTERN;
			}
			if (strncmp("vdd ", szLine+nDiskNameOffset, 4) == 0) {
				nFoundDevicesBitPattern |= VD3_BIT_PATTERN;
			}
		} // endif status
		TRACEF(7,("   GetNumberOfNics() Status=%d, FoundDevicesBitPattern=%d\n", nStatus,nFoundDevicesBitPattern));

	} // end while.

	CloseFile();

	int nFoundHdBitPattern = nFoundDevicesBitPattern & nHdBitPattern;
	int nFoundSdBitPattern = nFoundDevicesBitPattern & nSdBitPattern;
	int nFoundVdBitPattern = nFoundDevicesBitPattern & nVdBitPattern;
	if ( nFoundHdBitPattern == HD0_BIT_PATTERN ) {
		pStruct->nHD = 1;
	} else  if ( nFoundHdBitPattern == (HD0_BIT_PATTERN | HD1_BIT_PATTERN) ) {
		pStruct->nHD = 2;
	} else  if ( nFoundHdBitPattern == (HD0_BIT_PATTERN | HD1_BIT_PATTERN | HD2_BIT_PATTERN)) {
		pStruct->nHD = 3;
	} else  if ( nFoundHdBitPattern == (HD0_BIT_PATTERN | HD1_BIT_PATTERN | HD2_BIT_PATTERN | HD3_BIT_PATTERN)) {
		pStruct->nHD = 4;
	}
	if ( nFoundSdBitPattern == SD0_BIT_PATTERN ) {
		pStruct->nSD = 1;
	} else  if ( nFoundSdBitPattern == (SD0_BIT_PATTERN | SD1_BIT_PATTERN) ) {
		pStruct->nSD = 2;
	} else  if ( nFoundSdBitPattern == (SD0_BIT_PATTERN | SD1_BIT_PATTERN | SD2_BIT_PATTERN)) {
		pStruct->nSD = 3;
	} else  if ( nFoundSdBitPattern == (SD0_BIT_PATTERN | SD1_BIT_PATTERN | SD2_BIT_PATTERN | SD3_BIT_PATTERN)) {
		pStruct->nSD = 4;
	}
	if ( nFoundVdBitPattern == VD0_BIT_PATTERN ) {
		pStruct->nVD = 1;
	} else  if ( nFoundVdBitPattern == (VD0_BIT_PATTERN | VD1_BIT_PATTERN) ) {
		pStruct->nVD = 2;
	} else  if ( nFoundVdBitPattern == (VD0_BIT_PATTERN | VD1_BIT_PATTERN | VD2_BIT_PATTERN)) {
		pStruct->nVD = 3;
	} else  if ( nFoundVdBitPattern == (VD0_BIT_PATTERN | VD1_BIT_PATTERN | VD2_BIT_PATTERN | VD3_BIT_PATTERN)) {
		pStruct->nVD = 4;
	}

	if ((pStruct->nHD>0) || (pStruct->nSD>0)  || (pStruct->nVD>0)) {
		nStatus=0;
	}
	return(nStatus);
}
