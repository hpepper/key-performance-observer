/*
 * FunctionTracing.h
 *
 *  Created on: Dec 4, 2010
 */

#ifndef FUNCTIONTRACING_H_
#define FUNCTIONTRACING_H_

#include <sys/types.h>
#include <unistd.h>

// for fprintf, fflush.
#include <stdio.h>


// Defined in the main c file.
extern int g_nTraceLevel;

#define TRACE_PID 1

/**
 * Trace functions.
 * trace all messages below the level of g_nTraceLevel.
 *
 * The lower the Level, the more important the the message.
 * So trace level 1 messages will be shown whenever trace is active.
 * Trace level 9, is used when very detailed level is used.
 */
#define TRACEF(level,msg) { \
	if ( level <= g_nTraceLevel ) { \
		if (TRACE_PID == 1 ) { \
			printf("%5u-",getpid());\
		}\
		printf("%-20s @ %4u : ", __FILE__, __LINE__); \
		printf msg; \
		fflush(stdout); \
	}  \
}

#endif /* FUNCTIONTRACING_H_ */
