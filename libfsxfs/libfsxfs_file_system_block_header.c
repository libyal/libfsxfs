/*
 * File system file system block header functions
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
#include "libfsxfs_file_system_block_header.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_file_system_block.h"

/* Creates a file system block header
 * Make sure the value file_system_block_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_block_header_initialize(
     libfsxfs_file_system_block_header_t **file_system_block_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_block_header_initialize";

	if( file_system_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system block header.",
		 function );

		return( -1 );
	}
	if( *file_system_block_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system block header value already set.",
		 function );

		return( -1 );
	}
	*file_system_block_header = memory_allocate_structure(
	                             libfsxfs_file_system_block_header_t );

	if( *file_system_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system block header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system_block_header,
	     0,
	     sizeof( libfsxfs_file_system_block_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system block header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_system_block_header != NULL )
	{
		memory_free(
		 *file_system_block_header );

		*file_system_block_header = NULL;
	}
	return( -1 );
}

/* Frees a file system block header
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_block_header_free(
     libfsxfs_file_system_block_header_t **file_system_block_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_block_header_free";

	if( file_system_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system block header.",
		 function );

		return( -1 );
	}
	if( *file_system_block_header != NULL )
	{
		memory_free(
		 *file_system_block_header );

		*file_system_block_header = NULL;
	}
	return( 1 );
}

/* Reads the file system block header data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_block_header_read_data(
     libfsxfs_file_system_block_header_t *file_system_block_header,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsxfs_file_system_block_header_read_data";
	size_t header_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit    = 0;
	uint32_t value_32bit    = 0;
	uint16_t value_16bit    = 0;
#endif

	if( file_system_block_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system block header.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file system block header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 header_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_file_system_block_header_v2_t *) data )->next_block_number,
	 file_system_block_header->next_block_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_file_system_block_header_v2_t *) data )->previous_block_number,
	 file_system_block_header->previous_block_number );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_file_system_block_header_v2_t *) data )->signature,
	 file_system_block_header->signature );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: next block number\t\t: %" PRIu32 "\n",
		 function,
		 file_system_block_header->next_block_number );

		libcnotify_printf(
		 "%s: previous block number\t: %" PRIu32 "\n",
		 function,
		 file_system_block_header->previous_block_number );

		libcnotify_printf(
		 "%s: signature\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 file_system_block_header->signature );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_file_system_block_header_v2_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		if( io_handle->format_version == 5 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_file_system_block_header_v3_t *) data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_file_system_block_header_v3_t *) data )->block_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: block number\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_file_system_block_header_v3_t *) data )->log_sequence_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: log sequence number\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			if( libfsxfs_debug_print_guid_value(
			     function,
			     "block type identifier\t",
			     ( (fsxfs_file_system_block_header_v3_t *) data )->block_type_identifier,
			     16,
			     LIBFGUID_ENDIAN_BIG,
			     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print GUID value.",
				 function );

				return( -1 );
			}
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

