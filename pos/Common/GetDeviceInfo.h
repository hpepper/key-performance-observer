/*
 * GetDeviceInfo.h
 *
 *  Created on: Jul 6, 2011
 */

#ifndef GETDEVICEINFO_H_
#define GETDEVICEINFO_H_

#define MAX_STATISTICS_LINE 0xff00


struct HardDiskInformation {
	int nHD; // Number of hdX disks. (pata et al.)
	int nSD; // Number of sdX disks. (sata/scsi)
	int nVD; // Number of vdX disks. (Virtual disks.)
};
struct NicInformation {
	int nEth; // Number of eth devices.
	int nBridge; // Number of br devices.
};

int GetNumberOfDisks(char *szFileName, struct HardDiskInformation *pStruct);
int GetNumberOfNics(char *szFileName, struct NicInformation *pStruct);

#endif /* GETDEVICEINFO_H_ */
