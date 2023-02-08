/*
 * B+ tree block functions
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
#include <memory.h>
#include <types.h>

#include "libfsxfs_btree_block.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_btree.h"

/* Creates a B+ tree block
 * Make sure the value btree_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_block_initialize(
     libfsxfs_btree_block_t **btree_block,
     size_t block_size,
     size_t block_number_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_btree_block_initialize";

	if( btree_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B+ tree block.",
		 function );

		return( -1 );
	}
	if( *btree_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B+ tree block value already set.",
		 function );

		return( -1 );
	}
	if( ( block_size == 0 )
	 || ( block_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
	*btree_block = memory_allocate_structure(
	                libfsxfs_btree_block_t );

	if( *btree_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B+ tree block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *btree_block,
	     0,
	     sizeof( libfsxfs_btree_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear B+ tree block.",
		 function );

		memory_free(
		 *btree_block );

		*btree_block = NULL;

		return( -1 );
	}
	( *btree_block )->data = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * block_size );

	if( ( *btree_block )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create B+ tree block data.",
		 function );

		goto on_error;
	}
	( *btree_block )->data_size              = block_size;
	( *btree_block )->block_number_data_size = block_number_data_size;

	return( 1 );

on_error:
	if( *btree_block != NULL )
	{
		memory_free(
		 *btree_block );

		*btree_block = NULL;
	}
	return( -1 );
}

/* Frees a B+ tree block
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_block_free(
     libfsxfs_btree_block_t **btree_block,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_btree_block_free";
	int result            = 1;

	if( btree_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B+ tree block.",
		 function );

		return( -1 );
	}
	if( *btree_block != NULL )
	{
		if( ( *btree_block )->header != NULL )
		{
			if( libfsxfs_btree_header_free(
			     &( ( *btree_block )->header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free B+ tree header.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 ( *btree_block )->data );

		memory_free(
		 *btree_block );

		*btree_block = NULL;
	}
	return( result );
}

/* Reads the B+ tree block data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_block_read_data(
     libfsxfs_btree_block_t *btree_block,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsxfs_btree_block_read_data";
	size_t header_data_size = 0;

	if( btree_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B+ tree block.",
		 function );

		return( -1 );
	}
	if( btree_block->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B+ tree block - header value already set.",
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
	if( io_handle->format_version == 5 )
	{
		if( btree_block->block_number_data_size == 8 )
		{
			header_data_size = sizeof( fsxfs_btree_header_v5_64bit_t );
		}
		else
		{
			header_data_size = sizeof( fsxfs_btree_header_v5_32bit_t );
		}
	}
	else
	{
		if( btree_block->block_number_data_size == 8 )
		{
			header_data_size = sizeof( fsxfs_btree_header_v1_64bit_t );
		}
		else
		{
			header_data_size = sizeof( fsxfs_btree_header_v1_32bit_t );
		}
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
	if( ( data_size < header_data_size )
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
	if( libfsxfs_btree_header_initialize(
	     &( btree_block->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B+ tree header.",
		 function );

		goto on_error;
	}
	if( libfsxfs_btree_header_read_data(
	     btree_block->header,
	     io_handle,
	     data,
	     header_data_size,
	     btree_block->block_number_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B+ tree header.",
		 function );

		goto on_error;
	}
	btree_block->records_data      = &( data[ header_data_size ] );
	btree_block->records_data_size = data_size - header_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B+ tree records data:\n",
		 function );
		libcnotify_print_data(
		 btree_block->records_data,
		 btree_block->records_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( btree_block->header != NULL )
	{
		libfsxfs_btree_header_free(
		 &( btree_block->header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the B+ tree block from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_block_read_file_io_handle(
     libfsxfs_btree_block_t *btree_block,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_btree_block_read_file_io_handle";
	ssize_t read_count    = 0;

	if( btree_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid B+ tree block.",
		 function );

		return( -1 );
	}
	if( btree_block->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid B+ tree block - header value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading B+ tree block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              btree_block->data,
	              btree_block->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) btree_block->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B+ tree block data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsxfs_btree_block_read_data(
	     btree_block,
	     io_handle,
	     btree_block->data,
	     btree_block->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read B+ tree block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

