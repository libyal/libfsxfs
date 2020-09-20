/*
 * Inode B+ tree functions
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

#include "libfsxfs_btree_block.h"
#include "libfsxfs_inode_btree.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

/* Creates an inode B+ tree
 * Make sure the value inode_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_initialize(
     libfsxfs_inode_btree_t **inode_btree,
     uint32_t root_block_number,
     uint32_t depth,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_btree_initialize";

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
		 function );

		return( -1 );
	}
	if( *inode_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode B+ tree value already set.",
		 function );

		return( -1 );
	}
	*inode_btree = memory_allocate_structure(
	                libfsxfs_inode_btree_t );

	if( *inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode B+ tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *inode_btree,
	     0,
	     sizeof( libfsxfs_inode_btree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inode B+ tree.",
		 function );

		goto on_error;
	}
	( *inode_btree )->root_block_number = root_block_number;
	( *inode_btree )->depth             = depth;

	return( 1 );

on_error:
	if( *inode_btree != NULL )
	{
		memory_free(
		 *inode_btree );

		*inode_btree = NULL;
	}
	return( -1 );
}

/* Frees an inode B+ tree
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_free(
     libfsxfs_inode_btree_t **inode_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_btree_free";

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
		 function );

		return( -1 );
	}
	if( *inode_btree != NULL )
	{
		memory_free(
		 *inode_btree );

		*inode_btree = NULL;
	}
	return( 1 );
}

/* Retrieves a specific inode from the inode B+ tree
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfsxfs_inode_btree_get_inode_by_number(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t inode_number,
     libcerror_error_t **error )
{
	libfsxfs_btree_block_t *btree_block = NULL;
	static char *function               = "libfsxfs_inode_btree_get_inode_by_number";
	off64_t btree_block_offset          = 0;
	int result                          = 0;

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libfsxfs_btree_block_initialize(
	     &btree_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B+ tree block.",
		 function );

		goto on_error;
	}
	btree_block_offset = inode_btree->root_block_number * io_handle->block_size;

	if( libfsxfs_btree_block_read_file_io_handle(
	     btree_block,
	     io_handle,
	     file_io_handle,
	     btree_block_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode B+ tree block: %" PRIu32 " at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 inode_btree->root_block_number,
		 btree_block_offset,
		 btree_block_offset );

		goto on_error;
	}
	if( memory_compare(
	     btree_block->header->signature,
	     "IAB3",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		goto on_error;
	}
/* TODO traverse records */
/* TODO traverse tree nodes */

	result = 1;

	if( libfsxfs_btree_block_free(
	     &btree_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B+ tree block.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( btree_block != NULL )
	{
		libfsxfs_btree_block_free(
		 &btree_block,
		 NULL );
	}
	return( -1 );
}

