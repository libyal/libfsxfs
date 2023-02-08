/*
 * Short-form directory table header functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsxfs_directory_table_header.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_btree.h"

/* Creates a directory_table_header
 * Make sure the value directory_table_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_table_header_initialize(
     libfsxfs_directory_table_header_t **directory_table_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_table_header_initialize";

	if( directory_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory table header.",
		 function );

		return( -1 );
	}
	if( *directory_table_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory table header value already set.",
		 function );

		return( -1 );
	}
	*directory_table_header = memory_allocate_structure(
	                           libfsxfs_directory_table_header_t );

	if( *directory_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory table header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_table_header,
	     0,
	     sizeof( libfsxfs_directory_table_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory table header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_table_header != NULL )
	{
		memory_free(
		 *directory_table_header );

		*directory_table_header = NULL;
	}
	return( -1 );
}

/* Frees a directory_table_header
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_table_header_free(
     libfsxfs_directory_table_header_t **directory_table_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_table_header_free";

	if( directory_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory table header.",
		 function );

		return( -1 );
	}
	if( *directory_table_header != NULL )
	{
		memory_free(
		 *directory_table_header );

		*directory_table_header = NULL;
	}
	return( 1 );
}

/* Reads the directory_table_header data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_table_header_read_data(
     libfsxfs_directory_table_header_t *directory_table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function           = "libfsxfs_directory_table_header_read_data";
	size_t header_data_size         = 0;
	uint8_t number_of_32bit_entries = 0;
	uint8_t number_of_64bit_entries = 0;

	if( directory_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory table header.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 2 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	number_of_32bit_entries = data[ 0 ];
	number_of_64bit_entries = data[ 1 ];

	if( ( number_of_32bit_entries != 0 )
	 && ( number_of_64bit_entries != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid number of 32-bit and 64-bit entries.",
		 function );

		return( -1 );
	}
	if( number_of_64bit_entries == 0 )
	{
		header_data_size = 6;
	}
	else
	{
		header_data_size = 10;
	}
	if( data_size < header_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: directory table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( number_of_64bit_entries == 0 )
	{
		directory_table_header->number_of_entries = number_of_32bit_entries;

		directory_table_header->inode_number_data_size = 4;

		byte_stream_copy_to_uint32_big_endian(
		 &( data[ 2 ] ),
		 directory_table_header->parent_inode_number );
	}
	else
	{
		directory_table_header->number_of_entries = number_of_64bit_entries;

		directory_table_header->inode_number_data_size = 8;

		byte_stream_copy_to_uint64_big_endian(
		 &( data[ 2 ] ),
		 directory_table_header->parent_inode_number );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of 32-bit entries\t: %" PRIu8 "\n",
		 function,
		 number_of_32bit_entries );

		libcnotify_printf(
		 "%s: number of 64-bit entries\t: %" PRIu8 "\n",
		 function,
		 number_of_64bit_entries );

		libcnotify_printf(
		 "%s: parent inode number\t\t: %" PRIu64 "\n",
		 function,
		 directory_table_header->parent_inode_number );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

