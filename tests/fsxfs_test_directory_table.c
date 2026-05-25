/*
 * Library directory_table type test program
 *
 * Copyright (C) 2020-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include "fsxfs_test_libcdata.h"
#include "fsxfs_test_libcerror.h"
#include "fsxfs_test_libfsxfs.h"
#include "fsxfs_test_macros.h"
#include "fsxfs_test_memory.h"
#include "fsxfs_test_unused.h"

#include "../libfsxfs/libfsxfs_directory_entry.h"
#include "../libfsxfs/libfsxfs_directory_table.h"
#include "../libfsxfs/libfsxfs_io_handle.h"

/* Version 5 directory table data
 */
uint8_t fsxfs_test_directory_table_data1[ 212 ] = {
	0x0a, 0x00, 0x00, 0x00, 0x60, 0x80, 0x09, 0x00, 0x60, 0x65, 0x6d, 0x70, 0x74, 0x79, 0x66, 0x69,
	0x6c, 0x65, 0x01, 0x00, 0x00, 0x60, 0x83, 0x08, 0x00, 0x78, 0x74, 0x65, 0x73, 0x74, 0x64, 0x69,
	0x72, 0x31, 0x02, 0x00, 0x00, 0x60, 0x84, 0x0e, 0x00, 0x90, 0x66, 0x69, 0x6c, 0x65, 0x5f, 0x68,
	0x61, 0x72, 0x64, 0x6c, 0x69, 0x6e, 0x6b, 0x31, 0x01, 0x00, 0x00, 0x60, 0x85, 0x12, 0x00, 0xb0,
	0x66, 0x69, 0x6c, 0x65, 0x5f, 0x73, 0x79, 0x6d, 0x62, 0x6f, 0x6c, 0x69, 0x63, 0x6c, 0x69, 0x6e,
	0x6b, 0x31, 0x07, 0x00, 0x00, 0x60, 0x87, 0x17, 0x00, 0xd0, 0x64, 0x69, 0x72, 0x65, 0x63, 0x74,
	0x6f, 0x72, 0x79, 0x5f, 0x73, 0x79, 0x6d, 0x62, 0x6f, 0x6c, 0x69, 0x63, 0x6c, 0x69, 0x6e, 0x6b,
	0x31, 0x07, 0x00, 0x00, 0x60, 0x88, 0x0e, 0x00, 0xf8, 0x6e, 0x66, 0x63, 0x5f, 0x74, 0xc3, 0xa9,
	0x73, 0x74, 0x66, 0x69, 0x6c, 0xc3, 0xa8, 0x01, 0x00, 0x00, 0x60, 0x89, 0x10, 0x01, 0x18, 0x6e,
	0x66, 0x64, 0x5f, 0x74, 0x65, 0xcc, 0x81, 0x73, 0x74, 0x66, 0x69, 0x6c, 0x65, 0xcc, 0x80, 0x01,
	0x00, 0x00, 0x60, 0x8a, 0x06, 0x01, 0x38, 0x6e, 0x66, 0x64, 0x5f, 0xc2, 0xbe, 0x01, 0x00, 0x00,
	0x60, 0x8b, 0x0a, 0x01, 0x50, 0x6e, 0x66, 0x6b, 0x64, 0x5f, 0x33, 0xe2, 0x81, 0x84, 0x34, 0x01,
	0x00, 0x00, 0x60, 0x8c, 0x08, 0x01, 0x68, 0x74, 0x65, 0x73, 0x74, 0x64, 0x69, 0x72, 0x32, 0x02,
	0x00, 0x00, 0x60, 0x96 };

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

/* Tests the libfsxfs_directory_table_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_directory_table_initialize(
     void )
{
	libcerror_error_t *error                    = NULL;
	libfsxfs_directory_table_t *directory_table = NULL;
	int result                                  = 0;

#if defined( HAVE_FSXFS_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 1;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Test regular cases
	 */
	result = libfsxfs_directory_table_initialize(
	          &directory_table,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_table",
	 directory_table );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_directory_table_free(
	          &directory_table,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "directory_table",
	 directory_table );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_directory_table_initialize(
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

	directory_table = (libfsxfs_directory_table_t *) 0x12345678UL;

	result = libfsxfs_directory_table_initialize(
	          &directory_table,
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

	directory_table = NULL;

#if defined( HAVE_FSXFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsxfs_directory_table_initialize with malloc failing
		 */
		fsxfs_test_malloc_attempts_before_fail = test_number;

		result = libfsxfs_directory_table_initialize(
		          &directory_table,
		          &error );

		if( fsxfs_test_malloc_attempts_before_fail != -1 )
		{
			fsxfs_test_malloc_attempts_before_fail = -1;

			if( directory_table != NULL )
			{
				libfsxfs_directory_table_free(
				 &directory_table,
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
			 "directory_table",
			 directory_table );

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
		/* Test libfsxfs_directory_table_initialize with memset failing
		 */
		fsxfs_test_memset_attempts_before_fail = test_number;

		result = libfsxfs_directory_table_initialize(
		          &directory_table,
		          &error );

		if( fsxfs_test_memset_attempts_before_fail != -1 )
		{
			fsxfs_test_memset_attempts_before_fail = -1;

			if( directory_table != NULL )
			{
				libfsxfs_directory_table_free(
				 &directory_table,
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
			 "directory_table",
			 directory_table );

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
	if( directory_table != NULL )
	{
		libfsxfs_directory_table_free(
		 &directory_table,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsxfs_directory_table_free function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_directory_table_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsxfs_directory_table_free(
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

/* Tests the libfsxfs_directory_table_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_directory_table_read_data(
     void )
{
	libcdata_array_t *entries_array             = NULL;
	libcerror_error_t *error                    = NULL;
	libfsxfs_directory_table_t *directory_table = NULL;
	libfsxfs_io_handle_t *io_handle             = NULL;
	int result                                  = 0;

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
	io_handle->block_size     = 4096;
	io_handle->inode_size     = 512;
	io_handle->feature_flags  = 0x2000;

	result = libfsxfs_directory_table_initialize(
	          &directory_table,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "directory_table",
	 directory_table );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcdata_array_initialize(
	          &entries_array,
	          0,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "entries_array",
	 entries_array );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsxfs_directory_table_read_data(
	          directory_table,
	          io_handle,
	          fsxfs_test_directory_table_data1,
	          212,
	          entries_array,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libcdata_array_free(
	          &entries_array,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "entries_array",
	 entries_array );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libcdata_array_initialize(
	          &entries_array,
	          0,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "entries_array",
	 entries_array );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_directory_table_read_data(
	          NULL,
	          io_handle,
	          fsxfs_test_directory_table_data1,
	          212,
	          entries_array,
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

	result = libfsxfs_directory_table_read_data(
	          directory_table,
	          NULL,
	          fsxfs_test_directory_table_data1,
	          212,
	          entries_array,
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

	result = libfsxfs_directory_table_read_data(
	          directory_table,
	          io_handle,
	          NULL,
	          6,
	          entries_array,
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

	result = libfsxfs_directory_table_read_data(
	          directory_table,
	          io_handle,
	          fsxfs_test_directory_table_data1,
	          (size_t) SSIZE_MAX + 1,
	          entries_array,
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

	result = libfsxfs_directory_table_read_data(
	          directory_table,
	          io_handle,
	          fsxfs_test_directory_table_data1,
	          0,
	          entries_array,
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
	result = libcdata_array_free(
	          &entries_array,
	          (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "entries_array",
	 entries_array );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_directory_table_free(
	          &directory_table,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "directory_table",
	 directory_table );

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
	if( entries_array != NULL )
	{
		libcdata_array_free(
		 &entries_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
		 NULL );
	}
	if( directory_table != NULL )
	{
		libfsxfs_directory_table_free(
		 &directory_table,
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
	 "libfsxfs_directory_table_initialize",
	 fsxfs_test_directory_table_initialize );

	FSXFS_TEST_RUN(
	 "libfsxfs_directory_table_free",
	 fsxfs_test_directory_table_free );

	FSXFS_TEST_RUN(
	 "libfsxfs_directory_table_read_data",
	 fsxfs_test_directory_table_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */
}

