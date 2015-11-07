/*
 * FileRecordHandler.c
 *
 *  Created on: Jun 18, 2011
 *
 *  Open file
 *  GetRecord (get pointer to record
 *     Is the record pointer going to be zero terminated? I think so.
 *  Have a var that is defined when the file has been opened?
 *    Probably use the FileHandler, if zero not init. if set then a file is open. And a new open command can't be called, or it will close the current file?
 *
 *  NOTE: This module is not re-entrant, since there is only one f_nFileHandle.
 */

#include "FileRecordHandler.h"
#include "../Common/FunctionTracing.h"
#include "../Common/RingBuffer.h"

// for stderr
#include <stdio.h>

// for close().
#include <unistd.h>

// For open().
#include <sys/stat.h>
#include <fcntl.h>

// For exit().
#include <stdlib.h>

int f_nFileHandle = 0;

struct RingBuffer *f_pRingBufferStruct = NULL;

char f_pRecordBuffer[FILE_OPS__MAX_RECORD_LENGTH];

/**
 * Close the f_nFileHandle.
 * @return 0 on ok otherwise -1.
 */
int CloseFile() {
	int nStatus = 0;

	if (f_nFileHandle != 0) {
		nStatus = close(f_nFileHandle);
		if (nStatus == 0) {
			TRACEF(5,("    File closed - handle:%d\n", f_nFileHandle));
			f_nFileHandle = 0;
			if (f_pRingBufferStruct != NULL) {
				// TODO V Change this so it calls ResetRingBuffer() instead. And the buffer must be created at a higher level.
				DestroyRingBuffer(f_pRingBufferStruct);
				f_pRingBufferStruct = NULL;
			} //endif ringbuffer initialised.
		} else {
			nStatus = -1;
			perror("!!!  in CloseFile()");
		}
	} else {
		fprintf(stderr,
				"trying to close a filehandle when no file handle has been initialized.\n");
	}
	return (nStatus);
}

/**
 * Open the file named in pFileName.
 * If the f_nFileHandle is already in use, then close it
 *  before opening the new file.
 *
 * Return: 0 on ok, -1 on failure.
 */
int OpenFile(char *pFileName) {
	int nStatus = 0;

	if (f_nFileHandle != 0) {
		nStatus = CloseFile();
		if (nStatus != 0) {
			// TODO V Provide a more lenient way of handling this.
			fprintf(stderr,
					"For some reason, a file seems to be open, but closing it fails\n");
			// TODO V Shouldn't use exit here.
			exit(18);
		}
	}
	// TODO V - can the file content change while it is being read?.
	f_nFileHandle = open(pFileName, O_RDONLY);
	if (f_nFileHandle == -1) {
		// TODO N provide filename in error message.
		perror("OpenFile Could not open file\n");
		nStatus = -1;
	}

	if (nStatus == 0) {
		f_pRingBufferStruct = InitCharRingBuffer();
	}
	TRACEF(5,("    File opened: %s  handle:%d\n", pFileName, f_nFileHandle));

	return (nStatus);
} // end openfile.

/**
 * ReadRecord - read a record from the file identified with f_nFileHandle.
 *
 * A record is a line of text terminated with newline or with EOF.
 *
 * @param pStringRecord - pointer to a character area of at least FILE_OPS__MAX_RECORD_LENGTH
 *  including the null terminator.
 *
 * @return -1 on error.
 *  Fail if f_nFileHandle isn't set.
 *
 *  TODO V Return a specific return value if file EOF is reached.
 */
int ReadRecord(char *pStringRecord, int nMaxRecordLength) {
	int nStatus = 0;

	TRACEF(7,("    >>> ReadRecord()\n"));

	if (pStringRecord == NULL) {
		nStatus = -2;
	}

	if ((nStatus == 0) && (f_nFileHandle <= 0)) {
		nStatus = -1;
	}

	if (nStatus == 0) {
		// If the RingBufferPointer, isn't initialised then call the RingBufferCreate function with the pointer).
		// I should use a ring buffer, so that I don't waste time moving memory around.
		// TODO N Should I keep the buffer under a page size?
		nStatus = GetCharRecord(f_pRingBufferStruct, '\n', pStringRecord, nMaxRecordLength);
		if ( (nStatus == RING_BUFFER_ERROR__END_MARK_NOT_FOUND) || (nStatus == RING_BUFFER_ERROR__BUFFER_IS_EMPTY) ) {
			TRACEF(7,("    ReadRecord() No end mark found, now, or buffer empty.\n"));
			// If data in read buffer, fill them over.
			if (f_pRingBufferStruct->nWritersBufferUncommittedItems != 0) {
				TRACEF(7,("    ReadRecord() Committing last of writer burffer: %Zi\n", f_pRingBufferStruct->nWritersBufferUncommittedItems));
				// commit As much of the buffer as possible.
				f_pRingBufferStruct->nWritersBufferUncommittedItems
						= PushToCharRingBuffer(
								f_pRingBufferStruct,
								f_pRingBufferStruct->pWritersBuffer,
								f_pRingBufferStruct->nWritersBufferUncommittedItems);
			}
			// if all data has been committed then read more data from the file.
			if (f_pRingBufferStruct->nWritersBufferUncommittedItems == 0) {
				// otherwise read in from file.
				ssize_t nReadLength = read(f_nFileHandle,
						f_pRingBufferStruct->pWritersBuffer,
						f_pRingBufferStruct->nWritersBufferSize);
				// TODO V when getting EOF, then push an EndRecordMarker, if pRingTail isn't pointing at one.
				TRACEF(7,("    Read file from handle(%d) to buffer: %Zi, buffer:%p, buffer size=%Zi\n", f_nFileHandle, nReadLength, f_pRingBufferStruct->pWritersBuffer, f_pRingBufferStruct->nWritersBufferSize));
				f_pRingBufferStruct->nWritersBufferUncommittedItems
						= PushToCharRingBuffer(f_pRingBufferStruct,
								f_pRingBufferStruct->pWritersBuffer,
								nReadLength);
				//DumpCharRingBufferInfo(f_pRingBufferStruct);
			}

			// try and see if the end mark can be found now.
			nStatus = GetCharRecord(f_pRingBufferStruct, '\n', pStringRecord, nMaxRecordLength);
			//DumpCharRingBufferInfo(f_pRingBufferStruct);
		}

		if (nStatus == RING_BUFFER_ERROR__END_MARK_NOT_FOUND) {
			TRACEF(7,("    ReadRecord() Final result: RING_BUFFER_ERROR__END_MARK_NOT_FOUND\n"));
			// Major fail.
		}
	} // endif nstatus
	TRACEF(7,("    <<< ReadRecord(): %d\n", nStatus));
	return (nStatus);
} // end readrecord.
