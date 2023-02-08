/*
 * Library file_system_block_header type test program
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

#include "fsxfs_test_libcerror.h"
#include "fsxfs_test_libfsxfs.h"
#include "fsxfs_test_macros.h"
#include "fsxfs_test_memory.h"
#include "fsxfs_test_unused.h"

#include "../libfsxfs/libfsxfs_file_system_block_header.h"
#include "../libfsxfs/libfsxfs_io_handle.h"

uint8_t fsxfs_test_file_system_block_header_data1[ 56 ] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0xee, 0x00, 0x00, 0x83, 0x64, 0x08, 0x8a,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x38, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02,
	0xeb, 0xd6, 0x54, 0x96, 0xec, 0xd8, 0x49, 0x90, 0x95, 0x48, 0x47, 0x85, 0x39, 0x5a, 0x1b, 0x6c,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x4f };

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

/* Tests the libfsxfs_file_system_block_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_file_system_block_header_initialize(
     void )
{
	libcerror_error_t *error                                      = NULL;
	libfsxfs_file_system_block_header_t *file_system_block_header = NULL;
	int result                                                    = 0;

#if defined( HAVE_FSXFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                               = 1;
	int number_of_memset_fail_tests                               = 1;
	int test_number                                               = 0;
#endif

	/* Test regular cases
	 */
	result = libfsxfs_file_system_block_header_initialize(
	          &file_system_block_header,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_system_block_header",
	 file_system_block_header );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_file_system_block_header_free(
	          &file_system_block_header,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "file_system_block_header",
	 file_system_block_header );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_file_system_block_header_initialize(
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

	file_system_block_header = (libfsxfs_file_system_block_header_t *) 0x12345678UL;

	result = libfsxfs_file_system_block_header_initialize(
	          &file_system_block_header,
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

	file_system_block_header = NULL;

#if defined( HAVE_FSXFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsxfs_file_system_block_header_initialize with malloc failing
		 */
		fsxfs_test_malloc_attempts_before_fail = test_number;

		result = libfsxfs_file_system_block_header_initialize(
		          &file_system_block_header,
		          &error );

		if( fsxfs_test_malloc_attempts_before_fail != -1 )
		{
			fsxfs_test_malloc_attempts_before_fail = -1;

			if( file_system_block_header != NULL )
			{
				libfsxfs_file_system_block_header_free(
				 &file_system_block_header,
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
			 "file_system_block_header",
			 file_system_block_header );

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
		/* Test libfsxfs_file_system_block_header_initialize with memset failing
		 */
		fsxfs_test_memset_attempts_before_fail = test_number;

		result = libfsxfs_file_system_block_header_initialize(
		          &file_system_block_header,
		          &error );

		if( fsxfs_test_memset_attempts_before_fail != -1 )
		{
			fsxfs_test_memset_attempts_before_fail = -1;

			if( file_system_block_header != NULL )
			{
				libfsxfs_file_system_block_header_free(
				 &file_system_block_header,
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
			 "file_system_block_header",
			 file_system_block_header );

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
	if( file_system_block_header != NULL )
	{
		libfsxfs_file_system_block_header_free(
		 &file_system_block_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsxfs_file_system_block_header_free function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_file_system_block_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsxfs_file_system_block_header_free(
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

/* Tests the libfsxfs_file_system_block_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_file_system_block_header_read_data(
     void )
{
	libcerror_error_t *error                                      = NULL;
	libfsxfs_file_system_block_header_t *file_system_block_header = NULL;
	libfsxfs_io_handle_t *io_handle                               = NULL;
	int result                                                    = 0;

	/* Initialize test
	 */
	result = libfsxfs_io_handle_initialize(
	          &io_handle,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format_version = 5;

	result = libfsxfs_file_system_block_header_initialize(
	          &file_system_block_header,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "file_system_block_header",
	 file_system_block_header );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsxfs_file_system_block_header_read_data(
	          file_system_block_header,
	          io_handle,
	          fsxfs_test_file_system_block_header_data1,
	          56,
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
	result = libfsxfs_file_system_block_header_read_data(
	          NULL,
	          io_handle,
	          fsxfs_test_file_system_block_header_data1,
	          56,
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

	result = libfsxfs_file_system_block_header_read_data(
	          file_system_block_header,
	          NULL,
	          fsxfs_test_file_system_block_header_data1,
	          56,
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

	result = libfsxfs_file_system_block_header_read_data(
	          file_system_block_header,
	          io_handle,
	          NULL,
	          56,
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

	result = libfsxfs_file_system_block_header_read_data(
	          file_system_block_header,
	          io_handle,
	          fsxfs_test_file_system_block_header_data1,
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

	result = libfsxfs_file_system_block_header_read_data(
	          file_system_block_header,
	          io_handle,
	          fsxfs_test_file_system_block_header_data1,
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
	result = libfsxfs_file_system_block_header_free(
	          &file_system_block_header,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "file_system_block_header",
	 file_system_block_header );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_io_handle_free(
	          &io_handle,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( file_system_block_header != NULL )
	{
		libfsxfs_file_system_block_header_free(
		 &file_system_block_header,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfsxfs_io_handle_free(
		 &io_handle,
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
	 "libfsxfs_file_system_block_header_initialize",
	 fsxfs_test_file_system_block_header_initialize );

	FSXFS_TEST_RUN(
	 "libfsxfs_file_system_block_header_free",
	 fsxfs_test_file_system_block_header_free );

	FSXFS_TEST_RUN(
	 "libfsxfs_file_system_block_header_read_data",
	 fsxfs_test_file_system_block_header_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */
}

