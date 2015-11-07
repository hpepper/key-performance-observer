/** check_the functions in the 'Common' dir.
 * See: http://check.sourceforge.net/doc/check_html/check_3.html#SEC4
 */
// For unlink();
#include <unistd.h>
#include <stdlib.h>
#include <check.h>
#include "../FileOps/FileRecordHandler.h"

#include "check_Common.h"

#include "../Common/FunctionTracing.h"
#include "../Common/RingBuffer.h"
#include "../Common/GetDeviceInfo.h"

START_TEST( test_InitCharRingBuffer)
	{
		int nRc = -1;

		struct RingBuffer *pStruct = InitCharRingBuffer();

		fail_unless(pStruct != NULL, "InitCharRingBuffer() test.");
		nRc = DestroyRingBuffer(pStruct);

	}END_TEST

START_TEST( test_GetItemsFree)
	{
		size_t nRc = 0;

		struct RingBuffer *pStruct = InitCharRingBuffer();

		nRc = GetItemsFree(pStruct);
		fail_unless(nRc == RING_BUFFER_SIZE, "GetItemsFree() test.");

		DestroyRingBuffer(pStruct);

	}END_TEST

START_TEST( test_PushToCharRingBuffer)
	{
		int nRc = -1;

		size_t nItemsLeft = 999;
		char szBuffer[] = "LiveMem";

		struct RingBuffer *pStruct = InitCharRingBuffer();

		if (pStruct != NULL) {
			nItemsLeft = PushToCharRingBuffer(pStruct, szBuffer, strlen(
					szBuffer));
			DumpCharRingBufferInfo(pStruct);
		}
		TRACEF(7,("    Items Left: %Zi\n", nItemsLeft));

		fail_unless(nItemsLeft == 0, "PushToCharRingBuffer() not all data written.");
		nRc = DestroyRingBuffer(pStruct);

	}END_TEST

START_TEST( test_PopFromCharRingBuffer)
	{
		int nRc = -1;

		size_t nItemsLeft = 999;
		size_t nPopBufferSize = 14;
		size_t nItemsRead = 0;

		char szBuffer1[] = "LiveMem1";
		char szBuffer2[] = "LiveMem2";
		char szPopBuffer[14] = "";

		struct RingBuffer *pStruct = InitCharRingBuffer();

		TRACEF(7,("    test_PopFromCharRingBuffer()\n"));
		if (pStruct != NULL) {
			nItemsLeft = PushToCharRingBuffer(pStruct, szBuffer1, strlen(
					szBuffer1));
			DumpCharRingBufferInfo(pStruct);
			nItemsLeft = PushToCharRingBuffer(pStruct, szBuffer2, strlen(
					szBuffer2));
			DumpCharRingBufferInfo(pStruct);
			TRACEF(7,("    Items read: %Zi writen: '%s' read: '%s'\n", nItemsRead, szBuffer1, szPopBuffer ));
			nItemsRead = PopFromCharRingBuffer(pStruct, szPopBuffer, 8);
			DumpCharRingBufferInfo(pStruct);
			szPopBuffer[nItemsRead];
		}
		TRACEF(7,("    Items read: %Zi writen: '%s' read: '%s'\n", nItemsRead, szBuffer1, szPopBuffer ));

		fail_unless(strcmp(szBuffer1, szPopBuffer) == 0, "PopFromCharRingBuffer() not all data read.");
		nRc = DestroyRingBuffer(pStruct);

	}END_TEST

START_TEST( test_PopFromCharRingBuffer_read_beyond_buffer)
	{
		int nRc = -1;

		size_t nItemsLeft = 999;
		size_t nPopBufferSize = 14;
		size_t nItemsRead = 0;

		char szBuffer[] = "LiveMem";
		char szPopBuffer[14] = "";

		TRACEF(7,("    test_PopFromCharRingBuffer_read_beyond_buffer()\n"));
		struct RingBuffer *pStruct = InitCharRingBuffer();

		if (pStruct != NULL) {
			nItemsLeft = PushToCharRingBuffer(pStruct, szBuffer, strlen(
					szBuffer));
			nItemsRead = PopFromCharRingBuffer(pStruct, szPopBuffer,
					nPopBufferSize - 1);
			szPopBuffer[nItemsRead];
			TRACEF(7,("    Items read: %Zi writen: '%s' read: '%s'\n", nItemsRead, szBuffer, szPopBuffer ));
			nItemsRead = PopFromCharRingBuffer(pStruct, szPopBuffer,
					nPopBufferSize - 1);
		}

		fail_unless(nItemsRead == 0, "PopFromCharRingBuffer() There was data available read.");
		nRc = DestroyRingBuffer(pStruct);

	}END_TEST

START_TEST( test_PopFromCharRingBuffer_walk_the_ring)
	{
		int nRc = -1;

		size_t nItemsLeft = 999;
		size_t nPopBufferSize = 14;
		size_t nItemsRead = 999;

		char szBuffer[] = "LiveMem";
		char szPopBuffer[14] = "";

		TRACEF(7,("    test_PopFromCharRingBuffer_walk_the_ring()\n"));
		struct RingBuffer *pStruct = InitCharRingBufferCustomSize(40);

		if (pStruct != NULL) {
			int nOrgTraceLevel = g_nTraceLevel;
			g_nTraceLevel = 1;
			int nCount;
			for (nCount = 0; nCount < 20; nCount++) {
				nItemsLeft = PushToCharRingBuffer(pStruct, szBuffer, strlen(
						szBuffer));
				nItemsRead = PopFromCharRingBuffer(pStruct, szPopBuffer,
						nPopBufferSize - 1);
				if ((nItemsLeft != 0) || (nItemsRead != 7)) {
					TRACEF(7,("    Items left: %Zi, items read: %Zi writen: '%s' read: '%s'\n", nItemsLeft, nItemsRead, szBuffer, szPopBuffer ));
				}
			}
			g_nTraceLevel = nOrgTraceLevel;
		}

		fail_unless(nItemsRead == 7, "PopFromCharRingBuffer() There was data available read.");
		nRc = DestroyRingBuffer(pStruct);

	}END_TEST

START_TEST( test_PopFromCharRingBuffer_FillThenEmpty)
	{
		int nMisreads = 0;

		size_t nPopBufferSize = 14;

		char szBuffer[12];
		char szPopBuffer[14] = "";

		TRACEF(7,("    test_PopFromCharRingBuffer_FillThenEmpty()\n"));
		struct RingBuffer *pStruct = InitCharRingBufferCustomSize(40);

		if (pStruct != NULL) {

			int nCount = 1;
			size_t nItemsLeft = 0;
			int nOrgTraceLevel = g_nTraceLevel;
			g_nTraceLevel = 1;
			while (nItemsLeft == 0) {
				sprintf(szBuffer, "LiveMem%03d", nCount);
				nCount++;
				nItemsLeft = PushToCharRingBuffer(pStruct, szBuffer, strlen(
						szBuffer));

			}

			printf("--- Items left: %Zi\n", nItemsLeft);

			//DumpCharRingBufferInfo(pStruct);
			size_t nItemsRead = 10;
			nCount = 1;
			while (nItemsRead == 10) {
				sprintf(szBuffer, "LiveMem%03d", nCount);
				nCount++;
				nItemsRead = PopFromCharRingBuffer(pStruct, szPopBuffer, 10);
				if (nItemsRead > 0) {
					if (strcmp(szBuffer, szPopBuffer) != 0) {
						szPopBuffer[nItemsRead] = '\0';
						printf(
								"!!! Popped something unexpected. '%s' != '%s'\n",
								szBuffer, szPopBuffer);
						nMisreads++;
					} // endif strcmp
				} // endif itemsread.
			}
			g_nTraceLevel = nOrgTraceLevel;
		}

		fail_unless(nMisreads == 0, "PopFromCharRingBuffer() Didn't read the expected after popping everything..");
		DestroyRingBuffer(pStruct);
	}END_TEST

START_TEST( test_GetCharRecord)
	{
		int nRc = -1;

		size_t nItemsLeft = 999;
		size_t nPopBufferSize = 14;
		size_t nItemsRead = 0;

		char szBuffer[] = "LiveMem1\nLiveMem2\n";
		char szPopBuffer[14] = "";

		struct RingBuffer *pStruct = InitCharRingBuffer();

		TRACEF(7,("    test_GetCharRecord()\n"));
		if (pStruct != NULL) {
			nItemsLeft = PushToCharRingBuffer(pStruct, szBuffer, strlen(
					szBuffer));
			nItemsRead = GetCharRecord(pStruct, '\n', szPopBuffer,
					nPopBufferSize - 1);
			DumpCharRingBufferData(pStruct);
			TRACEF(7,("    GetCharRecord returned: %Zi writen: '%s' read: '%s'\n", nItemsRead, szBuffer, szPopBuffer ));
			// TRACEF(7,("    GetCharRecord returned: pStruct->pRingHead: '%s'\n", pStruct->pRingHead ));
			nItemsRead = GetCharRecord(pStruct, '\n', szPopBuffer,
					nPopBufferSize - 1);
			DumpCharRingBufferData(pStruct);
			TRACEF(7,("    GetCharRecord returned: %Zi writen: '%s' read: '%s'\n", nItemsRead, szBuffer, szPopBuffer ));
		}

		fail_unless(nItemsRead == 0, "GetCharRecord() Record not found.");
		nRc = DestroyRingBuffer(pStruct);

	}END_TEST

START_TEST( test_GetNumberOfNics)
	{
		int nRc = -2;
		char szNetDevFileName[] = "net_dev";
		struct NicInformation stNicInformation;
		FILE *pFileHandle = fopen(szNetDevFileName, "w");

		TRACEF(3,("    test_GetNumberOfNics()\n"));
		if (pFileHandle != NULL) {
			fprintf(pFileHandle, "\n");
			fprintf(
					pFileHandle,
					"Inter-|   Receive                                                |  Transmit\n");
			fprintf(
					pFileHandle,
					" face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed\n");
			fprintf(
					pFileHandle,
					"    lo:736517538  525971    0    0    0     0          0         0 736517538  525971    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"  eth0:4022315145 4180846    0    0    0     0          0     31792 725452986 2341643    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"  eth1:16908767  234413    0    0    0     0          0     33920  2172428   33932    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"  eth2:46317293  391204    0    0    0     0          0     33920  2172428   33932    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"  eth3:85491375  905079    0    0    0     0          0    120493  9963994  120505    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"   br0:4090610532 6607428    0    0    0     0          0    546877 14588743684 4707179    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"   br1:12691420  234451    0    0    0     0          0    234451      816      12    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"   br2:16725299  248690    0    0    0     0          0    248690      816      12    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"   br3:48315537  792475    0    0    0     0          0    792475  3906334   45425    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					" vnet0: 1452934   31583    0    0    0     0          0         0 32424374  491407    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					" vnet1:  627570    6537    0    0    0     0          0         0 32396080  490984    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					" vnet2:     900      14    0    0    0     0          0         0 15752895  241195    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					" vnet3:     900      14    0    0    0     0          0         0 19708021  255912    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					" vnet4: 3460594   40243    0    0    0     0          0         0 55439670  746976    0    0    0     0       0          0\n");

			fclose(pFileHandle);
			nRc = GetNumberOfNics(szNetDevFileName, &stNicInformation);
			unlink(szNetDevFileName);
		}
		fail_unless( ( (nRc == 0) && (stNicInformation.nBridge == 4) && (stNicInformation.nEth == 4)), "test_GetNumberOfNics() couldn't identify 4 NICs");

	}END_TEST

START_TEST( test_GetNumberOfNics_Eth0)
	{
		int nRc = -2;
		char szNetDevFileName[] = "net_dev";
		struct NicInformation stNicInformation;
		FILE *pFileHandle = fopen(szNetDevFileName, "w");

		TRACEF(3,("    test_GetNumberOfNics_Eth0()\n"));
		if (pFileHandle != NULL) {
			fprintf(pFileHandle, "\n");
			fprintf(
					pFileHandle,
					"Inter-|   Receive                                                |  Transmit\n");
			fprintf(
					pFileHandle,
					" face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed\n");
			fprintf(
					pFileHandle,
					"    lo:736517538  525971    0    0    0     0          0         0 736517538  525971    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"  eth0:4022315145 4180846    0    0    0     0          0     31792 725452986 2341643    0    0    0     0       0          0\n");
			fclose(pFileHandle);
			nRc = GetNumberOfNics(szNetDevFileName, &stNicInformation);
			unlink(szNetDevFileName);
		}
		fail_unless( ( (nRc == 0) && (stNicInformation.nBridge == 0) && (stNicInformation.nEth == 1)), "test_GetNumberOfNics_Eth0() couldn't identify 1 NIC");

	}END_TEST

START_TEST( test_GetNumberOfNics_Eth1Only)
	{
		int nRc = -2;
		char szNetDevFileName[] = "net_dev";
		struct NicInformation stNicInformation;
		FILE *pFileHandle = fopen(szNetDevFileName, "w");

		TRACEF(3,("    test_GetNumberOfNics_Eth1Only()\n"));
		if (pFileHandle != NULL) {
			fprintf(pFileHandle, "\n");
			fprintf(
					pFileHandle,
					"Inter-|   Receive                                                |  Transmit\n");
			fprintf(
					pFileHandle,
					" face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed\n");
			fprintf(
					pFileHandle,
					"    lo:736517538  525971    0    0    0     0          0         0 736517538  525971    0    0    0     0       0          0\n");
			fprintf(
					pFileHandle,
					"  eth1:4022315145 4180846    0    0    0     0          0     31792 725452986 2341643    0    0    0     0       0          0\n");
			fclose(pFileHandle);
			nRc = GetNumberOfNics(szNetDevFileName, &stNicInformation);
			unlink(szNetDevFileName);
		}
		fail_unless( ( (nRc != 0) && (stNicInformation.nBridge == 0) && (stNicInformation.nEth == 0)), "test_GetNumberOfNics_Eth1Only() couldn't identify 1 NIC");

	}END_TEST

START_TEST( test_GetNumberOfDiscs)
	{
		int nRc = -2;
		char szProcFileName[] = "diskstat";
		struct HardDiskInformation stDiskInformation;
		FILE *pFileHandle = fopen(szProcFileName, "w");

		TRACEF(3,("    test_GetNumberOfDiscs()\n"));
		if (pFileHandle != NULL) {
			fprintf(pFileHandle, "\n");
			fprintf(pFileHandle, "   1       0 ram0 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       1 ram1 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       2 ram2 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       3 ram3 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       4 ram4 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       5 ram5 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       6 ram6 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       7 ram7 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       8 ram8 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1       9 ram9 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1      10 ram10 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1      11 ram11 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1      12 ram12 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1      13 ram13 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1      14 ram14 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   1      15 ram15 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       0 loop0 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       1 loop1 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       2 loop2 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       3 loop3 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       4 loop4 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       5 loop5 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       6 loop6 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(pFileHandle, "   7       7 loop7 0 0 0 0 0 0 0 0 0 0 0\n");
			fprintf(
					pFileHandle,
					"   8       0 sda 1058933 9689 62009321 3169662 7705498 1123353 156832694 57612010 0 16703447 60770683\n");
			fprintf(
					pFileHandle,
					"   8       1 sda1 162280 8754 23489636 1430789 212733 1099268 10511424 5907233 0 1373537 7338274\n");
			fprintf(pFileHandle,
					"   8       2 sda2 567 118 5480 769 24 511 4280 225 0 854 993\n");
			fprintf(
					pFileHandle,
					"   8       3 sda3 895924 804 38512805 1737926 7424040 23574 146316990 51701508 0 15713299 53428231\n");
			fprintf(pFileHandle,
					"  11       0 sr0 27 0 216 984 0 0 0 0 0 984 984\n");
			fprintf(
					pFileHandle,
					" 253       1 dm-1 294333 0 11609314 566902 814969 0 31749920 6351258 0 4071556 6918193\n");
			fprintf(
					pFileHandle,
					" 253       0 dm-0 31248 0 1285253 57594 36365 0 3440033 1143516 0 191028 1201112\n");
			fprintf(
					pFileHandle,
					" 253       3 dm-3 30845 0 1393196 60392 23383 0 3467977 1194837 0 104583 1255230\n");
			fclose(pFileHandle);
			nRc = GetNumberOfDisks(szProcFileName, &stDiskInformation);
			unlink(szProcFileName);
		}
		fail_unless( ( (nRc == 0) && (stDiskInformation.nHD == 0) && (stDiskInformation.nSD == 1) && (stDiskInformation.nVD == 0)), "test_GetNumberOfDiscs() couldn't identify 1 NIC");

	}END_TEST

	START_TEST( test_GetNumberOfDiscs_find_vda)
		{
			int nRc = -2;
			char szProcFileName[] = "diskstat";
			struct HardDiskInformation stDiskInformation;
			FILE *pFileHandle = fopen(szProcFileName, "w");

			TRACEF(3,("    test_GetNumberOfDiscs_find_vda()\n"));
			if (pFileHandle != NULL) {
				fprintf(pFileHandle, "\n");
				fprintf(pFileHandle, "   1       0 ram0 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   1       1 ram1 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   1       2 ram2 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   1       3 ram3 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   1       4 ram4 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   1       5 ram5 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   1       6 ram6 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   7       0 loop0 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   7       1 loop1 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   7       2 loop2 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(pFileHandle, "   7       3 loop3 0 0 0 0 0 0 0 0 0 0 0\n");
				fprintf(
						pFileHandle,
						" 252       0 vda 4947 34 86916 12244 15175 676 126824 350021 0 111380 362259\n");
				fprintf(
						pFileHandle,
						" 252       1 vda1 4461 3 82780 11399 15175 676 126824 350021 0 111126 361415\n");
				fprintf(pFileHandle,
						" 252       2 vda2 336 31 2936 683 0 0 0 0 0 682 682\n");
				fclose(pFileHandle);
				nRc = GetNumberOfDisks(szProcFileName, &stDiskInformation);
				unlink(szProcFileName);
			}
			fail_unless( ( (nRc == 0) && (stDiskInformation.nHD == 0) && (stDiskInformation.nSD == 0) && (stDiskInformation.nVD == 1)), "test_GetNumberOfDiscs_find_vda() couldn't identify 1 NIC");

		}END_TEST

Suite *common_suite(void) {
	Suite *s = suite_create("Common");

	/* Core test case */
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_InitCharRingBuffer);
	tcase_add_test(tc_core, test_GetItemsFree);
	tcase_add_test(tc_core, test_PushToCharRingBuffer);
	tcase_add_test(tc_core, test_PopFromCharRingBuffer);
	tcase_add_test(tc_core, test_PopFromCharRingBuffer_read_beyond_buffer);
	tcase_add_test(tc_core, test_PopFromCharRingBuffer_walk_the_ring);
	tcase_add_test(tc_core, test_PopFromCharRingBuffer_FillThenEmpty);
	tcase_add_test(tc_core, test_GetCharRecord);
	tcase_add_test(tc_core, test_GetNumberOfNics);
	tcase_add_test(tc_core, test_GetNumberOfNics);
	tcase_add_test(tc_core, test_GetNumberOfNics_Eth1Only);
	tcase_add_test(tc_core, test_GetNumberOfDiscs);
	tcase_add_test(tc_core, test_GetNumberOfDiscs_find_vda);
	//tcase_add_test(tc_core, );
	//tcase_add_test(tc_core, );

	suite_add_tcase(s, tc_core);

	return s;
}
