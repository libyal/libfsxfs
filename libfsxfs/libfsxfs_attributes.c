/*
 * (Extended) attributes functions
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
#include <types.h>

#include "libfsxfs_attribute_values.h"
#include "libfsxfs_attributes.h"
#include "libfsxfs_attributes_leaf_block_header.h"
#include "libfsxfs_attributes_table.h"
#include "libfsxfs_block_data_handle.h"
#include "libfsxfs_data_stream.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_extent.h"
#include "libfsxfs_file_system_block.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfdata.h"

#include "fsxfs_attributes_block.h"
#include "fsxfs_file_system_block.h"

/* Reads the attributes branch values
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_read_branch_values(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extended_attributes_array,
     int recursion_depth,
     libcerror_error_t **error )
{
	static char *function          = "libfsxfs_attributes_read_branch_values";
	size_t branch_header_data_size = 0;
	size_t data_offset             = 0;
	size_t entries_data_size       = 0;
	uint32_t sub_block_number      = 0;
	uint16_t block_entry_index     = 0;
	uint16_t number_of_entries     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit           = 0;
	uint16_t value_16bit           = 0;
#endif

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
		data_offset             = sizeof( fsxfs_file_system_block_header_v3_t );
		branch_header_data_size = sizeof( fsxfs_attributes_branch_block_header_v3_t );
	}
	else
	{
		data_offset             = sizeof( fsxfs_file_system_block_header_v2_t );
		branch_header_data_size = sizeof( fsxfs_attributes_branch_block_header_v2_t );
	}
	if( ( data_offset >= data_size )
	 || ( branch_header_data_size > ( data_size - data_offset ) ) )
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
		 "%s: branch header data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 branch_header_data_size,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_attributes_branch_block_header_v2_t *) &( data[ data_offset ] ) )->number_of_entries,
		 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: number of entries\t\t: %" PRIu16 "\n",
			 function,
			 number_of_entries );

			byte_stream_copy_to_uint16_big_endian(
			 ( (fsxfs_attributes_branch_block_header_v2_t *) &( data[ data_offset ] ) )->node_level,
			 value_16bit );
			libcnotify_printf(
			 "%s: node level\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			if( io_handle->format_version == 5 )
			{
				byte_stream_copy_to_uint32_big_endian(
				 ( (fsxfs_attributes_branch_block_header_v3_t *) &( data[ data_offset ] ) )->unknown1,
				 value_32bit );
				libcnotify_printf(
				 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 value_32bit );
			}
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += branch_header_data_size;

	entries_data_size = sizeof( fsxfs_attributes_branch_block_entry_t ) * number_of_entries;

	if( entries_data_size > ( data_size - data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute branch entries data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 entries_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	for( block_entry_index = 0;
	     block_entry_index < number_of_entries;
	     block_entry_index++ )
	{
		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_attributes_branch_block_entry_t *) &( data[ data_offset ] ) )->sub_block_number,
		 sub_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_branch_block_entry_t *) &( data[ data_offset ] ) )->name_hash,
			 value_32bit );
			libcnotify_printf(
			 "%s: name hash\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: sub block number\t\t: %" PRIu32 "\n",
			 function,
			 sub_block_number );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsxfs_attributes_branch_block_entry_t );

		if( libfsxfs_attributes_get_from_block(
		     io_handle,
		     file_io_handle,
		     inode,
		     sub_block_number,
		     extended_attributes_array,
		     recursion_depth + 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extended attributes from attributes block: %" PRIu32 ".",
			 function,
			 sub_block_number );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the attributes leaf values
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_read_leaf_values(
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extended_attributes_array,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values              = NULL;
	libfsxfs_attributes_leaf_block_header_t *leaf_block_header = NULL;
	static char *function                                      = "libfsxfs_attributes_read_leaf_values";
	size_t data_offset                                         = 0;
	size_t entries_data_end_offset                             = 0;
	size_t entries_data_size                                   = 0;
	size_t values_data_size                                    = 0;
	uint16_t block_entry_index                                 = 0;
	uint16_t values_offset                                     = 0;
	uint8_t flags                                              = 0;
	uint8_t name_size                                          = 0;
	int entry_index                                            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                                       = 0;
#endif

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
		data_offset = sizeof( fsxfs_file_system_block_header_v3_t );
	}
	else
	{
		data_offset = sizeof( fsxfs_file_system_block_header_v2_t );
	}
	if( data_offset >= data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsxfs_attributes_leaf_block_header_initialize(
	     &leaf_block_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize attributes leaf block header.",
		 function );

		goto on_error;
	}
	if( libfsxfs_attributes_leaf_block_header_read_data(
	     leaf_block_header,
	     io_handle,
	     &( data[ data_offset ] ),
	     data_size - data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attributes leaf block header.",
		 function );

		goto on_error;
	}
	if( io_handle->format_version == 5 )
	{
		data_offset += sizeof( fsxfs_attributes_leaf_block_header_v3_t );
	}
	else
	{
		data_offset += sizeof( fsxfs_attributes_leaf_block_header_v2_t );
	}
	entries_data_size = sizeof( fsxfs_attributes_leaf_block_entry_t ) * leaf_block_header->number_of_entries;

	if( entries_data_size > ( data_size - data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute leaf entries data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 entries_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	entries_data_end_offset = data_offset + entries_data_size;

	for( block_entry_index = 0;
	     block_entry_index < leaf_block_header->number_of_entries;
	     block_entry_index++ )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_attributes_leaf_block_entry_t *) &( data[ data_offset ] ) )->values_offset,
		 values_offset );

		flags = ( (fsxfs_attributes_leaf_block_entry_t *) &( data[ data_offset ] ) )->flags;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_leaf_block_entry_t *) &( data[ data_offset ] ) )->name_hash,
			 value_32bit );
			libcnotify_printf(
			 "%s: name hash\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: values offset\t\t\t: %" PRIu16 "\n",
			 function,
			 values_offset );

			libcnotify_printf(
			 "%s: flags\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 flags );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsxfs_attributes_leaf_block_entry_t *) &( data[ data_offset ] ) )->unknown1 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsxfs_attributes_leaf_block_entry_t );

		if( ( values_offset < entries_data_end_offset )
		 || ( values_offset >= data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid values offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( flags & 0x01 ) != 0 )
		{
			values_data_size = sizeof( fsxfs_attributes_block_values_local_t );
		}
		else
		{
			values_data_size = sizeof( fsxfs_attributes_block_values_remote_t );
		}
		if( values_data_size > ( data_size - values_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid values data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( flags & 0x01 ) != 0 )
		{
			name_size = ( (fsxfs_attributes_block_values_local_t *) &( data[ values_offset ] ) )->name_size;
		}
		else
		{
			name_size = ( (fsxfs_attributes_block_values_remote_t *) &( data[ values_offset ] ) )->name_size;
		}
		if( name_size > ( data_size - values_offset - values_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: values data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ values_offset ] ),
			 values_data_size + name_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfsxfs_attribute_values_initialize(
		     &attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute: %d values.",
			 function,
			 block_entry_index );

			goto on_error;
		}
		if( ( flags & 0x01 ) != 0 )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fsxfs_attributes_block_values_local_t *) &( data[ values_offset ] ) )->value_data_size,
			 attribute_values->value_data_size );
		}
		else
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_block_values_remote_t *) &( data[ values_offset ] ) )->value_data_block_number,
			 attribute_values->value_data_block_number );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_block_values_remote_t *) &( data[ values_offset ] ) )->value_data_size,
			 attribute_values->value_data_size );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( flags & 0x01 ) == 0 )
			{
				libcnotify_printf(
				 "%s: value data block number\t\t: %" PRIu32 "\n",
				 function,
				 attribute_values->value_data_block_number );
			}
			libcnotify_printf(
			 "%s: value data size\t\t\t: %" PRIu32 "\n",
			 function,
			 attribute_values->value_data_size );

			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu8 "\n",
			 function,
			 name_size );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		values_offset += values_data_size;

		if( name_size > 0 )
		{
			if( libfsxfs_attribute_values_set_name(
			     attribute_values,
			     &( data[ values_offset ] ),
			     name_size,
			     flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set name.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libfsxfs_debug_print_utf8_string_value(
				     function,
				     "name\t\t\t\t",
				     attribute_values->name,
				     attribute_values->name_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print UTF-8 string value.",
					 function );

					goto on_error;
				}
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			values_offset += name_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( ( flags & 0x01 ) != 0 )
		{
			if( attribute_values->value_data_size > ( data_size - values_offset ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid value data size value out of bounds.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: value data:\n",
				 function );
				libcnotify_print_data(
				 &( data[ values_offset ] ),
				 (size_t) attribute_values->value_data_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			if( attribute_values->value_data_size > 0 )
			{
				if( libfsxfs_attribute_values_set_value_data(
				     attribute_values,
				     &( data[ values_offset ] ),
				     (size_t) attribute_values->value_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set value data.",
					 function );

					goto on_error;
				}
			}
		}
		if( libcdata_array_append_entry(
		     extended_attributes_array,
		     &entry_index,
		     (intptr_t *) attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attribute values to extended attributes array.",
			 function );

			goto on_error;
		}
		attribute_values = NULL;
	}
	if( libfsxfs_attributes_leaf_block_header_free(
	     &leaf_block_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free attributes leaf block header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( attribute_values != NULL )
	{
		libfsxfs_attribute_values_free(
		 &attribute_values,
		 NULL );
	}
	if( leaf_block_header != NULL )
	{
		libfsxfs_attributes_leaf_block_header_free(
		 &leaf_block_header,
		 NULL );
	}
	libcdata_array_empty(
	 extended_attributes_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_attribute_values_free,
	 NULL );

	return( -1 );
}

/* Retrieves the extended attributes from an attributes block
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_get_from_block(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     uint32_t block_number,
     libcdata_array_t *extended_attributes_array,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsxfs_extent_t *extent                       = NULL;
	libfsxfs_file_system_block_t *file_system_block = NULL;
	static char *function                           = "libfsxfs_attributes_get_from_block";
	off64_t block_offset                            = 0;
	uint64_t relative_block_number                  = 0;
	int allocation_group_index                      = 0;
	int extent_index                                = 0;
	int number_of_extents                           = 0;

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
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBFSXFS_MAXIMUM_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfsxfs_inode_get_number_of_attributes_extents(
	     inode,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of attributes extents.",
		 function );

		goto on_error;
	}
/* TODO optimize this lookup */

	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libfsxfs_inode_get_attributes_extent_by_index(
		     inode,
		     extent_index,
		     &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attributes extent: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( extent == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing extent: %d.",
			 function,
			 extent_index );

			goto on_error;
		}
		if( ( block_number >= extent->logical_block_number )
		 && ( block_number < ( extent->logical_block_number + extent->number_of_blocks ) ) )
		{
			break;
		}
	}
	if( extent_index >= number_of_extents )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block number: %" PRIu32 " value out of bounds.",
		 function,
		 block_number );

		return( -1 );
	}
	allocation_group_index = (int) ( extent->physical_block_number >> io_handle->number_of_relative_block_number_bits );
	relative_block_number  = extent->physical_block_number & ( ( 1 << io_handle->number_of_relative_block_number_bits ) - 1 );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extent: %d physical block number\t: %" PRIu64 "\n",
		 function,
		 extent_index,
		 extent->physical_block_number );

		libcnotify_printf(
		 "%s: extent: %d allocation group index\t: %d\n",
		 function,
		 extent_index,
		 allocation_group_index );

		libcnotify_printf(
		 "%s: extent: %d relative block number\t: %" PRIu64 "\n",
		 function,
		 extent_index,
		 relative_block_number );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	block_offset  = ( (off64_t) allocation_group_index * io_handle->allocation_group_size ) + relative_block_number;
	block_offset += block_number - extent->logical_block_number;
	block_offset *= io_handle->block_size;

	if( libfsxfs_file_system_block_initialize(
	     &file_system_block,
	     io_handle->block_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file system block.",
		 function );

		goto on_error;
	}
	if( libfsxfs_file_system_block_read_file_io_handle(
	     file_system_block,
	     io_handle,
	     file_io_handle,
	     block_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 block_offset,
		 block_offset );

		goto on_error;
	}
	if( ( file_system_block->header->signature == 0x3bee )
	 || ( file_system_block->header->signature == 0xfbee ) )
	{
		if( libfsxfs_attributes_read_leaf_values(
		     io_handle,
		     file_system_block->data,
		     file_system_block->data_size,
		     extended_attributes_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attributes leaf values.",
			 function );

			goto on_error;
		}
	}
	else if( ( file_system_block->header->signature == 0x3ebe )
	      || ( file_system_block->header->signature == 0xfebe ) )
	{
		if( libfsxfs_attributes_read_branch_values(
		     io_handle,
		     file_io_handle,
		     inode,
		     file_system_block->data,
		     file_system_block->data_size,
		     extended_attributes_array,
		     recursion_depth,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attributes branch values.",
			 function );

			goto on_error;
		}
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block signature: 0x%04" PRIx16 ".",
		 function,
		 file_system_block->header->signature );

		goto on_error;
	}
	if( libfsxfs_file_system_block_free(
	     &file_system_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file system block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_system_block != NULL )
	{
		libfsxfs_file_system_block_free(
		 &file_system_block,
		 NULL );
	}
	libcdata_array_empty(
	 extended_attributes_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_attribute_values_free,
	 NULL );

	return( -1 );
}

/* Retrieves the extended attributes from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_get_from_inode(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libcdata_array_t *extended_attributes_array,
     libcerror_error_t **error )
{
	libfsxfs_attributes_table_t *attributes_table = NULL;
	static char *function                         = "libfsxfs_attributes_get_from_inode";

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
	if( ( inode->attributes_fork_type != LIBFSXFS_FORK_TYPE_INLINE_DATA )
	 && ( inode->attributes_fork_type != LIBFSXFS_FORK_TYPE_EXTENTS )
	 && ( inode->attributes_fork_type != LIBFSXFS_FORK_TYPE_BTREE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid inode - unsupported attributes fork type.",
		 function );

		goto on_error;
	}
	if( inode->attributes_fork_type == LIBFSXFS_FORK_TYPE_INLINE_DATA )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: extended attributes data:\n",
			 function );
			libcnotify_print_data(
			 inode->inline_attributes_data,
			 inode->attributes_fork_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfsxfs_attributes_table_initialize(
		     &attributes_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attributes table.",
			 function );

			goto on_error;
		}
		if( libfsxfs_attributes_table_read_data(
		     attributes_table,
		     inode->inline_attributes_data,
		     inode->attributes_fork_size,
		     extended_attributes_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attributes table.",
			 function );

			goto on_error;
		}
		if( libfsxfs_attributes_table_free(
		     &attributes_table,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free attributes table.",
			 function );

			goto on_error;
		}
	}
	else if( inode->attributes_extents_array != NULL )
	{
		if( libfsxfs_attributes_get_from_block(
		     io_handle,
		     file_io_handle,
		     inode,
		     0,
		     extended_attributes_array,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extended attributes from attributes block: 0.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( attributes_table != NULL )
	{
		libfsxfs_attributes_table_free(
		 &attributes_table,
		 NULL );
	}
	return( -1 );
}

/* Creates a data stream of the attribute value data
 * Make sure the value data_stream is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_get_value_data_stream(
     libfsxfs_io_handle_t *io_handle,
     libfsxfs_inode_t *inode,
     libfsxfs_attribute_values_t *attribute_values,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error )
{
	libfdata_stream_t *safe_data_stream = NULL;
	libfsxfs_extent_t *extent           = NULL;
	static char *function               = "libfsxfs_attributes_get_value_data_stream";
	size64_t data_segment_size          = 0;
	off64_t data_segment_offset         = 0;
	uint64_t relative_block_number      = 0;
	uint32_t remaining_value_data_size  = 0;
	int allocation_group_index          = 0;
	int extent_index                    = 0;
	int number_of_extents               = 0;
	int segment_index                   = 0;

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
	if( attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute values.",
		 function );

		return( -1 );
	}
	if( data_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data stream.",
		 function );

		return( -1 );
	}
	if( attribute_values->value_data_block_number == 0 )
	{
		if( libfsxfs_data_stream_initialize_from_data(
		     data_stream,
		     attribute_values->value_data,
		     (size_t) attribute_values->value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data stream.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfsxfs_inode_get_number_of_attributes_extents(
		     inode,
		     &number_of_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of attributes extents.",
			 function );

			goto on_error;
		}
		for( extent_index = 0;
		     extent_index < number_of_extents;
		     extent_index++ )
		{
			if( libfsxfs_inode_get_attributes_extent_by_index(
			     inode,
			     extent_index,
			     &extent,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve attributes extent: %d.",
				 function,
				 extent_index );

				goto on_error;
			}
			if( extent == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing extent: %d.",
				 function,
				 extent_index );

				goto on_error;
			}
			if( ( attribute_values->value_data_block_number >= extent->logical_block_number )
			 && ( attribute_values->value_data_block_number < ( extent->logical_block_number + extent->number_of_blocks ) ) )
			{
				break;
			}
		}
		if( extent_index >= number_of_extents )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid block number: %" PRIu32 " value out of bounds.",
			 function,
			 attribute_values->value_data_block_number );

			return( -1 );
		}
/* TODO add v5 remote value data block header support */
		if( io_handle->format_version == 5 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid IO handle - unsupported format version.",
			 function );

			return( -1 );
		}
		if( libfdata_stream_initialize(
		     &safe_data_stream,
		     NULL,
		     NULL,
		     NULL,
		     NULL,
		     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libfsxfs_block_data_handle_read_segment_data,
		     NULL,
		     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libfsxfs_block_data_handle_seek_segment_offset,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data stream.",
			 function );

			goto on_error;
		}
		remaining_value_data_size = attribute_values->value_data_size;

		while( remaining_value_data_size > 0 )
		{
			if( extent == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing extent: %d.",
				 function,
				 extent_index );

				goto on_error;
			}
			allocation_group_index = (int) ( extent->physical_block_number >> io_handle->number_of_relative_block_number_bits );
			relative_block_number  = extent->physical_block_number & ( ( 1 << io_handle->number_of_relative_block_number_bits ) - 1 );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: extent: %d allocation group index\t: %d\n",
				 function,
				 extent_index,
				 allocation_group_index );

				libcnotify_printf(
				 "%s: extent: %d relative block number\t: %" PRIu64 "\n",
				 function,
				 extent_index,
				 relative_block_number );

				libcnotify_printf(
				 "\n" );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			data_segment_offset = ( ( (off64_t) allocation_group_index * io_handle->allocation_group_size ) + relative_block_number ) * io_handle->block_size;
			data_segment_size   = (size64_t) extent->number_of_blocks * io_handle->block_size;

			if( data_segment_size > remaining_value_data_size )
			{
				data_segment_size = remaining_value_data_size;
			}
			if( libfdata_stream_append_segment(
			     safe_data_stream,
			     &segment_index,
			     0,
			     data_segment_offset,
			     data_segment_size,
			     extent->range_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append extent: %d data stream segment.",
				 function,
				 extent_index );

				goto on_error;
			}
			remaining_value_data_size -= data_segment_size;

			if( remaining_value_data_size > 0 )
			{
				if( libfsxfs_inode_get_attributes_extent_by_index(
				     inode,
				     extent_index,
				     &extent,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve data extent: %d.",
					 function,
					 extent_index );

					goto on_error;
				}
			}
		}
		*data_stream = safe_data_stream;
	}
	return( 1 );

on_error:
	if( safe_data_stream != NULL )
	{
		libfdata_stream_free(
		 &safe_data_stream,
		 NULL );
	}
	return( -1 );
}

