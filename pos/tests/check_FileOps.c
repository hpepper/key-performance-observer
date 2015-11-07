/** check_the functions in the 'FileOps' dir.
 * See: http://check.sourceforge.net/doc/check_html/check_3.html#SEC4
 */
#include <stdlib.h>
#include <check.h>
#include "../FileOps/FileRecordHandler.h"

#include "../FileOps/StoreCpuData.h"
#include "../FileOps/StoreNetData.h"
#include "../FileOps/StoreStorageData.h"
#include "../FileOps/StoreVmData.h"

#include "check_FileOps.h"

#include "../Common/FunctionTracing.h"
#include "../Statistics/CpuStatistics.h"

// TOCO V Move these functions to a common function file.
int CheckFuncWriteStatFile() {
	int nStatus = 0;
	FILE *pFileHandle = fopen("stat", "w");

	if (pFileHandle != NULL) {
		fprintf(pFileHandle,
				"cpu  142186 12258 84294 8573230 88115 2 3914 0 0 0\n");
		fprintf(pFileHandle,
				"cpu0 142186 12258 84294 8573230 88115 2 3914 0 0 0\n");
		fprintf(
				pFileHandle,
				"intr 5881244 83 28097 0 0 0 0 0 0 0 0 0 0 14889 0 0 125 0 0 0 141507 112976 229328 26 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n");
		fprintf(pFileHandle, "ctxt 18679686\n");
		fprintf(pFileHandle, "btime 1308546504\n");
		fprintf(pFileHandle, "processes 15451\n");
		fprintf(pFileHandle, "procs_running 2\n");
		fprintf(pFileHandle, "procs_blocked 5\n");
		fprintf(pFileHandle,
				"softirq 1549284 0 609428 17824 139418 152653 0 23383 0 9497 597081\n");

		fclose(pFileHandle);
	} else {
		nStatus = -1;
	}
	return (nStatus);
} // end writestatefile.

int CheckFuncRemoveStateFile() {
	int nStatus = unlink("stat");
	return (nStatus);
}

START_TEST( test_OpenFile)
	{
		int nRc = -1;

		nRc = OpenFile("/proc/stat");

		fail_unless(nRc == 0, "OpenFile() test.");
	}END_TEST

START_TEST( test_CloseFile)
	{
		int nRc = -1;

		nRc = OpenFile("/proc/stat");

		nRc = CloseFile();

		fail_unless(nRc == 0, "CloseFile() test.");
	}END_TEST

// Test open twice.
// Test close.
// Test close twice.


START_TEST( test_ReadRecord__null_parm)
	{
		int nRc = -1;

		nRc = ReadRecord(NULL, 0);

		fail_unless(nRc == -2, "ReadRecord() null parm test.");
	}END_TEST

START_TEST( test_ReadRecord__file_not_opened)
	{
		int nRc = -1;

		char szLineRecord[FILE_OPS__MAX_RECORD_LENGTH];
		nRc = ReadRecord(szLineRecord, FILE_OPS__MAX_RECORD_LENGTH);

		fail_unless(nRc == -1, "ReadRecord() file not opened test.");
	}END_TEST

START_TEST( test_ReadRecord__read_first_record)
	{
		int nRc = -1;

		char szLineRecord[FILE_OPS__MAX_RECORD_LENGTH];

		CheckFuncWriteStatFile();

		nRc = OpenFile("stat");

		nRc = ReadRecord(szLineRecord, FILE_OPS__MAX_RECORD_LENGTH);

		TRACEF(7,("First record line(): %s\n", szLineRecord));

		fail_unless(nRc == 0, "ReadRecord() read first record test.");

		nRc = CloseFile();

		CheckFuncRemoveStateFile();

	}END_TEST

START_TEST( test_StoreCpuData)
	{
		int nRc = -1;

		char szDataFileName[] = "DataFile.txt";

		struct CpuInformationStructure *pStruct = CreateCpuInformationStruct();

		TRACEF(3,("   >>> test_StoreCpuData() \n"));

		if (pStruct != NULL) {
			nRc = InitCpuInformationStruct(pStruct, 2);
			TRACEF(7,("         InitCpuInformationStruct called with %p nRc=%d\n", pStruct, nRc));
			FILE *DataFile = fopen(szDataFileName, "w");
			if (DataFile == NULL) {
				perror("Couldn't open the data file for write.");

			} else {
				nRc = StoreCpuData(DataFile, pStruct);
				TRACEF(7,("        StoreCpuData  called with %p nRc=%d\n", pStruct, nRc));
				fclose(DataFile);
				unlink(szDataFileName);
			}

			DestroyCpuInfromationStruct(pStruct);
		}

		fail_unless(nRc == 0, "ReadRecord() StoreCpuData() test.");
	}END_TEST

START_TEST( test_StoreNetData)
	{
		int nRc = -1;

		char szDataFileName[] = "DataFile.txt";

		struct NetInformationStructure *pStruct = CreateNetInformationStruct();

		TRACEF(3,("   >>> test_StoreNetData() \n"));

		if (pStruct != NULL) {
			nRc = InitNetInformationStruct(pStruct, 1, 'E');
			TRACEF(7,("         InitNetInformationStruct called with %p nRc=%d\n", pStruct, nRc));
			FILE *DataFile = fopen(szDataFileName, "w");
			if (DataFile == NULL) {
				perror("Couldn't open the data file for write.");

			} else {
				nRc = StoreNetData(DataFile, pStruct);
				TRACEF(7,("        StoreNetData  called with %p nRc=%d\n", pStruct, nRc));
				fclose(DataFile);
				unlink(szDataFileName);
			}

			DestroyNetInfromationStruct(pStruct);
		}

		fail_unless(nRc == 0, "ReadRecord() StoreNetData() test.");
	}END_TEST

START_TEST( test_StoreStorageData)
	{
		int nRc = -1;

		char szDataFileName[] = "DataFile.txt";

		struct StorageInformationStructure *pStruct =
				CreateStorageInformationStruct();

		TRACEF(3,("   >>> test_StoreStorageData() \n"));

		if (pStruct != NULL) {
			nRc = InitStorageInformationStructure(pStruct, 1, 's');
			TRACEF(7,("         InitStorageInformationStruct called with %p nRc=%d\n", pStruct, nRc));
			FILE *DataFile = fopen(szDataFileName, "w");
			if (DataFile == NULL) {
				perror("Couldn't open the data file for write.");

			} else {
				nRc = StoreStorageData(DataFile, pStruct);
				TRACEF(7,("        StoreStorageData  called with %p nRc=%d\n", pStruct, nRc));
				fclose(DataFile);
				unlink(szDataFileName);
			}

			DestroyStorageInfromationStruct(pStruct);
		}

		fail_unless(nRc == 0, "ReadRecord() StoreStorageData() test.");
	}END_TEST

START_TEST( test_StoreVmData)
	{
		int nRc = -1;

		char szDataFileName[] = "DataFile.txt";

		struct VmStatistics *pStruct = CreateVmStatStruct();

		TRACEF(3,("   >>> test_StoreVmData() \n"));

		if (pStruct != NULL) {
			nRc = InitVmStatStruct(pStruct);
			TRACEF(7,("        InitVmStatStruct  called with %p nRc=%d\n", pStruct, nRc));
			FILE *DataFile = fopen(szDataFileName, "w");
			if (DataFile == NULL) {
				perror("Couldn't open the data file for write.");

			} else {
				nRc = StoreVmData(DataFile, pStruct);
				TRACEF(7,("        StoreVmData  called with %p nRc=%d\n", pStruct, nRc));
				fclose(DataFile);
				unlink(szDataFileName);
			}

			DestroyVmStatStruct(pStruct);
		}

		fail_unless(nRc == 0, "ReadRecord() StoreVmData() test.");
	}END_TEST

Suite *fileops_suite(void) {
	Suite *s = suite_create("FileOps");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_OpenFile);
	tcase_add_test(tc_core, test_CloseFile);

	tcase_add_test(tc_core, test_ReadRecord__null_parm);
	tcase_add_test(tc_core, test_ReadRecord__file_not_opened);
	tcase_add_test(tc_core, test_ReadRecord__read_first_record);

	tcase_add_test(tc_core, test_StoreCpuData);
	tcase_add_test(tc_core, test_StoreNetData);
	tcase_add_test(tc_core, test_StoreStorageData);
	tcase_add_test(tc_core, test_StoreVmData);
	//tcase_add_test(tc_core, );
	//tcase_add_test(tc_core, );


	suite_add_tcase(s, tc_core);

	return s;
}

