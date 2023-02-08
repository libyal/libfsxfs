/*
 * Library superblock type test program
 *
 * Copyright (C) 2020-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsxfs_test_functions.h"
#include "fsxfs_test_libbfio.h"
#include "fsxfs_test_libcerror.h"
#include "fsxfs_test_libfsxfs.h"
#include "fsxfs_test_macros.h"
#include "fsxfs_test_memory.h"
#include "fsxfs_test_unused.h"

#include "../libfsxfs/libfsxfs_superblock.h"

uint8_t fsxfs_test_superblock_data1[ 512 ] = {
	0x58, 0x46, 0x53, 0x42, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xf0, 0xa2, 0xfc, 0x3b, 0x18, 0x47, 0xa6, 0x8e, 0xce, 0x3d, 0x94, 0x3a, 0x9f, 0x12, 0x4d,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x40,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x42,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x05, 0x58, 0xb4, 0xb5, 0x02, 0x00, 0x02, 0x00, 0x00, 0x08, 0x78, 0x66, 0x73, 0x5f,
	0x74, 0x65, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x09, 0x09, 0x03, 0x0c, 0x00, 0x00, 0x19,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x8a, 0x00, 0x00, 0x01, 0x8a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
	0x26, 0xe1, 0xa5, 0xe0, 0x00, 0x00, 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

/* Tests the libfsxfs_superblock_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_superblock_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libfsxfs_superblock_t *superblock = NULL;
	int result                        = 0;

#if defined( HAVE_FSXFS_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 1;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libfsxfs_superblock_initialize(
	          &superblock,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "superblock",
	 superblock );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_superblock_free(
	          &superblock,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "superblock",
	 superblock );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_superblock_initialize(
	          NULL,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	superblock = (libfsxfs_superblock_t *) 0x12345678UL;

	result = libfsxfs_superblock_initialize(
	          &superblock,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	superblock = NULL;

#if defined( HAVE_FSXFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsxfs_superblock_initialize with malloc failing
		 */
		fsxfs_test_malloc_attempts_before_fail = test_number;

		result = libfsxfs_superblock_initialize(
		          &superblock,
		          &error );

		if( fsxfs_test_malloc_attempts_before_fail != -1 )
		{
			fsxfs_test_malloc_attempts_before_fail = -1;

			if( superblock != NULL )
			{
				libfsxfs_superblock_free(
				 &superblock,
				 NULL );
			}
		}
		else
		{
			FSXFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSXFS_TEST_ASSERT_IS_NULL(
			 "superblock",
			 superblock );

			FSXFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfsxfs_superblock_initialize with memset failing
		 */
		fsxfs_test_memset_attempts_before_fail = test_number;

		result = libfsxfs_superblock_initialize(
		          &superblock,
		          &error );

		if( fsxfs_test_memset_attempts_before_fail != -1 )
		{
			fsxfs_test_memset_attempts_before_fail = -1;

			if( superblock != NULL )
			{
				libfsxfs_superblock_free(
				 &superblock,
				 NULL );
			}
		}
		else
		{
			FSXFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSXFS_TEST_ASSERT_IS_NULL(
			 "superblock",
			 superblock );

			FSXFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSXFS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( superblock != NULL )
	{
		libfsxfs_superblock_free(
		 &superblock,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsxfs_superblock_free function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_superblock_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsxfs_superblock_free(
	          NULL,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsxfs_superblock_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_superblock_read_data(
     void )
{
	libcerror_error_t *error          = NULL;
	libfsxfs_superblock_t *superblock = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libfsxfs_superblock_initialize(
	          &superblock,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "superblock",
	 superblock );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsxfs_superblock_read_data(
	          superblock,
	          fsxfs_test_superblock_data1,
	          512,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_superblock_read_data(
	          NULL,
	          fsxfs_test_superblock_data1,
	          512,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsxfs_superblock_read_data(
	          superblock,
	          NULL,
	          512,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsxfs_superblock_read_data(
	          superblock,
	          fsxfs_test_superblock_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsxfs_superblock_read_data(
	          superblock,
	          fsxfs_test_superblock_data1,
	          0,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsxfs_superblock_free(
	          &superblock,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "superblock",
	 superblock );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( superblock != NULL )
	{
		libfsxfs_superblock_free(
		 &superblock,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsxfs_superblock_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_superblock_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle  = NULL;
	libcerror_error_t *error          = NULL;
	libfsxfs_superblock_t *superblock = NULL;
	int result                        = 0;

	/* Initialize test
	 */
	result = libfsxfs_superblock_initialize(
	          &superblock,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "superblock",
	 superblock );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = fsxfs_test_open_file_io_handle(
	          &file_io_handle,
	          fsxfs_test_superblock_data1,
	          512,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsxfs_superblock_read_file_io_handle(
	          superblock,
	          file_io_handle,
	          0,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_superblock_read_file_io_handle(
	          NULL,
	          file_io_handle,
	          0,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsxfs_superblock_read_file_io_handle(
	          superblock,
	          NULL,
	          0,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsxfs_superblock_read_file_io_handle(
	          superblock,
	          file_io_handle,
	          -1,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up file IO handle
	 */
	result = fsxfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data too small
	 */
	result = fsxfs_test_open_file_io_handle(
	          &file_io_handle,
	          fsxfs_test_superblock_data1,
	          8,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_superblock_read_file_io_handle(
	          superblock,
	          file_io_handle,
	          0,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = fsxfs_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test data invalid
	 */
/* TODO implement */

	/* Clean up
	 */
	result = libfsxfs_superblock_free(
	          &superblock,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "superblock",
	 superblock );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( superblock != NULL )
	{
		libfsxfs_superblock_free(
		 &superblock,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSXFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSXFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSXFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSXFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
	FSXFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSXFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

	FSXFS_TEST_RUN(
	 "libfsxfs_superblock_initialize",
	 fsxfs_test_superblock_initialize );

	FSXFS_TEST_RUN(
	 "libfsxfs_superblock_free",
	 fsxfs_test_superblock_free );

	FSXFS_TEST_RUN(
	 "libfsxfs_superblock_read_data",
	 fsxfs_test_superblock_read_data );

	FSXFS_TEST_RUN(
	 "libfsxfs_superblock_read_file_io_handle",
	 fsxfs_test_superblock_read_file_io_handle );

/* TODO add additional tests */

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */
}

