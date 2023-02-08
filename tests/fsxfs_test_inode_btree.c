/*
 * Library inode_btree type test program
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

#include "../libfsxfs/libfsxfs_inode_btree.h"

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

/* Tests the libfsxfs_inode_btree_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_inode_btree_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libfsxfs_inode_btree_t *inode_btree = NULL;
	int result                          = 0;

#if defined( HAVE_FSXFS_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
#endif

	/* Test regular cases
	 */
	result = libfsxfs_inode_btree_initialize(
	          &inode_btree,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NOT_NULL(
	 "inode_btree",
	 inode_btree );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsxfs_inode_btree_free(
	          &inode_btree,
	          &error );

	FSXFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "inode_btree",
	 inode_btree );

	FSXFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsxfs_inode_btree_initialize(
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

	inode_btree = (libfsxfs_inode_btree_t *) 0x12345678UL;

	result = libfsxfs_inode_btree_initialize(
	          &inode_btree,
	          &error );

	inode_btree = NULL;

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
		/* Test libfsxfs_inode_btree_initialize with malloc failing
		 */
		fsxfs_test_malloc_attempts_before_fail = test_number;

		result = libfsxfs_inode_btree_initialize(
		          &inode_btree,
		          &error );

		if( fsxfs_test_malloc_attempts_before_fail != -1 )
		{
			fsxfs_test_malloc_attempts_before_fail = -1;

			if( inode_btree != NULL )
			{
				libfsxfs_inode_btree_free(
				 &inode_btree,
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
			 "inode_btree",
			 inode_btree );

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
		/* Test libfsxfs_inode_btree_initialize with memset failing
		 */
		fsxfs_test_memset_attempts_before_fail = test_number;

		result = libfsxfs_inode_btree_initialize(
		          &inode_btree,
		          &error );

		if( fsxfs_test_memset_attempts_before_fail != -1 )
		{
			fsxfs_test_memset_attempts_before_fail = -1;

			if( inode_btree != NULL )
			{
				libfsxfs_inode_btree_free(
				 &inode_btree,
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
			 "inode_btree",
			 inode_btree );

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
	if( inode_btree != NULL )
	{
		libfsxfs_inode_btree_free(
		 &inode_btree,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsxfs_inode_btree_free function
 * Returns 1 if successful or 0 if not
 */
int fsxfs_test_inode_btree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsxfs_inode_btree_free(
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
	 "libfsxfs_inode_btree_initialize",
	 fsxfs_test_inode_btree_initialize );

	FSXFS_TEST_RUN(
	 "libfsxfs_inode_btree_free",
	 fsxfs_test_inode_btree_free );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFSXFS_DLL_IMPORT ) */
}

