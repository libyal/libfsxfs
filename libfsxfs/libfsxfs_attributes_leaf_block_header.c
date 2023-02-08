/*
 * Attributes leaf attributes leaf block header functions
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

#include "libfsxfs_debug.h"
#include "libfsxfs_attributes_leaf_block_header.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_attributes_block.h"

/* Creates a attributes leaf block header
 * Make sure the value attributes_leaf_block_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_leaf_block_header_initialize(
     libfsxfs_attributes_leaf_block_header_t **attributes_leaf_block_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_leaf_block_header_initialize";

	if( attributes_leaf_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes leaf block header.",
		 function );

		return( -1 );
	}
	if( *attributes_leaf_block_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes leaf block header value already set.",
		 function );

		return( -1 );
	}
	*attributes_leaf_block_header = memory_allocate_structure(
	                                 libfsxfs_attributes_leaf_block_header_t );

	if( *attributes_leaf_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attributes leaf block header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attributes_leaf_block_header,
	     0,
	     sizeof( libfsxfs_attributes_leaf_block_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attributes leaf block header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attributes_leaf_block_header != NULL )
	{
		memory_free(
		 *attributes_leaf_block_header );

		*attributes_leaf_block_header = NULL;
	}
	return( -1 );
}

/* Frees a attributes leaf block header
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_leaf_block_header_free(
     libfsxfs_attributes_leaf_block_header_t **attributes_leaf_block_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_leaf_block_header_free";

	if( attributes_leaf_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes leaf block header.",
		 function );

		return( -1 );
	}
	if( *attributes_leaf_block_header != NULL )
	{
		memory_free(
		 *attributes_leaf_block_header );

		*attributes_leaf_block_header = NULL;
	}
	return( 1 );
}

/* Reads the attributes leaf block header data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_leaf_block_header_read_data(
     libfsxfs_attributes_leaf_block_header_t *attributes_leaf_block_header,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsxfs_attributes_leaf_block_header_read_data";
	size_t header_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t data_offset      = 0;
	uint32_t value_32bit    = 0;
	uint16_t value_16bit    = 0;
	int free_region_index   = 0;
#endif

	if( attributes_leaf_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes leaf block header.",
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
		header_data_size = sizeof( fsxfs_attributes_leaf_block_header_v3_t );
	}
	else
	{
		header_data_size = sizeof( fsxfs_attributes_leaf_block_header_v2_t );
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attributes leaf block header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_attributes_leaf_block_header_v2_t *) data )->number_of_entries,
	 attributes_leaf_block_header->number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t: %" PRIu16 "\n",
		 function,
		 attributes_leaf_block_header->number_of_entries );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_attributes_leaf_block_header_v2_t *) data )->used_data_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: used data size\t\t: %" PRIu16 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_attributes_leaf_block_header_v2_t *) data )->used_data_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: used data offset\t: %" PRIu16 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: block compaction flag\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fsxfs_attributes_leaf_block_header_v2_t *) data )->block_compaction_flag );

		libcnotify_printf(
		 "%s: unknown2\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fsxfs_attributes_leaf_block_header_v2_t *) data )->unknown2 );

		data_offset = 8;

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
			 "%s: free region: %d size\t: %" PRIu16 "\n",
			 function,
			 free_region_index,
			 value_16bit );

			data_offset += 2;
		}
		if( io_handle->format_version == 5 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_leaf_block_header_v3_t *) data )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

