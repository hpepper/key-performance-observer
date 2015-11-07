/** check_the functions in the 'GetData' dir.
 * See: http://check.sourceforge.net/doc/check_html/check_3.html#SEC4
 */
#include <stdlib.h>
#include <check.h>

#include "check_GetData.h"
#include "../GetData/GetProcStat.h"
#include "../GetData/GetProcNetDev.h"
#include "../GetData/GetProcDiskstats.h"
#include "../GetData/GetProcVmstat.h"

#include "../Common/FunctionTracing.h"

#include "../Statistics/NetStatistics.h"

START_TEST( test_InterpretCpuLine)
	{
		int nRc = -1;

		TRACEF(7,("    test_InterpretCpuLine()\n"));
		struct CpuStatistics *pCpuStatStruct = malloc(
				sizeof(struct CpuStatistics));
		if (pCpuStatStruct != NULL) {
			InitCpuStatStruct(pCpuStatStruct);
			// TODO V change the ticks and #cpus to info retrieved from the system.
			nRc = InterpretCpuLine(pCpuStatStruct,
					" 142186 12258 84294 8573230 88115 2 3914 0 0 0", 100);
			DumpCpuStatistics(pCpuStatStruct);
			free(pCpuStatStruct);
		}
		fail_unless(nRc == 0, "InterpretCpuLine() test.");
	}END_TEST

START_TEST( test_GetProcStat)
	{
		int nRc = -1;

		TRACEF(7,("    test_GetProcStat()\n"));
		struct CpuInformationStructure *pStruct = CreateCpuInformationStruct();
		if (pStruct != NULL) {
			InitCpuInformationStruct(pStruct, 1);
			// TODO V change the ticks and #cpus to info retrieved from the system.
			nRc = GetProcStat(pStruct, 100);
			TRACEF(7,("    test_GetProcStat() nRc=%d\n",nRc));
			DumpCpuInformationStructure(pStruct);
			DestroyCpuInfromationStruct(pStruct);
		}
		fail_unless(nRc == 0, "GetProcStat() test.");
	}END_TEST

START_TEST( test_InterpretNetLine)
	{
		int nRc = -1;

		TRACEF(7,("    test_InterpretNetLine()\n"));
		struct NetStatistics *pNetStatStruct = malloc(
				sizeof(struct NetStatistics));
		if (pNetStatStruct != NULL) {
			InitNetStatStruct(pNetStatStruct);
			nRc
					= InterpretNetLine(
							pNetStatStruct,
							"       1       2    3    4    5     6          7         8    10328      45    9    10    11     12       13          14");
			DumpNetStatStruct(pNetStatStruct);
			free(pNetStatStruct);
		}
		fail_unless(nRc == 0, "InterpretCpuLine() test.");
	}END_TEST

START_TEST( test_GetProcNetDev)
	{
		int nRc = -1;

		TRACEF(7,("    test_GetProcNetDev()\n"));
		struct NetInformationStructure *pStruct = CreateNetInformationStruct();
		if (pStruct != NULL) {
			InitNetInformationStruct(pStruct, 1,'E');
			nRc = GetProcNetDev(pStruct);
			TRACEF(7,("    test_GetProcNetDev() nRc=%d\n",nRc));
			DumpNetInformationStructure(pStruct);
			DestroyNetInfromationStruct(pStruct);
		}
		fail_unless(nRc == 0, "GetProcStat() test.");
	}END_TEST

// Storage.
START_TEST( test_InterpretStorageLine)
	{
		int nRc = -1;

		TRACEF(7,("    test_InterpretStorageLine()\n"));
		struct StorageStatistics *pStorageStatStruct = malloc(
				sizeof(struct StorageStatistics));
		if (pStorageStatStruct != NULL) {
			InitStorageStatStruct(pStorageStatStruct);
			// TODO V change the ticks and #cpus to info retrieved from the system.
			nRc
					= InterpretStorageLine(pStorageStatStruct,
							"493134 19865 4805878 3160950 58411 595183 5228936 13085370 0 1988310 16246130");
			DumpStorageStatStruct(pStorageStatStruct);
			free(pStorageStatStruct);
		}
		fail_unless(nRc == 0, "InterpretStorageLine() test.");
	}END_TEST

START_TEST( test_GetProcDiskstat)
	{
		int nRc = -1;

		TRACEF(7,("    test_GetDiskstat()\n"));
		struct StorageInformationStructure *pStruct =
				CreateStorageInformationStruct();
		if (pStruct != NULL) {
			InitStorageInformationStructure(pStruct, 1, 's');
			// TODO V change the ticks and #cpus to info retrieved from the system.
			nRc = GetProcDiskstats(pStruct);
			TRACEF(7,("    test_GetProcDiskstats() nRc=%d\n",nRc));
			DumpStorageInformationStructure(pStruct);
			DestroyStorageInfromationStruct(pStruct);
		}
		fail_unless(nRc == 0, "GetProcStat() test.");
	}END_TEST

// VM
START_TEST( test_GetProcVmstat)
	{
		int nRc = -1;

		TRACEF(7,("    test_GetProcVmstat()\n"));
		struct VmStatistics *pStruct = CreateVmStatStruct();
		if (pStruct != NULL) {
			InitVmStatStruct(pStruct);
			nRc = GetProcVmstat(pStruct);
			TRACEF(7,("    test_GetProcVmstat() nRc=%d\n",nRc));
			DumpVmStatStruct(pStruct);
			DestroyVmStatStruct(pStruct);
		}
		fail_unless(nRc == 0, "GetProcVmstat() test.");
	}END_TEST

START_TEST( test_GetProcMeminfo)
	{
		int nRc = -1;

		TRACEF(7,("    test_GetProcMeminfo()\n"));
		struct VmStatistics *pStruct = CreateVmStatStruct();
		if (pStruct != NULL) {
			InitVmStatStruct(pStruct);
			nRc = GetProcMeminfo(pStruct);
			TRACEF(7,("    GetProcMeminfo() nRc=%d\n",nRc));
			DumpVmStatStruct(pStruct);
			DestroyVmStatStruct(pStruct);
		}
		fail_unless(nRc == 0, "GetProcMeminfo() test.");
	}END_TEST

Suite *getdata_suite(void) {
	Suite *s = suite_create("GetData");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_GetProcStat);
	tcase_add_test(tc_core, test_InterpretCpuLine);
	tcase_add_test(tc_core, test_InterpretNetLine);
	tcase_add_test(tc_core, test_GetProcNetDev);
	tcase_add_test(tc_core, test_InterpretStorageLine);
	tcase_add_test(tc_core, test_GetProcDiskstat);
	tcase_add_test(tc_core, test_GetProcVmstat);
	tcase_add_test(tc_core, test_GetProcMeminfo);
	//tcase_add_test(tc_core, );
	//tcase_add_test(tc_core, );


	suite_add_tcase(s, tc_core);

	return s;
}
