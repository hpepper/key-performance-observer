/** check_the functions in the 'GetData' dir.
 * See: http://check.sourceforge.net/doc/check_html/check_3.html#SEC4
 */
#include <stdlib.h>
#include <check.h>

#include "check_Statistics.h"
#include "../Statistics/CpuStatistics.h"
#include "../Statistics/NetStatistics.h"
#include "../Statistics/StorageStatistics.h"
#include "../Statistics/VmStatistics.h"

#include "../Common/FunctionTracing.h"


// CPU
START_TEST( test_CreateCpuStatistics)
	{
		int nRc = -1;

		struct CpuInformationStructure *pStruct = CreateCpuInformationStruct();

		fail_unless(pStruct != NULL, "CreateCpuStatistics() test.");
		nRc = DestroyCpuInfromationStruct(pStruct);

	}END_TEST

START_TEST( test_DestroyCpuStatistics)
	{
		int nRc = -1;

		struct CpuInformationStructure *pStruct = CreateCpuInformationStruct();

		nRc = DestroyCpuInfromationStruct(pStruct);
		fail_unless(nRc==0, "DestroyCpuStatistics() test.");

	}END_TEST

START_TEST( test_InitCpuStatisticsStructures)
	{
		int nRc = -1;

		struct CpuInformationStructure *pStruct = CreateCpuInformationStruct();
		if (pStruct != NULL) {
			InitCpuInformationStruct(pStruct, 2);
			DumpCpuInformationStructure(pStruct);

		}
		fail_unless(pStruct->nNumberOfCpusSupported==2, "InitCpuStatisticsStructures() test.");
		DestroyCpuInfromationStruct(pStruct);

	}END_TEST


	// NET
	START_TEST( test_CreateNetStatistics)
		{
			int nRc = -1;

			struct NetInformationStructure *pStruct = CreateNetInformationStruct();

			fail_unless(pStruct != NULL, "CreateNetStatistics() test.");
			nRc = DestroyNetInfromationStruct(pStruct);

		}END_TEST

	START_TEST( test_DestroyNetStatistics)
		{
			int nRc = -1;

			struct NetInformationStructure *pStruct = CreateNetInformationStruct();

			nRc = DestroyNetInfromationStruct(pStruct);
			fail_unless(nRc==0, "DestroyNetStatistics() test.");

		}END_TEST

	START_TEST( test_InitNetStatisticsStructures)
		{
			int nRc = -1;

			struct NetInformationStructure *pStruct = CreateNetInformationStruct();
			if (pStruct != NULL) {
				InitNetInformationStruct(pStruct, 4,'B');
				DumpNetInformationStructure(pStruct);

			}
			fail_unless(pStruct->nNumberOfNetsSupported==4, "InitNetStatisticsStructures() test.");
			DestroyNetInfromationStruct(pStruct);

		}END_TEST

		// STORAGE

		START_TEST( test_CreateStorageStatistics)
			{
				int nRc = -1;

				struct StorageInformationStructure *pStruct = CreateStorageInformationStruct();

				fail_unless(pStruct != NULL, "CreateStorageStatistics() test.");
				nRc = DestroyStorageInfromationStruct(pStruct);

			}END_TEST

		START_TEST( test_DestroyStorageStatistics)
			{
				int nRc = -1;

				struct StorageInformationStructure *pStruct = CreateStorageInformationStruct();

				nRc = DestroyStorageInfromationStruct(pStruct);
				fail_unless(nRc==0, "DestroyStorageStatistics() test.");

			}END_TEST

		START_TEST( test_InitStorageStatisticsStructures)
			{
				int nRc = -1;

				struct StorageInformationStructure *pStruct = CreateStorageInformationStruct();
				if (pStruct != NULL) {
					InitStorageInformationStructure(pStruct, 2, 's');
					DumpStorageInformationStructure(pStruct);

				}
				fail_unless(pStruct->nNumberOfDisksSupported == 2, "InitStorageStatisticsStructures() test.");
				DestroyStorageInfromationStruct(pStruct);

			}END_TEST



			// VM
			START_TEST( test_CreateVmStatistics)
				{
					int nRc = -1;

					struct VmStatistics *pStruct = CreateVmStatStruct();

					fail_unless(pStruct != NULL, "CreateVmStorageStatStruct() test.");
					nRc = DestroyVmStatStruct(pStruct);

				}END_TEST

			START_TEST( test_DestroyVmStatistics)
				{
					int nRc = -1;


					struct VmStatistics *pStruct = CreateVmStatStruct();

					nRc = DestroyVmStatStruct(pStruct);
					fail_unless(nRc==0, "DestroyVmStatistics() test.");

				}END_TEST

			START_TEST( test_InitVmStatisticsStructures)
				{
					int nRc = -1;

					struct VmStatistics *pStruct = CreateVmStatStruct();
					if (pStruct != NULL) {
						InitVmStatStruct(pStruct);
						DumpVmStatStruct(pStruct);

					}
					fail_unless(pStruct->PageScanned==0, "InitVmStatisticsStructures() test.");
					DestroyVmStatStruct(pStruct);

				}END_TEST


Suite *statistics_suite(void) {
	Suite *s = suite_create("Statistics");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_CreateCpuStatistics);
	tcase_add_test(tc_core, test_DestroyCpuStatistics);
	tcase_add_test(tc_core, test_InitCpuStatisticsStructures);

	tcase_add_test(tc_core, test_CreateNetStatistics);
	tcase_add_test(tc_core, test_DestroyNetStatistics);
	tcase_add_test(tc_core, test_InitNetStatisticsStructures);

	tcase_add_test(tc_core, test_CreateStorageStatistics);
	tcase_add_test(tc_core, test_DestroyStorageStatistics);
	tcase_add_test(tc_core, test_InitStorageStatisticsStructures);

	tcase_add_test(tc_core, test_CreateVmStatistics);
	tcase_add_test(tc_core, test_DestroyVmStatistics);
	tcase_add_test(tc_core, test_InitVmStatisticsStructures);

    //tcase_add_test(tc_core, );
	//tcase_add_test(tc_core, );

	suite_add_tcase(s, tc_core);

	return s;
}
