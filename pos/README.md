

## Clean-up
rm -rf build-aux autom4te.cache config.h.in configure aclocal.m4


# Troubleshooting

#### /usr/share/automake-1.13/am/ltlibrary.am: warning: 'libCommon.la': linking libtool libraries using a non-POSIX


 autoreconf -i
libtoolize: putting auxiliary files in AC_CONFIG_AUX_DIR, `build-aux'.
libtoolize: copying file `build-aux/ltmain.sh'
libtoolize: Consider adding `AC_CONFIG_MACRO_DIR([m4])' to configure.ac and
libtoolize: rerunning libtoolize, to keep the correct libtool macros in-tree.
libtoolize: Consider adding `-I m4' to ACLOCAL_AMFLAGS in Makefile.am.
configure.ac:19: installing 'build-aux/config.guess'
configure.ac:19: installing 'build-aux/config.sub'
configure.ac:15: installing 'build-aux/install-sh'
configure.ac:15: installing 'build-aux/missing'
automake: warnings are treated as errors
/usr/share/automake-1.13/am/ltlibrary.am: warning: 'libCommon.la': linking libtool libraries using a non-POSIX
/usr/share/automake-1.13/am/ltlibrary.am: archiver requires 'AM_PROG_AR' in 'configure.ac'
Common/Makefile.am:3:   while processing Libtool library 'libCommon.la'
Common/Makefile.am: installing 'build-aux/depcomp'
/usr/share/automake-1.13/am/ltlibrary.am: warning: 'libFileOps.la': linking libtool libraries using a non-POSIX
/usr/share/automake-1.13/am/ltlibrary.am: archiver requires 'AM_PROG_AR' in 'configure.ac'
FileOps/Makefile.am:3:   while processing Libtool library 'libFileOps.la'
/usr/share/automake-1.13/am/ltlibrary.am: warning: 'libGetData.la': linking libtool libraries using a non-POSIX
/usr/share/automake-1.13/am/ltlibrary.am: archiver requires 'AM_PROG_AR' in 'configure.ac'
GetData/Makefile.am:3:   while processing Libtool library 'libGetData.la'
/usr/share/automake-1.13/am/ltlibrary.am: warning: 'libStatistics.la': linking libtool libraries using a non-POSIX
/usr/share/automake-1.13/am/ltlibrary.am: archiver requires 'AM_PROG_AR' in 'configure.ac'
Statistics/Makefile.am:3:   while processing Libtool library 'libStatistics.la'
/usr/share/automake-1.13/am/ltlibrary.am: warning: 'libsrc.la': linking libtool libraries using a non-POSIX
/usr/share/automake-1.13/am/ltlibrary.am: archiver requires 'AM_PROG_AR' in 'configure.ac'
src/Makefile.am:3:   while processing Libtool library 'libsrc.la'
parallel-tests: installing 'build-aux/test-driver'
autoreconf: automake failed with exit status: 1


### configure.ac:28: error: possibly undefined macro: AM_PATH_CHECK
Solution: yum install check check-devel


libtoolize: putting auxiliary files in AC_CONFIG_AUX_DIR, `build-aux'.
libtoolize: copying file `build-aux/ltmain.sh'
libtoolize: Consider adding `AC_CONFIG_MACRO_DIR([m4])' to configure.ac and
libtoolize: rerunning libtoolize, to keep the correct libtool macros in-tree.
libtoolize: Consider adding `-I m4' to ACLOCAL_AMFLAGS in Makefile.am.
configure.ac:28: warning: macro 'AM_PATH_CHECK' not found in library
configure.ac:28: error: possibly undefined macro: AM_PATH_CHECK
      If this token and others are legitimate, please use m4_pattern_allow.
      See the Autoconf documentation.
autoreconf: /usr/bin/autoconf failed with exit status: 1


#### 
configure: error: cannot find install-sh, install.sh, or shtool in build-aux "."/build-aux
[vagrant@localhost pos]$ touch build-aux/install.sh


#### 
./configure: line 3586: AC_PROG_LIBTOOL: command not found
./configure: line 3599: syntax error near unexpected token `ac_ext=c'
./configure: line 3599: `ac_ext=c'


####
Possibly missing 'libtool' rpm
See: https://github.com/maxmind/libmaxminddb/issues/9

configure.ac:28: warning: macro 'AM_PATH_CHECK' not found in library
configure.ac:19: error: possibly undefined macro: AC_PROG_LIBTOOL
      If this token and others are legitimate, please use m4_pattern_allow.
      See the Autoconf documentation.
configure.ac:28: error: possibly undefined macro: AM_PATH_CHECK
autoreconf: /usr/bin/autoconf failed with exit status: 1

