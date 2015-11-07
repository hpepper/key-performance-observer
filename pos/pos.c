/*
 * pos.c
 *
 *  Created on: Jun 15, 2011
 */

// fopen().
#include <stdio.h>

// used by sysconf(), getopt(), getpid().
#include <unistd.h>

#include <stdlib.h>

#include "src/DataCollection.h"
#include "Common/GetDeviceInfo.h"


char f_szVersion[]="POS-1.0.1.1";

// TODO V Add a kill handler.

int main(int argc, char* argv[]) {
	int nStatus = 0;

	int nLoop = 1;

	// TODO N on the released code this must default to '0'  I should put an if DEBUG on this.
	int nCountDown = 0;

	int nSecondsBetweenDataAquisitions = 5;

	int nOpt;

	struct HardDiskInformation stDiskInformation;
	struct NicInformation stNicInformation;
	FILE *pPidFile;

	while ((nOpt = getopt(argc, argv, "Vhn:p:")) != -1) {
		switch (nOpt) {
		case 'V':
			printf("%s\n", f_szVersion);
			exit(0);
			break;
		case 'h':
			printf("%s\n", f_szVersion);
			puts(" -V: print version string.");
			puts(" -h: This help text.");
			puts(" -n: Number of iterations to run before exiting. 0(default) means continuous.");
			puts(" -p: File to put PID into.");
			exit(0);
			break;
		case 'n':
			// TODO V check if it is ':' in optarg? if the value is missing???
			nLoop = atoi(optarg);
			nCountDown = 1;
			break;
		case 'p':
			// TODO V verify that the optarg is a valid path and also that it is not dangerous to write to that file.
			if ( ( pPidFile = fopen(optarg,"w+")) != NULL ) {
				fprintf(pPidFile, "%u\n", getpid());
				fclose(pPidFile);
			} else {
				perror("Unable to open pidfile for writing.");
			}
			break;
		default: /* '?' */
			fprintf(stderr,
					"Usage: %s [-n Number of data points to collect(0 means continuous)] [-p ProcessIdFile] [-h]\n",
					argv[0]);
			exit(EXIT_FAILURE);
		} // end switch
	} // end while

	printf(">>> %s\n", f_szVersion);

	long nClockTicksPerSecond = sysconf(_SC_CLK_TCK);
	printf("    Clock ticks per second: %ld\n", nClockTicksPerSecond);

	long nNumberOfCpusConfigured = sysconf(_SC_NPROCESSORS_CONF);
	printf("    Number of CPUs: %ld\n", nNumberOfCpusConfigured);

	nStatus = GetNumberOfNics("/proc/net/dev", &stNicInformation);
	// TODO N FAIL if nStatus != 0
	if ( nStatus != 0) {
		perror(
				"main() Could not identify any valid NICs, so no data will be collected.");
	}
	int nNumberOfNics = 0;
	char cNicType = '-';

	if (stNicInformation.nBridge == 4) {
		nNumberOfNics = stNicInformation.nBridge;
		cNicType = 'B';
	} else {
		nNumberOfNics = stNicInformation.nEth;
		cNicType = 'E';
	}
	printf("    Number of NICs(%c): %d\n", cNicType, nNumberOfNics);

	nStatus = GetNumberOfDisks("/proc/diskstats", &stDiskInformation);
	// TODO N FAIL if nStatus != 0
	if ( nStatus != 0) {
		perror(
				"main() Could not identify any valid Disks, so no data will be collected.");
	}
	int nNumberOfDisks = 0;
	char cDiskType = '-';

	if (stDiskInformation.nVD > 0) {
		nNumberOfDisks = stDiskInformation.nVD;
		cDiskType = 'v';
	} else if (stDiskInformation.nSD > 0) {
		nNumberOfDisks = stDiskInformation.nSD;
		cDiskType = 's';
	} else if (stDiskInformation.nHD > 0) {
		nNumberOfDisks = stDiskInformation.nHD;
		cDiskType = 'h';
	}
	printf("    Number of Disks(%c): %d\n", cDiskType, nNumberOfDisks);
	nStatus = InitDataCollector(nClockTicksPerSecond, nNumberOfCpusConfigured,
			nNumberOfNics, cNicType, nNumberOfDisks, cDiskType);

	if (nCountDown == 0) {
		// Start by sleeping
		sleep(nSecondsBetweenDataAquisitions);
	}

	while (nLoop > 0) {
		LoopDataCollection();
		if (nCountDown) {
			nLoop--;
			// This if is here so that there wont be any sleep after the last data aquisition, returning faster to the tester.
			if (nLoop > 0) {
				sleep(nSecondsBetweenDataAquisitions);
			}
		} else {
			sleep(nSecondsBetweenDataAquisitions);
		}
	} // end while.

	DestroyDataCollector();

	printf("<<< POS\n");
	return (nStatus);
}
