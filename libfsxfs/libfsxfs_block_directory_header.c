/*
 * Block directory header functions
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

#include "libfsxfs_block_directory_header.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_block_directory.h"

/* Creates a block_directory_header
 * Make sure the value block_directory_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_header_initialize(
     libfsxfs_block_directory_header_t **block_directory_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_header_initialize";

	if( block_directory_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory header.",
		 function );

		return( -1 );
	}
	if( *block_directory_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block directory header value already set.",
		 function );

		return( -1 );
	}
	*block_directory_header = memory_allocate_structure(
	                           libfsxfs_block_directory_header_t );

	if( *block_directory_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block directory header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_directory_header,
	     0,
	     sizeof( libfsxfs_block_directory_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block directory header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *block_directory_header != NULL )
	{
		memory_free(
		 *block_directory_header );

		*block_directory_header = NULL;
	}
	return( -1 );
}

/* Frees a block_directory_header
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_header_free(
     libfsxfs_block_directory_header_t **block_directory_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_header_free";

	if( block_directory_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory header.",
		 function );

		return( -1 );
	}
	if( *block_directory_header != NULL )
	{
		memory_free(
		 *block_directory_header );

		*block_directory_header = NULL;
	}
	return( 1 );
}

/* Reads the block_directory_header data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_header_read_data(
     libfsxfs_block_directory_header_t *block_directory_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsxfs_block_directory_header_read_data";
	size_t header_data_size = 0;
	uint8_t format_version  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t data_offset      = 0;
	uint16_t value_16bit    = 0;
	int free_region_index   = 0;
#endif

	if( block_directory_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory header.",
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
	if( ( data_size < 4 )
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
	header_data_size = data_size;

	if( memory_compare(
	     data,
	     "XD2B",
	     4 ) == 0 )
	{
		format_version   = 2;
		header_data_size = sizeof( fsxfs_block_directory_header_v2_t );

		block_directory_header->has_footer = 1;
	}
	else if( memory_compare(
	          data,
	          "XD2D",
	          4 ) == 0 )
	{
		format_version   = 2;
		header_data_size = sizeof( fsxfs_block_directory_header_v2_t );
	}
	else if( memory_compare(
	          data,
	          "XDB3",
	          4 ) == 0 )
	{
		format_version   = 3;
		header_data_size = sizeof( fsxfs_block_directory_header_v3_t );

		block_directory_header->has_footer = 1;
	}
	else if( memory_compare(
	          data,
	          "XDD3",
	          4 ) == 0 )
	{
		format_version   = 3;
		header_data_size = sizeof( fsxfs_block_directory_header_v3_t );
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
		 "%s: block directory header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( format_version != 2 )
	 && ( format_version != 3 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported signature.",
		 function );

		return( -1 );
	}
	block_directory_header->format_version = format_version;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsxfs_block_directory_header_v2_t *) data )->signature[ 0 ],
		 ( (fsxfs_block_directory_header_v2_t *) data )->signature[ 1 ],
		 ( (fsxfs_block_directory_header_v2_t *) data )->signature[ 2 ],
		 ( (fsxfs_block_directory_header_v2_t *) data )->signature[ 3 ] );

		data_offset = 4;

		for( free_region_index = 0;
		     free_region_index < 3;
		     free_region_index++ )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( data[ data_offset ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: free region: %d offset\t: %" PRIu16 "\n",
			 function,
			 free_region_index,
			 value_16bit );

			data_offset += 2;

			byte_stream_copy_to_uint16_big_endian(
			 &( data[ data_offset ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: free region: %d size\t\t: %" PRIu16 "\n",
			 function,
			 free_region_index,
			 value_16bit );

			data_offset += 2;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

