/*
 * FileRecordHandler.h
 *
 *  Created on: Jun 18, 2011
 *      Author: cadm
 */

#ifndef FILERECORDHANDLER_H_
#define FILERECORDHANDLER_H_

// TODO V make this bit dependent, so it calculate to e.g. max int etc.
#define FILE_OPS__MAX_RECORD_LENGTH 31000

int CloseFile();
int OpenFile(char *pFileName);
int ReadRecord(char *pStringRecord, int nMaxRecordLength);

#endif /* FILERECORDHANDLER_H_ */
