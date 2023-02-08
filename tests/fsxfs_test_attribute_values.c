/*
 * Library attribute_values type test program
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
#include "fsxfs_test_libuna.h"
#include "fsxfs_test_macros.h"
#include "fsxfs_test_memory.h"
#include "fsxfs_test_unused.h"

#include "../libfsxfs/libfsxfs_attribute_values.h"

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

/* Tests the libfsxfs_attribute_values_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_attribute_values_initialize(
     void )
{
	libcerror_error_t *error                      = NULL;
	libfsxfs_attribute_values_t *attribute_values = NULL;
	int result                                    = 0;

#if defined( HAVE_FSXFS_TEST_MEMORY )
	int number_of_malloc_fail_tests               = 1;
	int number_of_memset_fail_tests               = 1;
	int test_number                               = 0;
#endif

	/* Test regular cases
	 */
	result = libfsxfs_attribute_values_initialize(
	          &attribute_values,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_values",
	 attribute_values );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_attribute_values_free(
	          &attribute_values,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "attribute_values",
	 attribute_values );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_attribute_values_initialize(
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

	attribute_values = (libfsxfs_attribute_values_t *) 0x12345678UL;

	result = libfsxfs_attribute_values_initialize(
	          &attribute_values,
	          &error );

	attribute_values = NULL;

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSXFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsxfs_attribute_values_initialize with malloc failing
		 */
		fsxfs_test_malloc_attempts_before_fail = test_number;

		result = libfsxfs_attribute_values_initialize(
		          &attribute_values,
		          &error );

		if( fsxfs_test_malloc_attempts_before_fail != -1 )
		{
			fsxfs_test_malloc_attempts_before_fail = -1;

			if( attribute_values != NULL )
			{
				libfsxfs_attribute_values_free(
				 &attribute_values,
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
			 "attribute_values",
			 attribute_values );

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
		/* Test libfsxfs_attribute_values_initialize with memset failing
		 */
		fsxfs_test_memset_attempts_before_fail = test_number;

		result = libfsxfs_attribute_values_initialize(
		          &attribute_values,
		          &error );

		if( fsxfs_test_memset_attempts_before_fail != -1 )
		{
			fsxfs_test_memset_attempts_before_fail = -1;

			if( attribute_values != NULL )
			{
				libfsxfs_attribute_values_free(
				 &attribute_values,
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
			 "attribute_values",
			 attribute_values );

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
	if( attribute_values != NULL )
	{
		libfsxfs_attribute_values_free(
		 &attribute_values,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsxfs_attribute_values_free function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_attribute_values_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsxfs_attribute_values_free(
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

/* Tests the libfsxfs_attribute_values_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_attribute_values_compare_name_with_utf8_string(
     libfsxfs_attribute_values_t *attribute_values )
{
	uint8_t utf8_string[ 14 ] = { 'u', 's', 'e', 'r', '.', 'm', 'y', 'x', 'a', 't', 't', 'r', '1', 0 };
	libcerror_error_t *error  = NULL;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfsxfs_attribute_values_compare_name_with_utf8_string(
	          attribute_values,
	          utf8_string,
	          13,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_attribute_values_compare_name_with_utf8_string(
	          NULL,
	          utf8_string,
	          13,
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

	result = libfsxfs_attribute_values_compare_name_with_utf8_string(
		  attribute_values,
		  NULL,
	          13,
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

/* Tests the libfsxfs_attribute_values_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_attribute_values_get_utf8_name_size(
     libfsxfs_attribute_values_t *attribute_values )
{
	libcerror_error_t *error = NULL;
	size_t utf8_name_size    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsxfs_attribute_values_get_utf8_name_size(
	          attribute_values,
	          &utf8_name_size,
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
	result = libfsxfs_attribute_values_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
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

	result = libfsxfs_attribute_values_get_utf8_name_size(
		  attribute_values,
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

/* Tests the libfsxfs_attribute_values_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_attribute_values_get_utf8_name(
     libfsxfs_attribute_values_t *attribute_values )
{
	uint8_t utf8_name[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsxfs_attribute_values_get_utf8_name(
	          attribute_values,
	          utf8_name,
	          256,
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
	result = libfsxfs_attribute_values_get_utf8_name(
	          NULL,
	          utf8_name,
	          256,
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

	result = libfsxfs_attribute_values_get_utf8_name(
	          attribute_values,
	          NULL,
	          256,
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

	result = libfsxfs_attribute_values_get_utf8_name(
	          attribute_values,
	          utf8_name,
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

	result = libfsxfs_attribute_values_get_utf8_name(
	          attribute_values,
	          utf8_name,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsxfs_attribute_values_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsapfs_test_attribute_values_compare_name_with_utf16_string(
     libfsxfs_attribute_values_t *attribute_values )
{
	uint16_t utf16_string[ 14 ] = { 'u', 's', 'e', 'r', '.', 'm', 'y', 'x', 'a', 't', 't', 'r', '1', 0 };
	libcerror_error_t *error    = NULL;
	int result                  = 0;

	/* Test regular cases
	 */
	result = libfsxfs_attribute_values_compare_name_with_utf16_string(
	          attribute_values,
	          utf16_string,
	          13,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBUNA_COMPARE_EQUAL );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_attribute_values_compare_name_with_utf16_string(
	          NULL,
	          utf16_string,
	          13,
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

	result = libfsxfs_attribute_values_compare_name_with_utf16_string(
		  attribute_values,
		  NULL,
	          13,
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

/* Tests the libfsxfs_attribute_values_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_attribute_values_get_utf16_name_size(
     libfsxfs_attribute_values_t *attribute_values )
{
	libcerror_error_t *error = NULL;
	size_t utf16_name_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsxfs_attribute_values_get_utf16_name_size(
	          attribute_values,
	          &utf16_name_size,
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
	result = libfsxfs_attribute_values_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
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

	result = libfsxfs_attribute_values_get_utf16_name_size(
		  attribute_values,
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

/* Tests the libfsxfs_attribute_values_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_attribute_values_get_utf16_name(
     libfsxfs_attribute_values_t *attribute_values )
{
	uint16_t utf16_name[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsxfs_attribute_values_get_utf16_name(
	          attribute_values,
	          utf16_name,
	          256,
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
	result = libfsxfs_attribute_values_get_utf16_name(
	          NULL,
	          utf16_name,
	          256,
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

	result = libfsxfs_attribute_values_get_utf16_name(
	          attribute_values,
	          NULL,
	          256,
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

	result = libfsxfs_attribute_values_get_utf16_name(
	          attribute_values,
	          utf16_name,
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

	result = libfsxfs_attribute_values_get_utf16_name(
	          attribute_values,
	          utf16_name,
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
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
#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )
	libcerror_error_t *error                      = NULL;
	libfsxfs_attribute_values_t *attribute_values = NULL;
	int result                                    = 0;
#endif

	FSXFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSXFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

	FSXFS_TEST_RUN(
	 "libfsxfs_attribute_values_initialize",
	 fsxfs_test_attribute_values_initialize );

	FSXFS_TEST_RUN(
	 "libfsxfs_attribute_values_free",
	 fsxfs_test_attribute_values_free );

	/* TODO add tests for libfsxfs_attribute_values_set_name */

	/* TODO add tests for libfsxfs_attribute_values_set_value_data */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize attribute_values for tests
	 */
	result = libfsxfs_attribute_values_initialize(
	          &attribute_values,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_values",
	 attribute_values );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_attribute_values_set_name(
	          attribute_values,
	          (uint8_t *) "myxattr1",
	          8,
	          0x00,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	FSXFS_TEST_RUN_WITH_ARGS(
	 "libfsxfs_attribute_values_compare_name_with_utf8_string",
	 fsapfs_test_attribute_values_compare_name_with_utf8_string,
	 attribute_values );

	FSXFS_TEST_RUN_WITH_ARGS(
	 "libfsxfs_attribute_values_get_utf8_name_size",
	 fsxfs_test_attribute_values_get_utf8_name_size,
	 attribute_values );

	FSXFS_TEST_RUN_WITH_ARGS(
	 "libfsxfs_attribute_values_get_utf8_name",
	 fsxfs_test_attribute_values_get_utf8_name,
	 attribute_values );

	FSXFS_TEST_RUN_WITH_ARGS(
	 "libfsxfs_attribute_values_compare_name_with_utf16_string",
	 fsapfs_test_attribute_values_compare_name_with_utf16_string,
	 attribute_values );

	FSXFS_TEST_RUN_WITH_ARGS(
	 "libfsxfs_attribute_values_get_utf16_name_size",
	 fsxfs_test_attribute_values_get_utf16_name_size,
	 attribute_values );

	FSXFS_TEST_RUN_WITH_ARGS(
	 "libfsxfs_attribute_values_get_utf16_name",
	 fsxfs_test_attribute_values_get_utf16_name,
	 attribute_values );

	/* Clean up
	 */
	result = libfsxfs_attribute_values_free(
	          &attribute_values,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "attribute_values",
	 attribute_values );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

on_error:
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( attribute_values != NULL )
	{
		libfsxfs_attribute_values_free(
		 &attribute_values,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */
}

