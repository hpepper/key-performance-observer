/*
 * VmStatistics.h
 *
 *  Created on: Jun 30, 2011
 *
 *  Source : /proc/vmstat
 *  Statistics collected in pos 1.0
 *   These are per CPU (Name="cpu_stat0")
 *  scan = pgscan_direct_dma + pgscan_direct_dma32 + pgscan_direct_normal + pgscan_direct_movable
 *    TODO V pgsteal may be the use of cache of buffers to get more mem.
 *  rev
 *  maj_fault  - pgmjfault - Only major faults.
 *  as_fault - for now use this for pswpin
 *  hat_fault- for now use this for pswpout
 *
 * TODO V I should probably add pgpgin, pkgpgout and possibly pswpin, pswpout
 *  These have Name=""
 *   freemem    - MemFree
 *   swap_free  - SwapFree
 *
 *   See also Kernel Documentation/filesystems/proc.txt
 *
 */

#ifndef VMSTATISTICS_H_
#define VMSTATISTICS_H_

struct VmStatistics {
	unsigned long PageScanned; // Number of pages scanned to identify pages to be release so they can be used for something else.
//	unsigned long PageScanRevolutions;  // from  Solaris, not sure it is available in Linux. For now use it for Cached
	unsigned long MajorPageFault; //
	unsigned long PageSwapIn; //
	unsigned long PageSwapOut; //
	unsigned long FreeMem; //
	unsigned long FreeSwap; //
	unsigned long MemBuffers;
	unsigned long MemCached;
	unsigned long SwapCached;
};


struct VmStatistics *CreateVmStatStruct();
int DestroyVmStatStruct(struct VmStatistics *pStruct);
void DumpVmStatStruct(struct VmStatistics *pStruct);
int InitVmStatStruct(struct VmStatistics *pStruct);

#endif /* VMSTATISTICS_H_ */
