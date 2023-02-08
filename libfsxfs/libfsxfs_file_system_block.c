/*
 * File system block functions
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

#include "libfsxfs_file_system_block.h"
#include "libfsxfs_file_system_block_header.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_file_system_block.h"

/* Creates a file system block
 * Make sure the value file_system_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_block_initialize(
     libfsxfs_file_system_block_t **file_system_block,
     size_t block_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_block_initialize";

	if( file_system_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system block.",
		 function );

		return( -1 );
	}
	if( *file_system_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system block value already set.",
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
	*file_system_block = memory_allocate_structure(
	                      libfsxfs_file_system_block_t );

	if( *file_system_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system_block,
	     0,
	     sizeof( libfsxfs_file_system_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system block.",
		 function );

		memory_free(
		 *file_system_block );

		*file_system_block = NULL;

		return( -1 );
	}
	( *file_system_block )->data = (uint8_t *) memory_allocate(
	                                            sizeof( uint8_t ) * block_size );

	if( ( *file_system_block )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system block data.",
		 function );

		goto on_error;
	}
	( *file_system_block )->data_size = block_size;

	return( 1 );

on_error:
	if( *file_system_block != NULL )
	{
		memory_free(
		 *file_system_block );

		*file_system_block = NULL;
	}
	return( -1 );
}

/* Frees a file system block
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_block_free(
     libfsxfs_file_system_block_t **file_system_block,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_block_free";
	int result            = 1;

	if( file_system_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system block.",
		 function );

		return( -1 );
	}
	if( *file_system_block != NULL )
	{
		if( ( *file_system_block )->header != NULL )
		{
			if( libfsxfs_file_system_block_header_free(
			     &( ( *file_system_block )->header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file system block header.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 ( *file_system_block )->data );

		memory_free(
		 *file_system_block );

		*file_system_block = NULL;
	}
	return( result );
}

/* Reads the file system block data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_block_read_data(
     libfsxfs_file_system_block_t *file_system_block,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsxfs_file_system_block_read_data";
	size_t header_data_size = 0;

	if( file_system_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system block.",
		 function );

		return( -1 );
	}
	if( file_system_block->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system block - header value already set.",
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
		header_data_size = sizeof( fsxfs_file_system_block_header_v3_t );
	}
	else
	{
		header_data_size = sizeof( fsxfs_file_system_block_header_v2_t );
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
	if( libfsxfs_file_system_block_header_initialize(
	     &( file_system_block->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file system block header.",
		 function );

		goto on_error;
	}
	if( libfsxfs_file_system_block_header_read_data(
	     file_system_block->header,
	     io_handle,
	     data,
	     header_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file system block header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system_block->header != NULL )
	{
		libfsxfs_file_system_block_header_free(
		 &( file_system_block->header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the file system block from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_block_read_file_io_handle(
     libfsxfs_file_system_block_t *file_system_block,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_block_read_file_io_handle";
	ssize_t read_count    = 0;

	if( file_system_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system block.",
		 function );

		return( -1 );
	}
	if( file_system_block->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system block - header value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file system block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              file_system_block->data,
	              file_system_block->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) file_system_block->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file system block data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsxfs_file_system_block_read_data(
	     file_system_block,
	     io_handle,
	     file_system_block->data,
	     file_system_block->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file system block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

