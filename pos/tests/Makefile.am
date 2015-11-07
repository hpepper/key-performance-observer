## Process this file with automake to produce Makefile.in

TESTS = check_all
check_PROGRAMS = check_all
check_all_SOURCES = check_all.c \
                     $(top_builddir)/FileOps/FileRecordHandler.h check_Common.c check_Common.h \
                  check_FileOps.c check_FileOps.h \
                  check_GetData.c check_GetData.h \
                  check_Statistics.c check_Statistics.h 

check_netops_CFLAGS = @CHECK_CFLAGS@
## -lrt is for mq_ ops.
check_all_LDADD = -lrt -lpthread @CHECK_LIBS@  $(top_builddir)/FileOps/libFileOps.la \
                   $(top_builddir)/GetData/libGetData.la \
                   $(top_builddir)/Statistics/libStatistics.la \
                   $(top_builddir)/Common/libCommon.la 

