/*
 * B+ tree header functions
 *
 * Copyright (C) 2020, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsxfs_btree_header.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_btree.h"

/* Creates a btree_header
 * Make sure the value btree_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_header_initialize(
     libfsxfs_btree_header_t **btree_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_btree_header_initialize";

	if( btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B+ tree header.",
		 function );

		return( -1 );
	}
	if( *btree_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B+ tree header value already set.",
		 function );

		return( -1 );
	}
	*btree_header = memory_allocate_structure(
	                 libfsxfs_btree_header_t );

	if( *btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B+ tree header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_header,
	     0,
	     sizeof( libfsxfs_btree_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B+ tree header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *btree_header != NULL )
	{
		memory_free(
		 *btree_header );

		*btree_header = NULL;
	}
	return( -1 );
}

/* Frees a btree_header
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_header_free(
     libfsxfs_btree_header_t **btree_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_btree_header_free";

	if( btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B+ tree header.",
		 function );

		return( -1 );
	}
	if( *btree_header != NULL )
	{
		memory_free(
		 *btree_header );

		*btree_header = NULL;
	}
	return( 1 );
}

/* Reads the btree_header data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_header_read_data(
     libfsxfs_btree_header_t *btree_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsxfs_btree_header_read_data";
	uint32_t format_version = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit    = 0;
	uint16_t value_16bit    = 0;
#endif

	if( btree_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B+ tree header.",
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
	if( ( data_size < sizeof( fsxfs_btree_header_t ) )
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B+ tree header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsxfs_btree_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_copy(
	     btree_header->signature,
	     ( (fsxfs_btree_header_t *) data )->signature,
	     4 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_btree_header_t *) data )->level,
	 btree_header->level );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_btree_header_t *) data )->number_of_records,
	 btree_header->number_of_records );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsxfs_btree_header_t *) data )->signature[ 0 ],
		 ( (fsxfs_btree_header_t *) data )->signature[ 1 ],
		 ( (fsxfs_btree_header_t *) data )->signature[ 2 ],
		 ( (fsxfs_btree_header_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: level\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_header->level );

		libcnotify_printf(
		 "%s: number of records\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_header->number_of_records );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_btree_header_t *) data )->previous_btree_block_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: previous B+ tree block number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_btree_header_t *) data )->next_btree_block_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: next B+ tree block number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

