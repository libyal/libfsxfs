/*
 * Inode functions
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

#include "libfsxfs_attribute_values.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_extent.h"
#include "libfsxfs_extent_btree.h"
#include "libfsxfs_extent_list.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfdatetime.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_inode.h"

/* Creates a inode
 * Make sure the value inode is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_initialize(
     libfsxfs_inode_t **inode,
     size_t inode_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_initialize";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( *inode != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode value already set.",
		 function );

		return( -1 );
	}
	if( ( inode_size != 256 )
	 && ( inode_size != 512 )
	 && ( inode_size != 1024 )
	 && ( inode_size != 2048 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid inode size value out of bounds.",
		 function );

		return( -1 );
	}
	*inode = memory_allocate_structure(
	          libfsxfs_inode_t );

	if( *inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *inode,
	     0,
	     sizeof( libfsxfs_inode_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inode.",
		 function );

		memory_free(
		 *inode );

		*inode = NULL;

		return( -1 );
	}
	( *inode )->data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * inode_size );

	if( ( *inode )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode data.",
		 function );

		goto on_error;
	}
	( *inode )->data_size = inode_size;

	return( 1 );

on_error:
	if( *inode != NULL )
	{
		memory_free(
		 *inode );

		*inode = NULL;
	}
	return( -1 );
}

/* Frees a inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_free(
     libfsxfs_inode_t **inode,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_free";
	int result            = 1;

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( *inode != NULL )
	{
		if( ( *inode )->data != NULL )
		{
			memory_free(
			 ( *inode )->data );
		}
		if( ( *inode )->data_extents_array != NULL )
		{
			if( libcdata_array_free(
			     &( ( *inode )->data_extents_array ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data extents array.",
				 function );

				result = -1;
			}
		}
		if( ( *inode )->attributes_extents_array != NULL )
		{
			if( libcdata_array_free(
			     &( ( *inode )->attributes_extents_array ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attributes extents array.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *inode );

		*inode = NULL;
	}
	return( result );
}

/* Reads the inode data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_read_data(
     libfsxfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function  = "libfsxfs_inode_read_data";
	size_t data_fork_size  = 0;
	size_t inode_data_size = 0;
	uint32_t value_32bit   = 0;
	uint8_t format_version = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit   = 0;
	uint16_t value_16bit   = 0;
#endif

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
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
	if( ( data_size < sizeof( fsxfs_inode_v1_t ) )
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
	format_version = ( (fsxfs_inode_v1_t *) data )->format_version;

	if( format_version == 3 )
	{
		if( data_size < sizeof( fsxfs_inode_v3_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			return( -1 );
		}
		inode_data_size = sizeof( fsxfs_inode_v3_t );
	}
	else
	{
		inode_data_size = sizeof( fsxfs_inode_v1_t );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: inode data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 inode_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_compare(
	    ( (fsxfs_inode_v1_t *) data )->signature,
	    "IN",
	    2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->file_mode,
	 inode->file_mode );

	inode->fork_type = ( (fsxfs_inode_v1_t *) data )->fork_type;

	inode->format_version = format_version;

	if( format_version == 1 )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->number_of_links,
		 inode->number_of_links );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->owner_identifier,
	 inode->owner_identifier );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->group_identifier,
	 inode->group_identifier );

	if( format_version != 1 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v2_t *) data )->number_of_links,
		 inode->number_of_links );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->access_time,
	 value_32bit );

	inode->access_time = (int32_t) value_32bit * (int64_t) 1000000000;

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->access_time_nano_seconds,
	 value_32bit );

	if( inode->access_time > 0 )
	{
		inode->access_time += value_32bit;
	}
	else
	{
		inode->access_time -= value_32bit;
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->modification_time,
	 value_32bit );

	inode->modification_time = (int32_t) value_32bit * (int64_t) 1000000000;

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->modification_time_nano_seconds,
	 value_32bit );

	if( inode->modification_time > 0 )
	{
		inode->modification_time += value_32bit;
	}
	else
	{
		inode->modification_time -= value_32bit;
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->inode_change_time,
	 value_32bit );

	inode->inode_change_time = (int32_t) value_32bit * (int64_t) 1000000000;

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->inode_change_time_nano_seconds,
	 value_32bit );

	if( inode->inode_change_time > 0 )
	{
		inode->inode_change_time += value_32bit;
	}
	else
	{
		inode->inode_change_time -= value_32bit;
	}
	byte_stream_copy_to_uint64_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->data_size,
	 inode->size );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->number_of_data_extents,
	 inode->number_of_data_extents );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_inode_v1_t *) data )->number_of_attributes_extents,
	 inode->number_of_attributes_extents );

	inode->attributes_fork_offset = (uint16_t) ( (fsxfs_inode_v1_t *) data )->attributes_fork_offset * 8;

	inode->attributes_fork_type = ( (fsxfs_inode_v1_t *) data )->attributes_fork_type;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c\n",
		 function,
		 ( (fsxfs_inode_v1_t *) data )->signature[ 0 ],
		 ( (fsxfs_inode_v1_t *) data )->signature[ 1 ] );

		libcnotify_printf(
		 "%s: file mode\t\t\t\t\t: 0o07%" PRIo16 " (0x%04" PRIx16 ")\n",
		 function,
		 inode->file_mode,
		 inode->file_mode );
		libfsxfs_debug_print_file_mode(
		 inode->file_mode );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu8 "\n",
		 function,
		 format_version );

		libcnotify_printf(
		 "%s: fork type\t\t\t\t\t: %" PRIu8 " (%s)\n",
		 function,
		 ( (fsxfs_inode_v1_t *) data )->fork_type,
		 libfsxfs_debug_print_fork_type(
		  ( (fsxfs_inode_v1_t *) data )->fork_type ) );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: number of links\t\t\t\t: %" PRIu32 "\n",
			 function,
			 inode->number_of_links );
		}
		else
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fsxfs_inode_v1_t *) data )->number_of_links,
			 value_16bit );
			libcnotify_printf(
			 "%s: unknown1\t\t\t\t\t: 0x%04" PRIx16 "\n",
			 function,
			 value_16bit );
		}
		libcnotify_printf(
		 "%s: owner identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 inode->owner_identifier );

		libcnotify_printf(
		 "%s: group identifier\t\t\t\t: %" PRIu32 "\n",
		 function,
		 inode->group_identifier );

		if( format_version == 1 )
		{
			libcnotify_printf(
			 "%s: unknown2:\n",
			 function );
			libcnotify_print_data(
			 ( (fsxfs_inode_v1_t *) data )->unknown2,
			 14,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else
		{
			libcnotify_printf(
			 "%s: number of links\t\t\t\t: %" PRIu32 "\n",
			 function,
			 inode->number_of_links );

			byte_stream_copy_to_uint16_big_endian(
			 ( (fsxfs_inode_v2_t *) data )->project_identifier,
			 value_16bit );
			libcnotify_printf(
			 "%s: project identifier\t\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v2_t *) data )->unknown2,
			 value_64bit );
			libcnotify_printf(
			 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->flush_counter,
		 value_16bit );
		libcnotify_printf(
		 "%s: flush counter\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		if( libfsxfs_debug_print_posix_time_value(
		     function,
		     "access time (seconds)\t\t\t\t",
		     ( (fsxfs_inode_v1_t *) data )->access_time,
		     4,
		     LIBFDATETIME_ENDIAN_BIG,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->access_time_nano_seconds,
		 value_32bit );
		libcnotify_printf(
		 "%s: access time nano seconds\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfsxfs_debug_print_posix_time_value(
		     function,
		     "modification time (seconds)\t\t\t",
		     ( (fsxfs_inode_v1_t *) data )->modification_time,
		     4,
		     LIBFDATETIME_ENDIAN_BIG,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->modification_time_nano_seconds,
		 value_32bit );
		libcnotify_printf(
		 "%s: modification time nano seconds\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfsxfs_debug_print_posix_time_value(
		     function,
		     "inode change time (seconds)\t\t\t",
		     ( (fsxfs_inode_v1_t *) data )->inode_change_time,
		     4,
		     LIBFDATETIME_ENDIAN_BIG,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print POSIX time value.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->inode_change_time_nano_seconds,
		 value_32bit );
		libcnotify_printf(
		 "%s: inode change time nano seconds\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: data size\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 inode->size );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->extent_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: extent size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of data extents\t\t\t: %" PRIu32 "\n",
		 function,
		 inode->number_of_data_extents );

		libcnotify_printf(
		 "%s: number of attributes extents\t\t\t: %" PRIu16 "\n",
		 function,
		 inode->number_of_attributes_extents );

		libcnotify_printf(
		 "%s: attributes fork offset\t\t\t: %" PRIu8 " (%" PRIu16 ")\n",
		 function,
		 ( (fsxfs_inode_v1_t *) data )->attributes_fork_offset,
		 inode->attributes_fork_offset );

		libcnotify_printf(
		 "%s: attributes fork type\t\t\t\t: %" PRIu8 " (%s)\n",
		 function,
		 inode->attributes_fork_type,
		 libfsxfs_debug_print_fork_type(
		  inode->attributes_fork_type ) );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->unknown5,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->unknown6,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->inode_flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: inode flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v1_t *) data )->generation_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: generation number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v3_t *) data )->unknown7,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( format_version != 1 )
	 && ( format_version != 2 )
	 && ( format_version != 3 ) )
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
	if( format_version == 3 )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v3_t *) data )->creation_time,
		 value_32bit );

		inode->creation_time = (int32_t) value_32bit * (int64_t) 1000000000;

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_inode_v3_t *) data )->creation_time_nano_seconds,
		 value_32bit );

		if( inode->creation_time > 0 )
		{
			inode->creation_time += value_32bit;
		}
		else
		{
			inode->creation_time -= value_32bit;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->checksum,
			 value_32bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->change_count,
			 value_64bit );
			libcnotify_printf(
			 "%s: change count\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->log_sequence_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: log sequence number\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->extended_inode_flags,
			 value_64bit );
			libcnotify_printf(
			 "%s: extended inode flags\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->cow_extent_size,
			 value_32bit );
			libcnotify_printf(
			 "%s: copy-on-write extent size\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: unknown8:\n",
			 function );
			libcnotify_print_data(
			 ( (fsxfs_inode_v3_t *) data )->unknown8,
			 12,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			if( libfsxfs_debug_print_posix_time_value(
			     function,
			     "creation time (seconds)\t\t\t",
			     ( (fsxfs_inode_v3_t *) data )->creation_time,
			     4,
			     LIBFDATETIME_ENDIAN_BIG,
			     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
			     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print POSIX time value.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->creation_time_nano_seconds,
			 value_32bit );
			libcnotify_printf(
			 "%s: creation time nano seconds\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );

			byte_stream_copy_to_uint64_big_endian(
			 ( (fsxfs_inode_v3_t *) data )->inode_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: inode number\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			if( libfsxfs_debug_print_guid_value(
			     function,
			     "inode type identifier\t\t\t\t",
			     ( (fsxfs_inode_v3_t *) data )->inode_type_identifier,
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
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_fork_size = data_size - inode_data_size;

	if( inode->attributes_fork_offset > 0 )
	{
		if( inode->attributes_fork_offset >= data_fork_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attributes fork offset value out of bounds.",
			 function );

			return( -1 );
		}
		data_fork_size = (size_t) inode->attributes_fork_offset;

		inode->attributes_fork_offset += inode_data_size;
		inode->attributes_fork_size    = data_size - inode->attributes_fork_offset;
	}
	inode->data_fork_offset = (uint16_t) inode_data_size;
	inode->data_fork_size   = (uint16_t) data_fork_size;

	if( ( inode->fork_type == LIBFSXFS_FORK_TYPE_INLINE_DATA )
	 && ( inode->size > inode->data_fork_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid inline data size value out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the inode from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_read_file_io_handle(
     libfsxfs_inode_t *inode,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsxfs_extent_btree_t *extent_btree = NULL;
	static char *function                 = "libfsxfs_inode_read_file_io_handle";
	ssize_t read_count                    = 0;
	uint64_t number_of_blocks             = 0;
	uint8_t add_sparse_extents            = 0;

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( io_handle->block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - block size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading inode at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              inode->data,
	              inode->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) inode->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsxfs_inode_read_data(
	     inode,
	     inode->data,
	     inode->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	number_of_blocks = inode->size / io_handle->block_size;

	if( ( inode->size % io_handle->block_size ) != 0 )
	{
		number_of_blocks++;
	}
	if( ( inode->file_mode & 0xf000 ) != LIBFSXFS_FILE_TYPE_DIRECTORY )
	{
		add_sparse_extents = 1;
	}
	if( inode->fork_type == LIBFSXFS_FORK_TYPE_DEVICE )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: device identifier data:\n",
			 function );
			libcnotify_print_data(
			 &( inode->data[ inode->data_fork_offset ] ),
			 4,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		byte_stream_copy_to_uint32_big_endian(
		 &( inode->data[ inode->data_fork_offset ] ),
		 inode->device_identifier );
	}
	else if( inode->fork_type == LIBFSXFS_FORK_TYPE_INLINE_DATA )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: inline data:\n",
			 function );
			libcnotify_print_data(
			 &( inode->data[ inode->data_fork_offset ] ),
			 (size_t) inode->size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		inode->inline_data = &( inode->data[ inode->data_fork_offset ] );
	}
	else if( inode->fork_type == LIBFSXFS_FORK_TYPE_EXTENTS )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data extent list:\n",
			 function );
			libcnotify_print_data(
			 &( inode->data[ inode->data_fork_offset ] ),
			 (size_t) inode->data_fork_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libcdata_array_initialize(
		     &( inode->data_extents_array ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data extents array.",
			 function );

			goto on_error;
		}
		if( libfsxfs_extent_list_read_data(
		     inode->data_extents_array,
		     number_of_blocks,
		     inode->number_of_data_extents,
		     &( inode->data[ inode->data_fork_offset ] ),
		     inode->data_fork_size,
		     add_sparse_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data extent list.",
			 function );

			goto on_error;
		}
	}
	else if( inode->fork_type == LIBFSXFS_FORK_TYPE_BTREE )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data extent B+ tree:\n",
			 function );
			libcnotify_print_data(
			 &( inode->data[ inode->data_fork_offset ] ),
			 (size_t) inode->data_fork_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libcdata_array_initialize(
		     &( inode->data_extents_array ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data extents array.",
			 function );

			goto on_error;
		}
		if( libfsxfs_extent_btree_initialize(
		     &extent_btree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data extents B+ tree.",
			 function );

			goto on_error;
		}
		if( libfsxfs_extent_btree_get_extents_from_root_node(
		     extent_btree,
		     io_handle,
		     file_io_handle,
		     number_of_blocks,
		     &( inode->data[ inode->data_fork_offset ] ),
		     inode->data_fork_size,
		     inode->data_extents_array,
		     add_sparse_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data extents from extent B+ tree.",
			 function );

			goto on_error;
		}
		if( libfsxfs_extent_btree_free(
		     &extent_btree,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data extents B+ tree.",
			 function );

			goto on_error;
		}
	}
	if( inode->attributes_fork_size > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: attributes fork data:\n",
			 function );
			libcnotify_print_data(
			 &( inode->data[ inode->attributes_fork_offset ] ),
			 (size_t) inode->attributes_fork_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		number_of_blocks = inode->attributes_fork_size / io_handle->block_size;

		if( ( inode->attributes_fork_size % io_handle->block_size ) != 0 )
		{
			number_of_blocks++;
		}
		if( inode->attributes_fork_type == LIBFSXFS_FORK_TYPE_INLINE_DATA )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: inline attributes data:\n",
				 function );
				libcnotify_print_data(
				 &( inode->data[ inode->attributes_fork_offset ] ),
				 (size_t) inode->attributes_fork_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			inode->inline_attributes_data = &( inode->data[ inode->attributes_fork_offset ] );
		}
		else if( ( inode->attributes_fork_type == LIBFSXFS_FORK_TYPE_EXTENTS )
		      && ( inode->number_of_attributes_extents > 0 ) )
		{
			if( libcdata_array_initialize(
			     &( inode->attributes_extents_array ),
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create attributes extents array.",
				 function );

				goto on_error;
			}
			if( libfsxfs_extent_list_read_data(
			     inode->attributes_extents_array,
			     number_of_blocks,
			     inode->number_of_attributes_extents,
			     &( inode->data[ inode->attributes_fork_offset ] ),
			     inode->attributes_fork_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read attributes extent list.",
				 function );

				goto on_error;
			}
		}
		else if( inode->attributes_fork_type == LIBFSXFS_FORK_TYPE_BTREE )
		{
			if( libcdata_array_initialize(
			     &( inode->attributes_extents_array ),
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create attributes extents array.",
				 function );

				goto on_error;
			}
			if( libfsxfs_extent_btree_initialize(
			     &extent_btree,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create attributes extents B+ tree.",
				 function );

				goto on_error;
			}
			if( libfsxfs_extent_btree_get_extents_from_root_node(
			     extent_btree,
			     io_handle,
			     file_io_handle,
			     number_of_blocks,
			     &( inode->data[ inode->attributes_fork_offset ] ),
			     inode->attributes_fork_size,
			     inode->attributes_extents_array,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attributes extents from extent B+ tree.",
				 function );

				goto on_error;
			}
			if( libfsxfs_extent_btree_free(
			     &extent_btree,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free attributes extents B+ tree.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( extent_btree != NULL )
	{
		libfsxfs_extent_btree_free(
		 &extent_btree,
		 NULL );
	}
	if( inode->attributes_extents_array != NULL )
	{
		libcdata_array_free(
		 &( inode->attributes_extents_array ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
		 NULL );
	}
	if( inode->data_extents_array != NULL )
	{
		libcdata_array_free(
		 &( inode->data_extents_array ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the creation time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_inode_get_creation_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_creation_time";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	if( inode->format_version == 3 )
	{
		*posix_time = inode->creation_time;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the modification time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_modification_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_modification_time";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = inode->modification_time;

	return( 1 );
}

/* Retrieves the access time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_access_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_access_time";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = inode->access_time;

	return( 1 );
}

/* Retrieves the inode change time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_inode_change_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_inode_change_time";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( posix_time == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid POSIX time.",
		 function );

		return( -1 );
	}
	*posix_time = inode->inode_change_time;

	return( 1 );
}

/* Retrieves the owner identifier
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_owner_identifier(
     libfsxfs_inode_t *inode,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_owner_identifier";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( owner_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid owner identifier.",
		 function );

		return( -1 );
	}
	*owner_identifier = inode->owner_identifier;

	return( 1 );
}

/* Retrieves the group identifier
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_group_identifier(
     libfsxfs_inode_t *inode,
     uint32_t *group_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_group_identifier";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( group_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid group identifier.",
		 function );

		return( -1 );
	}
	*group_identifier = inode->group_identifier;

	return( 1 );
}

/* Retrieves the file mode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_file_mode(
     libfsxfs_inode_t *inode,
     uint16_t *file_mode,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_file_mode";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( file_mode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file mode.",
		 function );

		return( -1 );
	}
	*file_mode = inode->file_mode;

	return( 1 );
}

/* Retrieves the number of (hard) links
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_number_of_links(
     libfsxfs_inode_t *inode,
     uint32_t *number_of_links,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_number_of_links";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( number_of_links == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of (hard) links.",
		 function );

		return( -1 );
	}
	*number_of_links = inode->number_of_links;

	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_data_size(
     libfsxfs_inode_t *inode,
     uint64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_data_size";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = inode->size;

	return( 1 );
}

/* Retrieves the device identifier
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_inode_get_device_identifier(
     libfsxfs_inode_t *inode,
     uint32_t *device_identifier,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_device_identifier";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( device_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid device identifier.",
		 function );

		return( -1 );
	}
	if( inode->fork_type == LIBFSXFS_FORK_TYPE_DEVICE )
	{
		*device_identifier = inode->device_identifier;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the device number
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_inode_get_device_number(
     libfsxfs_inode_t *inode,
     uint32_t *major_device_number,
     uint32_t *minor_device_number,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_device_number";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( major_device_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major device number.",
		 function );

		return( -1 );
	}
	if( minor_device_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor device number.",
		 function );

		return( -1 );
	}
	if( inode->fork_type == LIBFSXFS_FORK_TYPE_DEVICE )
	{
		*major_device_number = inode->device_identifier >> 18;
		*minor_device_number = inode->device_identifier & 0x0003ffffUL;

		return( 1 );
	}
	return( 0 );
}

/* Retrieves the number of data extents
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_number_of_data_extents(
     libfsxfs_inode_t *inode,
     int *number_of_extents,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_number_of_data_extents";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     inode->data_extents_array,
	     number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific data extent
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_data_extent_by_index(
     libfsxfs_inode_t *inode,
     int extent_index,
     libfsxfs_extent_t **extent,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_data_extent_by_index";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     inode->data_extents_array,
	     extent_index,
	     (intptr_t **) extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data extent: %d.",
		 function,
		 extent_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of attributes extents
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_number_of_attributes_extents(
     libfsxfs_inode_t *inode,
     int *number_of_extents,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_number_of_attributes_extents";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     inode->attributes_extents_array,
	     number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific attributes extent
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_get_attributes_extent_by_index(
     libfsxfs_inode_t *inode,
     int extent_index,
     libfsxfs_extent_t **extent,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_get_attributes_extent_by_index";

	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     inode->attributes_extents_array,
	     extent_index,
	     (intptr_t **) extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attributes extent: %d.",
		 function,
		 extent_index );

		return( -1 );
	}
	return( 1 );
}

