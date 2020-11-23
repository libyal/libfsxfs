#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libfsxfs/.libs/libfsxfs.1.dylib && test -f ./pyfsxfs/.libs/pyfsxfs.so;
then
	install_name_tool -change /usr/local/lib/libfsxfs.1.dylib ${PWD}/libfsxfs/.libs/libfsxfs.1.dylib ./pyfsxfs/.libs/pyfsxfs.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

