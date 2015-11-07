/*
 * RingBuffer.h
 *
 *  Created on: Jun 21, 2011
 *
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <unistd.h>

#include "../Common/FunctionTracing.h"

#define RING_BUFFER_SIZE 65000
/*
 *  This is the size of the buffer that the push app can use for storing the data that is to be pushed into the ring.
 *  I got inspired by a paper from Red Hat on ring buffers in the kernel.
 */
#define WRITERS_BUFFER_SIZE 31000

#define RING_BUFFER_ERROR__END_MARK_NOT_FOUND -3
#define RING_BUFFER_ERROR__BUFFER_IS_EMPTY -4


struct RingBuffer {
	void *pBuffer;           // Pointer to the data buffer
	void *pBufferEnd;        // end of data buffer
	void *pWritersBuffer;    // Buffer that can be used by writing process, so that process wont have to bother with allocating a buffer.
	size_t nWritersBufferSize; // Size of the pWritersBuffer.
	size_t nWritersBufferUncommittedItems; // Number of items in pWritersBuffer, not yet committed to the ring pBuffer.
	size_t nMaximumItems;    // maximum number of items in the buffer
	size_t nItemsInBuffer;   // number of items in the buffer
	size_t nItemSize;        // size of each item in the buffer, in bytes.
	void *pRingHead;         // pointer to head
	void *pRingTail;         // pointer to tail, point to the first free cell.
};

/*
 * if pRingTail and pRingHead points to the same location, it means the ring is empty.
 * Is that an acceptable concept, or do I need to have a counter, that tells me how many
 * items there are in the buffer. But if I use such a counter, then I need a mutex to ensure that
 * reader and writer does not update the counter at the same time.
 */

int DestroyRingBuffer(struct RingBuffer *pStruct);
void DumpCharRingBufferData(struct RingBuffer *pStruct);
void DumpCharRingBufferInfo(struct RingBuffer *pStruct);
int GetCharRecord(struct RingBuffer *pStruct, char cRecordEndMark, char *pRecordBuffer, size_t nMaxRecordSize);
size_t GetItemsFree(struct RingBuffer *pStruct);
struct RingBuffer *InitCharRingBuffer();
struct RingBuffer *InitCharRingBufferCustomSize(size_t nRingBufferSize);
size_t PopFromCharRingBuffer(struct RingBuffer *pStruct, char *pDataBuffer, size_t nMaxNumberOfItems);
size_t PushToCharRingBuffer(struct RingBuffer *pStruct, char *pInputData, size_t nNumberOfItems);
int ResetRingBuffer(struct RingBuffer *pStruct);


#endif /* RINGBUFFER_H_ */
