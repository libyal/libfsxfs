/*
 * Inode B+ tree record functions
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

#include "libfsxfs_inode_btree_record.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_btree.h"

/* Creates a inode_btree_record
 * Make sure the value inode_btree_record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_record_initialize(
     libfsxfs_inode_btree_record_t **inode_btree_record,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_btree_record_initialize";

	if( inode_btree_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree record.",
		 function );

		return( -1 );
	}
	if( *inode_btree_record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode B+ tree record value already set.",
		 function );

		return( -1 );
	}
	*inode_btree_record = memory_allocate_structure(
	                       libfsxfs_inode_btree_record_t );

	if( *inode_btree_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode B+ tree record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *inode_btree_record,
	     0,
	     sizeof( libfsxfs_inode_btree_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inode B+ tree record.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *inode_btree_record != NULL )
	{
		memory_free(
		 *inode_btree_record );

		*inode_btree_record = NULL;
	}
	return( -1 );
}

/* Frees a inode_btree_record
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_record_free(
     libfsxfs_inode_btree_record_t **inode_btree_record,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_btree_record_free";

	if( inode_btree_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree record.",
		 function );

		return( -1 );
	}
	if( *inode_btree_record != NULL )
	{
		memory_free(
		 *inode_btree_record );

		*inode_btree_record = NULL;
	}
	return( 1 );
}

/* Reads the inode_btree_record data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_record_read_data(
     libfsxfs_inode_btree_record_t *inode_btree_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_btree_record_read_data";

	if( inode_btree_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree record.",
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
	if( ( data_size < sizeof( fsxfs_inode_btree_record_t ) )
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
		 "%s: inode B+ tree record data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsxfs_inode_btree_record_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_btree_record_t *) data )->inode_number,
	 inode_btree_record->inode_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_btree_record_t *) data )->number_of_unused_inodes,
	 inode_btree_record->number_of_unused_inodes );

	byte_stream_copy_to_uint64_big_endian(
	 ( (fsxfs_inode_btree_record_t *) data )->chunk_allocation_bitmap,
	 inode_btree_record->chunk_allocation_bitmap );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: inode number\t\t\t: %" PRIu32 "\n",
		 function,
		 inode_btree_record->inode_number );

		libcnotify_printf(
		 "%s: number of unused inodes\t\t: %" PRIu32 "\n",
		 function,
		 inode_btree_record->number_of_unused_inodes );

		libcnotify_printf(
		 "%s: chunk allocation bitmap\t\t: 0x%08" PRIx64 "\n",
		 function,
		 inode_btree_record->chunk_allocation_bitmap );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

