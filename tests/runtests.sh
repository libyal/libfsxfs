#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libfsxfs/.libs/libfsxfs.1.dylib" ] && [ -f ./pyfsxfs/.libs/pyfsxfs.so ]
then
    install_name_tool -change /usr/local/lib/libfsxfs.1.dylib "${PWD}/libfsxfs/.libs/libfsxfs.1.dylib" ./pyfsxfs/.libs/pyfsxfs.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

