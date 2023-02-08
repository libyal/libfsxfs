/*
 * B+ tree header functions
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

#include "libfsxfs_btree_header.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_btree.h"

/* Creates a B+tree header
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

/* Frees a B+tree header
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

/* Reads the B+tree header data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_btree_header_read_data(
     libfsxfs_btree_header_t *btree_header,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     size_t block_number_data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsxfs_btree_header_read_data";
	size_t header_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit    = 0;
	uint32_t value_32bit    = 0;
	int result              = 1;
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
	if( ( block_number_data_size != 4 )
	 && ( block_number_data_size != 8 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block number data size.",
		 function );

		return( -1 );
	}
	if( io_handle->format_version == 5 )
	{
		if( block_number_data_size == 8 )
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
		if( block_number_data_size == 8 )
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: B+ tree header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_copy(
	     btree_header->signature,
	     ( (fsxfs_btree_header_v1_32bit_t *) data )->signature,
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
	 ( (fsxfs_btree_header_v1_32bit_t *) data )->level,
	 btree_header->level );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_btree_header_v1_32bit_t *) data )->number_of_records,
	 btree_header->number_of_records );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsxfs_btree_header_v1_32bit_t *) data )->signature[ 0 ],
		 ( (fsxfs_btree_header_v1_32bit_t *) data )->signature[ 1 ],
		 ( (fsxfs_btree_header_v1_32bit_t *) data )->signature[ 2 ],
		 ( (fsxfs_btree_header_v1_32bit_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: level\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_header->level );

		libcnotify_printf(
		 "%s: number of records\t\t\t: %" PRIu16 "\n",
		 function,
		 btree_header->number_of_records );

		if( block_number_data_size == 4 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_btree_header_v1_32bit_t *) data )->previous_btree_block_number,
			 value_64bit );
		}
		else
		{
			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_btree_header_v1_64bit_t *) data )->previous_btree_block_number,
			 value_64bit );
		}
		libcnotify_printf(
		 "%s: previous B+ tree block number\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		if( block_number_data_size == 4 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_btree_header_v1_32bit_t *) data )->next_btree_block_number,
			 value_64bit );
		}
		else
		{
			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_btree_header_v1_64bit_t *) data )->next_btree_block_number,
			 value_64bit );
		}
		libcnotify_printf(
		 "%s: next B+ tree block number\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( io_handle->format_version == 5 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( block_number_data_size == 4 )
			{
				byte_stream_copy_to_uint64_big_endian(
				 ( (fsxfs_btree_header_v5_32bit_t *) data )->block_number,
				 value_64bit );
			}
			else
			{
				byte_stream_copy_to_uint64_big_endian(
				 ( (fsxfs_btree_header_v5_64bit_t *) data )->block_number,
				 value_64bit );
			}
			libcnotify_printf(
			 "%s: block number\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			if( block_number_data_size == 4 )
			{
				byte_stream_copy_to_uint64_big_endian(
				 ( (fsxfs_btree_header_v5_32bit_t *) data )->log_sequence_number,
				 value_64bit );
			}
			else
			{
				byte_stream_copy_to_uint64_big_endian(
				 ( (fsxfs_btree_header_v5_64bit_t *) data )->log_sequence_number,
				 value_64bit );
			}
			libcnotify_printf(
			 "%s: log sequence number\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			if( block_number_data_size == 4 )
			{
				result = libfsxfs_debug_print_guid_value(
				          function,
				          "block type identifier\t\t\t",
				          ( (fsxfs_btree_header_v5_32bit_t *) data )->block_type_identifier,
				          16,
				          LIBFGUID_ENDIAN_BIG,
				          LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
				          error );
			}
			else
			{
				result = libfsxfs_debug_print_guid_value(
				          function,
				          "block type identifier\t\t\t",
				          ( (fsxfs_btree_header_v5_64bit_t *) data )->block_type_identifier,
				          16,
				          LIBFGUID_ENDIAN_BIG,
				          LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
				          error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
			if( block_number_data_size == 4 )
			{
				byte_stream_copy_to_uint32_big_endian(
				 ( (fsxfs_btree_header_v5_32bit_t *) data )->owner_allocation_group,
				 value_32bit );
			}
			else
			{
				byte_stream_copy_to_uint32_big_endian(
				 ( (fsxfs_btree_header_v5_64bit_t *) data )->owner_allocation_group,
				 value_32bit );
			}
			libcnotify_printf(
			 "%s: owner allocation group\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			if( block_number_data_size == 4 )
			{
				byte_stream_copy_to_uint32_big_endian(
				 ( (fsxfs_btree_header_v5_32bit_t *) data )->checksum,
				 value_32bit );
			}
			else
			{
				byte_stream_copy_to_uint32_big_endian(
				 ( (fsxfs_btree_header_v5_64bit_t *) data )->checksum,
				 value_32bit );
			}
			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			if( block_number_data_size == 8 )
			{
				byte_stream_copy_to_uint32_big_endian(
				 ( (fsxfs_btree_header_v5_64bit_t *) data )->unknown1,
				 value_32bit );
				libcnotify_printf(
				 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 value_32bit );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

