#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libfsxfs/.libs/libfsxfs.1.dylib && test -f ./pyfsxfs/.libs/pyfsxfs.so
then
	install_name_tool -change /usr/local/lib/libfsxfs.1.dylib ${PWD}/libfsxfs/.libs/libfsxfs.1.dylib ./pyfsxfs/.libs/pyfsxfs.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

