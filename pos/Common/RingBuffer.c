/*
 * RingBuffer.c
 *
 *  Created on: Jun 21, 2011
 *
 */

#include "RingBuffer.h"

// for malloc().
#include <stdlib.h>

// for memcpy().
#include <string.h>

int DestroyRingBuffer(struct RingBuffer *pStruct) {
	int nStatus = 0;

	if (pStruct != NULL) {
		if (pStruct->pBuffer != NULL) {
			free(pStruct->pBuffer);
		} else {
			nStatus = -2;
		}
		if (pStruct->pWritersBuffer != NULL) {
			free(pStruct->pWritersBuffer);
		} else {
			nStatus = -3;
		}
		free(pStruct);
	} else {
		nStatus = -1;
	}
	return (nStatus);
} // end destroyringbuffer.


void DumpCharRingBufferInfo(struct RingBuffer *pStruct) {
	printf("pStruct...: %p\n", (void *) pStruct);
	printf("pBufferEnd: %p\n", pStruct->pBufferEnd);
	printf("pRingHead.: %p\n", pStruct->pRingHead);
	printf("pRingTail.: %p\n", pStruct->pRingTail);
	DumpCharRingBufferData(pStruct);
}

void DumpCharRingBufferData(struct RingBuffer *pStruct) {

	void *pIndex = pStruct->pRingHead;

	printf(">>>");
	// this while actually starts by checking that pRingHead and pRingTail isn't pointing at the same cell.
	while (pIndex != pStruct->pRingTail) {
		printf("%c", *((char *) pIndex));
		pIndex++;
		if (pIndex == pStruct->pBufferEnd) {
			pIndex = pStruct->pBuffer;
		}
	}
	puts("<<<");
}

/**
 * Look in the ring buffer for a record.
 * If the record is found then pRecordBuffer is filled. Provided that there is room for it.
 * @return - 0: Record found and there was room enough. pRecordBuffer is now a zero terminated string with the record.
 *   -1: No cRecordEndMark found in pStruct->pBuffer.
 *   -2: Record found but it is bigger than nMaxRecordSize.
 */
int GetCharRecord(struct RingBuffer *pStruct, char cRecordEndMark,
		char *pRecordBuffer, size_t nMaxRecordSize) {
	int nStatus = 0;
	int nFoundFirstRecord = 0;
	size_t nRecordLength = 0;
	void *pCurrentStartPostion = NULL; // used when the record wraps around the end of the pBufferEnd

	char *pEndMarkPosition = NULL;
	TRACEF(7,("    GetCharRecord(%p, 0x%X. %p, %Zi) \n", pStruct, cRecordEndMark, pRecordBuffer, nMaxRecordSize));

	if (pStruct->pRingHead == pStruct->pRingTail) {
		TRACEF(7,("    GetCharRecord() --- Buffer is empty.\n"));
		nStatus = RING_BUFFER_ERROR__BUFFER_IS_EMPTY;
	}
	if (pStruct->pRingHead < pStruct->pRingTail) {
		TRACEF(7,("    GetCharRecord() --- pRingHead before pRingTail.\n"));
		pEndMarkPosition = memchr(pStruct->pRingHead, cRecordEndMark,
				pStruct->pRingTail - pStruct->pRingHead);
		nFoundFirstRecord = 1;
		nRecordLength = (void *) pEndMarkPosition + 1 - pStruct->pRingHead;
	}

	if (pStruct->pRingTail < pStruct->pRingHead) {
		TRACEF(7,("    IMPLERMENT THIS !!! GetCharRecord() status=%d --- pRingTail before pRingHead.\n", nStatus));
		// From head to end
		pCurrentStartPostion = pStruct->pRingHead;
		pEndMarkPosition = memchr(pCurrentStartPostion, cRecordEndMark,
				pStruct->pBufferEnd - pStruct->pRingHead);
		if (pEndMarkPosition == NULL) {
			nRecordLength = pStruct->pBufferEnd - pStruct->pRingHead;
			// from buffer start to tail.
			pCurrentStartPostion = pStruct->pBuffer;
			pEndMarkPosition = memchr(pCurrentStartPostion = pStruct->pBuffer,
					cRecordEndMark, pStruct->pRingTail - pStruct->pBuffer);
		} // if endmark not found.
		if (pEndMarkPosition == NULL) {
			// IFF end mark found, then copy into buffer. Actually I have the length and can simply pop it, the pop function will take care of the rest.
			nRecordLength += (void *) pEndMarkPosition + 1
					- pCurrentStartPostion;
		}
	}

	if (pEndMarkPosition != NULL) {
		TRACEF(7,("    GetCharRecord() mark found.\n"));
		if (nMaxRecordSize >= (nRecordLength)) {
			size_t nBufferLength = PopFromCharRingBuffer(pStruct,
					pRecordBuffer, nRecordLength);
			TRACEF(7,("    GetCharRecord() --- BufLen: %Zi, RecLen: %Zi, nMaxRecordSize: %Zi .\n", nBufferLength, nRecordLength, nMaxRecordSize));
			if (nBufferLength == nRecordLength) {
				// -1 to ensure it is the record marker that is overwritten.
				pRecordBuffer[nBufferLength - 1] = '\0';
				TRACEF(7,("    GetCharRecord() --- nBufferLength(%Zi) '%s'.\n", nBufferLength, pRecordBuffer));
			} else {
				TRACEF(7,("    GetCharRecord() --- nBufferLength(%Zi) != nRecordLength(%Zi).\n", nBufferLength, nRecordLength));
				nStatus = -1;
			}
		} else {
			TRACEF(7,("    GetCharRecord() pRecordBuffer not big enough nRecordLength(%Zi) > nMaxRecordSize(%Zi)-1.\n", nRecordLength, nMaxRecordSize));
			nStatus = -2;
		}
	} else {
		TRACEF(7,("    GetCharRecord() mark NOT found. Searched items: %Zi\n", pStruct->pRingTail - pStruct->pRingHead));
		// TODO N Make this depenten on the debug or trace level. DumpCharRingBufferInfo(pStruct);
		nStatus = RING_BUFFER_ERROR__END_MARK_NOT_FOUND;
	}

	return (nStatus);
}

/**
 * Returns amount of items that are room for in the buffer.
 * TODO N I'm not sure I should implement it like this, If I use nItemsInBuffer, then
 * that would become a critical data location, since both the push and pop functions would be updating
 * the same data, potentially at the same time.
 * @BUG - this calculation could also go wrong, since the Ring(pointer) might be moved during calculations.
 */
size_t GetItemsFree(struct RingBuffer *pStruct) {
	size_t nItemsFree = 0;
	// return (pStruct->nMaximumItems - pStruct->nItemsInBuffer);
	if (pStruct->pRingHead == pStruct->pRingTail) {
		nItemsFree = pStruct->nMaximumItems;
	} else if (pStruct->pRingHead < pStruct->pRingTail) {
		nItemsFree = pStruct->pBufferEnd - pStruct->pRingTail;
		nItemsFree += pStruct->pRingHead - pStruct->pBuffer;
		nItemsFree--;
	} else if (pStruct->pRingTail < pStruct->pRingHead) {
		nItemsFree = pStruct->pRingTail - pStruct->pRingHead - 1;
	}
	return (nItemsFree);
}

/**
 * Initialise a Character ring buffer.
 * TODO V Allocate the pWritersBuffer as well.
 */
struct RingBuffer *InitCharRingBuffer() {
	return (InitCharRingBufferCustomSize(RING_BUFFER_SIZE));
} // end initcharringbuffer.


struct RingBuffer *InitCharRingBufferCustomSize(size_t nRingBufferSize) {

	struct RingBuffer *pStruct = malloc(sizeof(struct RingBuffer));
	if (pStruct != NULL) {
		pStruct->pBuffer = malloc(nRingBufferSize);
		if (pStruct->pBuffer == NULL) {
			free(pStruct);
			pStruct = NULL;
		} else {
			// TODO V This should probably also be a parm.
			pStruct->pWritersBuffer = malloc(WRITERS_BUFFER_SIZE);
			if (pStruct->pWritersBuffer == NULL) {
				free(pStruct->pBuffer);
				free(pStruct);
				pStruct = NULL;
			} else {
				pStruct->pBufferEnd = (char *) pStruct->pBuffer
						+ nRingBufferSize;
				pStruct->nMaximumItems = nRingBufferSize;
				pStruct->nWritersBufferSize = WRITERS_BUFFER_SIZE;
				pStruct->nWritersBufferUncommittedItems = 0;
				pStruct->nItemsInBuffer = 0;
				pStruct->nItemSize = 1;
				pStruct->pRingHead = pStruct->pBuffer;
				pStruct->pRingTail = pStruct->pBuffer;
			}
		}// end else.
	} // end if struct defined.

	return (pStruct);
} // end initcharringbuffer.


/**
 * Don't append the '\0' to the end of the string, since GetCharRecord is depending on this being a clean buffer.
 * @return nItemsToCopy -- actual number of items to copy.
 */
size_t PopFromCharRingBuffer(struct RingBuffer *pStruct, char *pDataBuffer,
		size_t nMaxNumberOfItems) {
	size_t nItemsCopied = 0;

	TRACEF(7,("    PopFromCharRingBuffer(%p, %p, %Zi), ItemsCopied=%Zi\n", pStruct, pDataBuffer, nMaxNumberOfItems, nItemsCopied));

	if (pStruct->pRingHead == pStruct->pRingTail) {
		TRACEF(7,("    PopFromCharRingBuffer(%Zi) --- Buffer is empty.\n", nItemsCopied));
	}
	if (pStruct->pRingTail < pStruct->pRingHead) {
		TRACEF(7,("    PopFromCharRingBuffer(%Zi) --- pRingTail before pRingHead.\n", nItemsCopied));
		// head to end buffer.
		size_t nItemsToCopy = pStruct->pBufferEnd - pStruct->pRingHead;
		if (nItemsToCopy > nMaxNumberOfItems) {
			nItemsToCopy = nMaxNumberOfItems;
		}
		memcpy(pDataBuffer, pStruct->pRingHead, nItemsToCopy);
		nItemsCopied += nItemsToCopy;
		pStruct->pRingHead += nItemsToCopy;
		// wrap around.
		// TODO V Do I have a problem of buffer end item not being read?
		if (pStruct->pRingHead == pStruct->pBufferEnd) {
			pStruct->pRingHead = pStruct->pBuffer;
		}
		// Let the next logic handle the rest.
	}
	if ((pStruct->pRingHead < pStruct->pRingTail) && (nItemsCopied
			< nMaxNumberOfItems)) {
		TRACEF(7,("    PopFromCharRingBuffer(%Zi) --- pRingHead before pRingTail.\n", nItemsCopied));
		size_t nItemsToCopy = pStruct->pRingTail - pStruct->pRingHead;
		if (nItemsToCopy > nMaxNumberOfItems) {
			TRACEF(7,("    PopFromCharRingBuffer(%Zi) --- adjusting nItemsToCopy since nItemsToCopy(%Zi) > nMaxNumberOfItems(%Zi).\n", nItemsCopied, nItemsToCopy, nMaxNumberOfItems));
			nItemsToCopy = nMaxNumberOfItems;
		}
		TRACEF(7,("    PopFromCharRingBuffer(%Zi) ---  nItemsToCopy(%Zi)\n", nItemsCopied, nItemsToCopy));
		memcpy(pDataBuffer, pStruct->pRingHead, nItemsToCopy);
		nItemsCopied += nItemsToCopy;
		pStruct->pRingHead += nItemsToCopy;
	}
	//pDataBuffer[nItemsCopied] = '\0';
	return (nItemsCopied);
} // PopFromCharRingBuffer().


/**
 * @return number of items NOT written to the buffer. 0 means all was written.
 *
 * BUG:
 *  The whole head before tail, etc. I think that is at least processor dependent, I think it depends on how the stack is implemented.
 */
size_t PushToCharRingBuffer(struct RingBuffer *pStruct, char *pInputData,
		size_t nNumberOfItems) {
	size_t nItemsWritten = 0;
	size_t nItemsLeft = nNumberOfItems;
	TRACEF(7,("    PushToCharRingBuffer(%Zi)\n", nItemsLeft));

	// Calc how much room there is before the end of the buffer space.
	//   If  pRingHead is comes between  pRingTail and pBufferEnd, then that is the only room there is left.
	if (pStruct->pRingHead <= pStruct->pRingTail) {
		TRACEF(7,("    PushToCharRingBuffer(%Zi) --- pStruct->pRingHead < pStruct->pRingTail\n", nItemsLeft));
		if ((pStruct->pBufferEnd - pStruct->pRingTail) > nNumberOfItems) {
			TRACEF(7,("    PushToCharRingBuffer(%Zi) --- pStruct->pBufferEnd - pStruct->pRingTail) > nNumberOfItems\n", nItemsLeft));
			// There is room enough, so everything can be written, and we are done.
			memcpy(pStruct->pRingTail, pInputData, nNumberOfItems);
			pStruct->pRingTail += nNumberOfItems;
			nItemsWritten = nNumberOfItems;
			nItemsLeft = 0;
		} else {
			/*
			 *  There is only room for part of the data, so write that data and see if there
			 *   is room once the pRingTail has wrapped around to the start of the buffer
			 *   Then let that part of this func logical handle the rest.
			 */
			TRACEF(7,("    PushToCharRingBuffer(%Zi) --- pStruct->pBufferEnd - pStruct->pRingTail) <= nNumberOfItems\n", nItemsLeft));
			nItemsWritten = pStruct->pBufferEnd - pStruct->pRingTail;
			memcpy(pStruct->pRingTail, pInputData, nItemsWritten);
			pStruct->pRingTail += nItemsWritten;
			nItemsLeft -= nItemsWritten;
			// Advance/wrap pRingTail if pRingHead isnt't pointing at the beginning of pBuffer
			if (pStruct->pRingHead != pStruct->pBuffer) {
				pStruct->pRingTail = pStruct->pBuffer;
			}
		}
	} // endif head before tail.

	if ((pStruct->pRingTail < pStruct->pRingHead - 1) && (nItemsLeft > 0)) {
		TRACEF(7,("    PushToCharRingBuffer(%Zi) -- pStruct->pRingTail < pStruct->pRingHead - 1) && (nItemsLeft > 0\n", nItemsLeft));

		if ((pStruct->pRingHead - 1 - pStruct->pRingTail) > nItemsLeft) {
			// There is room for the rest of the data.
			TRACEF(7,("    PushToCharRingBuffer(%Zi) --- pStruct->pRingHead - 1 - pStruct->pRingTail) > nItemsLeft\n", nItemsLeft));
			memcpy(pStruct->pRingTail, pInputData, nItemsLeft);
			pStruct->pRingTail += nItemsLeft;
			nItemsWritten += nItemsLeft;
			nItemsLeft = 0;
		} else {
			TRACEF(7,("    PushToCharRingBuffer(%Zi) pStruct->pRingHead - 1 - pStruct->pRingTail) <= nItemsLeft\n", nItemsLeft));
			size_t nItemsToCopy = pStruct->pRingHead - 1 - pStruct->pRingTail;
			memcpy(pStruct->pRingTail, pInputData, nItemsToCopy);
			pStruct->pRingTail += nItemsToCopy;
			nItemsWritten += nItemsToCopy;
			nItemsLeft -= nItemsToCopy;
		}

	} // endif tail before head.


	return (nItemsLeft);
} // end pushtocharringbuffer.

/**
 * Clear the Ring Buffer for use on the next file.
 */
int ResetRingBuffer(struct RingBuffer *pStruct) {
	int nStatus = -1;

	if (pStruct != NULL) {
		pStruct->pRingHead = pStruct->pBuffer;
		pStruct->pRingTail = pStruct->pRingHead;
		pStruct->nItemsInBuffer = 0;
		nStatus = 0;
	}

	return (nStatus);
}
