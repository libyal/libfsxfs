/*
 * Inode information functions
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
#include "libfsxfs_inode_information.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_inode_information.h"

/* Creates inode information
 * Make sure the value inode_information is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_information_initialize(
     libfsxfs_inode_information_t **inode_information,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_information_initialize";

	if( inode_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode information.",
		 function );

		return( -1 );
	}
	if( *inode_information != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode information value already set.",
		 function );

		return( -1 );
	}
	*inode_information = memory_allocate_structure(
	                      libfsxfs_inode_information_t );

	if( *inode_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode information.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *inode_information,
	     0,
	     sizeof( libfsxfs_inode_information_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inode information.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *inode_information != NULL )
	{
		memory_free(
		 *inode_information );

		*inode_information = NULL;
	}
	return( -1 );
}

/* Frees inode information
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_information_free(
     libfsxfs_inode_information_t **inode_information,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_information_free";

	if( inode_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode information.",
		 function );

		return( -1 );
	}
	if( *inode_information != NULL )
	{
		memory_free(
		 *inode_information );

		*inode_information = NULL;
	}
	return( 1 );
}

/* Reads the inode information data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_information_read_data(
     libfsxfs_inode_information_t *inode_information,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function              = "libfsxfs_inode_information_read_data";
	size_t inode_information_data_size = 0;
	uint32_t format_version            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit               = 0;
	uint32_t value_32bit               = 0;
#endif

	if( inode_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode information.",
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
	if( io_handle->format_version >= 5 )
	{
		inode_information_data_size = sizeof( fsxfs_inode_information_v5_t );
	}
	else
	{
		inode_information_data_size = sizeof( fsxfs_inode_information_t );
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
	if( ( data_size < inode_information_data_size )
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
		 "%s: inode information data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 inode_information_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_compare(
	     ( (fsxfs_inode_information_t *) data )->signature,
	     "XAGI",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_information_t *) data )->format_version,
	 format_version );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_information_t *) data )->inode_btree_root_block_number,
	 inode_information->inode_btree_root_block_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_information_t *) data )->inode_btree_depth,
	 inode_information->inode_btree_depth );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_information_t *) data )->last_allocated_chunk,
	 inode_information->last_allocated_chunk );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsxfs_inode_information_t *) data )->signature[ 0 ],
		 ( (fsxfs_inode_information_t *) data )->signature[ 1 ],
		 ( (fsxfs_inode_information_t *) data )->signature[ 2 ],
		 ( (fsxfs_inode_information_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu32 "\n",
		 function,
		 format_version );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_information_t *) data )->sequence_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: sequence number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_information_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_information_t *) data )->number_of_inodes,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of inodes\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: inode B+ tree root block number\t\t: %" PRIu32 "\n",
		 function,
		 inode_information->inode_btree_root_block_number );

		libcnotify_printf(
		 "%s: inode B+ tree depth\t\t\t: %" PRIu32 "\n",
		 function,
		 inode_information->inode_btree_depth );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_information_t *) data )->number_of_unused_inodes,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of unused inodes\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: last allocated chunk\t\t\t: %" PRIu32 "\n",
		 function,
		 inode_information->last_allocated_chunk );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_information_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unlinked hash table:\n",
		 function );
		libcnotify_print_data(
		 ( (fsxfs_inode_information_t *) data )->unlinked_hash_table,
		 64 * 4,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( format_version != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu32 ".",
		 function,
		 format_version );

		return( -1 );
	}
	if( io_handle->format_version >= 5 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfsxfs_debug_print_guid_value(
			     function,
			     "block type identifier\t\t\t",
			     ( (fsxfs_inode_information_v5_t *) data )->block_type_identifier,
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
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_information_v5_t *) data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_information_v5_t *) data )->unknown3,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown3\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_information_v5_t *) data )->log_sequence_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: log sequence number\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_information_v5_t *) data )->free_inode_btree_root_block_number,
			 value_32bit );
			libcnotify_printf(
			 "%s: free inode B+ tree root block number\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_information_v5_t *) data )->free_inode_btree_depth,
			 value_32bit );
			libcnotify_printf(
			 "%s: free inode B+ tree depth\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	return( 1 );
}

/* Reads the inode information from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_information_read_file_io_handle(
     libfsxfs_inode_information_t *inode_information,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t data[ 512 ];

	static char *function = "libfsxfs_inode_information_read_file_io_handle";
	ssize_t read_count    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading inode information at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              data,
	              512,
	              file_offset,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode information at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsxfs_inode_information_read_data(
	     inode_information,
	     io_handle,
	     data,
	     512,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode information at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

