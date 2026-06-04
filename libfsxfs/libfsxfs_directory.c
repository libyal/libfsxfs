/*
 * Directory functions
 *
 * Copyright (C) 2020-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include "libfsxfs_block_directory.h"
#include "libfsxfs_block_tree.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_directory.h"
#include "libfsxfs_directory_entry.h"
#include "libfsxfs_directory_leaf_block_header.h"
#include "libfsxfs_directory_table.h"
#include "libfsxfs_extent.h"
#include "libfsxfs_file_system_block.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libuna.h"

#include "fsxfs_directory_block.h"
#include "fsxfs_file_system_block.h"

/* Creates a directory
 * Make sure the value directory is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_initialize(
     libfsxfs_directory_t **directory,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_initialize";

	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( *directory != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory value already set.",
		 function );

		return( -1 );
	}
	*directory = memory_allocate_structure(
	              libfsxfs_directory_t );

	if( *directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory,
	     0,
	     sizeof( libfsxfs_directory_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory.",
		 function );

		memory_free(
		 *directory );

		*directory = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *directory )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory != NULL )
	{
		memory_free(
		 *directory );

		*directory = NULL;
	}
	return( -1 );
}

/* Frees a directory
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_free(
     libfsxfs_directory_t **directory,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_free";
	int result            = 1;

	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( *directory != NULL )
	{
		if( libcdata_array_free(
		     &( ( *directory )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *directory );

		*directory = NULL;
	}
	return( result );
}

/* Reads the directory branch values
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_read_branch_values(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libfsxfs_block_tree_t *node_block_tree,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *entries_array,
     libcerror_error_t **error )
{
	static char *function      = "libfsxfs_directory_read_branch_values";
	size_t data_offset         = 0;
	size_t entries_data_size   = 0;
	uint32_t sub_block_number  = 0;
	uint16_t block_entry_index = 0;
	uint16_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit       = 0;
	uint16_t value_16bit       = 0;
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	data_offset = sizeof( fsxfs_file_system_block_header_v1_t );

	if( ( data_offset >= data_size )
	 || ( sizeof( fsxfs_directory_branch_block_header_v1_t ) > ( data_size - data_offset ) ) )
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
		 sizeof( fsxfs_directory_branch_block_header_v1_t ),
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_directory_branch_block_header_v1_t *) &( data[ data_offset ] ) )->number_of_entries,
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t: %" PRIu16 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_directory_branch_block_header_v1_t *) &( data[ data_offset ] ) )->node_level,
		 value_16bit );
		libcnotify_printf(
		 "%s: node level\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += sizeof( fsxfs_directory_branch_block_header_v1_t );

	entries_data_size = sizeof( fsxfs_directory_branch_block_entry_v1_t ) * number_of_entries;

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
		 "%s: branch entries data:\n",
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
		 ( (fsxfs_directory_branch_block_entry_v1_t *) &( data[ data_offset ] ) )->sub_block_number,
		 sub_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_directory_branch_block_entry_v1_t *) &( data[ data_offset ] ) )->name_hash,
			 value_32bit );
			libcnotify_printf(
			 "%s: name hash\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: sub block number\t\t\t: %" PRIu32 "\n",
			 function,
			 sub_block_number );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsxfs_directory_branch_block_entry_v1_t );

		if( libfsxfs_directory_read_from_block(
		     io_handle,
		     file_io_handle,
		     inode,
		     node_block_tree,
		     sub_block_number,
		     entries_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entries from directory block: %" PRIu32 ".",
			 function,
			 sub_block_number );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the directory leaf values
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_read_leaf_values(
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *entries_array,
     libcerror_error_t **error )
{
	libfsxfs_directory_entry_t *directory_entry               = NULL;
	libfsxfs_directory_leaf_block_header_t *leaf_block_header = NULL;
	static char *function                                     = "libfsxfs_directory_read_leaf_values";
	size_t data_offset                                        = 0;
	size_t entries_data_end_offset                            = 0;
	size_t entries_data_size                                  = 0;
	uint64_t inode_number                                     = 0;
	uint16_t block_entry_index                                = 0;
	uint16_t values_offset                                    = 0;
	uint16_t values_size                                      = 0;
	uint8_t name_size                                         = 0;
	int entry_index                                           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                                      = 0;
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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	data_offset = sizeof( fsxfs_file_system_block_header_v1_t );

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
	if( libfsxfs_directory_leaf_block_header_initialize(
	     &leaf_block_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize directory leaf block header.",
		 function );

		goto on_error;
	}
	if( libfsxfs_directory_leaf_block_header_read_data(
	     leaf_block_header,
	     &( data[ data_offset ] ),
	     data_size - data_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read directory leaf block header.",
		 function );

		goto on_error;
	}
	data_offset      += sizeof( fsxfs_directory_leaf_block_header_v1_t );
	entries_data_size = sizeof( fsxfs_directory_leaf_block_entry_v1_t ) * leaf_block_header->number_of_entries;

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
		 "%s: leaf block entries data:\n",
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
		 ( (fsxfs_directory_leaf_block_entry_v1_t *) &( data[ data_offset ] ) )->values_offset,
		 values_offset );

		name_size = ( (fsxfs_directory_leaf_block_entry_v1_t *) &( data[ data_offset ] ) )->name_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_directory_leaf_block_entry_v1_t *) &( data[ data_offset ] ) )->name_hash,
			 value_32bit );
			libcnotify_printf(
			 "%s: name hash\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: values offset\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 values_offset,
			 values_offset );

			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu8 "\n",
			 function,
			 name_size );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsxfs_directory_leaf_block_entry_v1_t *) &( data[ data_offset ] ) )->unknown1 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsxfs_directory_leaf_block_entry_v1_t );

		if( ( values_offset < entries_data_end_offset )
		 || ( values_offset >= data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid values offset value out of bounds (%zd < %zd <= %zd).",
			 function, entries_data_end_offset, values_offset, data_size );

			goto on_error;
		}
		values_size = 8 + (uint16_t) name_size;

		if( values_size > ( data_size - values_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid values size value out of bounds.",
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
			 values_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		byte_stream_copy_to_uint64_big_endian(
		 &( data[ values_offset ] ),
		 inode_number );

		values_offset += 8;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: inode number\t\t\t: %" PRIu64 "\n",
			 function,
			 inode_number );

			if( libfsxfs_debug_print_utf8_string_value(
			     function,
			     "name\t\t\t\t",
			     &( data[ values_offset ] ),
			     name_size,
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

		if( ( name_size == 1 )
		 && ( data[ values_offset ] == '.' ) )
		{
			/* Ignore self directory entry "." */
		}
		else if( ( name_size == 2 )
		      && ( data[ values_offset ] == '.' )
		      && ( data[ values_offset + 1 ] == '.' ) )
		{
			/* Ignore parent directory entry ".." */
		}
		else
		{
			if( libfsxfs_directory_entry_initialize(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create directory entry: %d.",
				 function,
				 block_entry_index );

				goto on_error;
			}
			directory_entry->inode_number = inode_number;

			if( memory_copy(
			     directory_entry->name,
			     &( data[ values_offset ] ),
			     (size_t) name_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy name.",
				 function );

				goto on_error;
			}
			directory_entry->name_size = name_size;

			if( libcdata_array_append_entry(
			     entries_array,
			     &entry_index,
			     (intptr_t *) directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append directory entry to entries array.",
				 function );

				goto on_error;
			}
			directory_entry = NULL;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	if( libfsxfs_directory_leaf_block_header_free(
	     &leaf_block_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free directory leaf block header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsxfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( leaf_block_header != NULL )
	{
		libfsxfs_directory_leaf_block_header_free(
		 &leaf_block_header,
		 NULL );
	}
	libcdata_array_empty(
	 entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Reads the directory entries from a directory block
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_read_from_block(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libfsxfs_block_tree_t *node_block_tree,
     uint32_t block_number,
     libcdata_array_t *entries_array,
     libcerror_error_t **error )
{
	libfsxfs_extent_t *extent                       = NULL;
	libfsxfs_file_system_block_t *file_system_block = NULL;
	static char *function                           = "libfsxfs_directory_read_from_block";
	off64_t block_offset                            = 0;
	uint64_t maximum_block_number                   = 0;
	uint64_t number_of_blocks                       = 0;
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
	if( libfsxfs_inode_get_number_of_data_extents(
	     inode,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data extents.",
		 function );

		goto on_error;
	}
/* TODO optimize this lookup */

	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libfsxfs_inode_get_data_extent_by_index(
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

	if( allocation_group_index > ( UINT64_MAX / io_handle->allocation_group_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid allocation group index value out of bounds.",
		 function );

		return( -1 );
	}
	block_offset = (off64_t) allocation_group_index * io_handle->allocation_group_size;

	maximum_block_number = UINT64_MAX / io_handle->block_size;

	if( relative_block_number > ( maximum_block_number - block_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid relative block number value out of bounds.",
		 function );

		return( -1 );
	}
	block_offset += relative_block_number;

	number_of_blocks = block_number - extent->logical_block_number;

	if( number_of_blocks > ( maximum_block_number - block_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of blocks value out of bounds.",
		 function );

		return( -1 );
	}
	block_offset = ( block_offset + number_of_blocks ) * io_handle->block_size;

	if( libfsxfs_block_tree_check_if_block_first_read(
	     node_block_tree,
	     (uint64_t) block_number,
	     block_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to check if first read of block number: %" PRIu32 ".",
		 function,
		 block_number );

		goto on_error;
	}
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
	if( file_system_block->header->signature == 0xfeeb )
	{
		if( libfsxfs_directory_read_leaf_values(
		     io_handle,
		     file_system_block->data,
		     file_system_block->data_size,
		     entries_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory leaf values.",
			 function );

			goto on_error;
		}
	}
	else if( file_system_block->header->signature == 0xfebe )
	{
		if( libfsxfs_directory_read_branch_values(
		     io_handle,
		     file_io_handle,
		     inode,
		     node_block_tree,
		     file_system_block->data,
		     file_system_block->data_size,
		     entries_array,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory branch values.",
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
	 entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Reads the directory entries from a block directory
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_read_from_block_directory(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libfsxfs_block_tree_t *node_block_tree,
     libcdata_array_t *entries_array,
     libcerror_error_t **error )
{
	libfsxfs_block_directory_t *block_directory = NULL;
	libfsxfs_extent_t *extent                   = NULL;
	static char *function                       = "libfsxfs_directory_read_from_block_directory";
	off64_t block_offset                        = 0;
	off64_t logical_offset                      = 0;
	size64_t extent_size                        = 0;
	int allocation_group_index                  = 0;
	int extent_index                            = 0;
	int number_of_extents                       = 0;
	uint64_t block_number                       = 0;
	uint64_t relative_block_number              = 0;

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
	if( io_handle->allocation_group_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - allocation group size value out of bounds.",
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
	if( libfsxfs_inode_get_number_of_data_extents(
	     inode,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data extents.",
		 function );

		goto on_error;
	}
	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libfsxfs_inode_get_data_extent_by_index(
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
		if( extent->logical_block_number > ( (uint64_t) INT64_MAX / io_handle->block_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid extent: %d - logical block number value out of bounds.",
			 function,
			 extent_index );

			goto on_error;
		}
		logical_offset = (off64_t) extent->logical_block_number * io_handle->block_size;

		if( logical_offset >= (off64_t) LIBFSXFS_DIRECTORY_LEAF_OFFSET )
		{
			break;
		}
		block_number           = extent->physical_block_number;
		allocation_group_index = (int) ( block_number >> io_handle->number_of_relative_block_number_bits );
		relative_block_number  = block_number & ( ( 1 << io_handle->number_of_relative_block_number_bits ) - 1 );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: extent: %d physical block number\t: %" PRIu64 "\n",
			 function,
			 extent_index,
			 block_number );

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

		block_offset = ( ( (off64_t) allocation_group_index * io_handle->allocation_group_size ) + relative_block_number ) * io_handle->block_size;
		extent_size  = (size64_t) extent->number_of_blocks * io_handle->block_size;

		while( extent_size > 0 )
		{
			if( libfsxfs_block_tree_check_if_block_first_read(
			     node_block_tree,
			     block_number,
			     block_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to check if first read of block number: %" PRIu64 ".",
				 function,
				 block_number );

				goto on_error;
			}
			if( libfsxfs_block_directory_initialize(
			     &block_directory,
			     io_handle->block_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize block directory.",
				 function );

				goto on_error;
			}
			if( libfsxfs_block_directory_read_file_io_handle(
			     block_directory,
			     io_handle,
			     file_io_handle,
			     block_offset,
			     entries_array,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read block directory.",
				 function );

				goto on_error;
			}
			if( libfsxfs_block_directory_free(
			     &block_directory,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free block directory.",
				 function );

				goto on_error;
			}
			block_number += 1;
			block_offset += io_handle->directory_block_size;
			extent_size  -= io_handle->directory_block_size;
		}
	}
	return( 1 );

on_error:
	if( block_directory != NULL )
	{
		libfsxfs_block_directory_free(
		 &block_directory,
		 NULL );
	}
	libcdata_array_empty(
	 entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Reads the directory entries
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_read_file_io_handle(
     libfsxfs_directory_t *directory,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libcerror_error_t **error )
{
	libfsxfs_block_tree_t *node_block_tree      = NULL;
	libfsxfs_directory_table_t *directory_table = NULL;
	static char *function                       = "libfsxfs_directory_read_file_io_handle";

	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
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
	if( ( inode->file_mode & 0xf000 ) == LIBFSXFS_FILE_TYPE_DIRECTORY )
	{
		if( ( inode->fork_type != LIBFSXFS_FORK_TYPE_INLINE_DATA )
		 && ( inode->fork_type != LIBFSXFS_FORK_TYPE_EXTENTS )
		 && ( inode->fork_type != LIBFSXFS_FORK_TYPE_BTREE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported directory data fork type.",
			 function );

			goto on_error;
		}
		if( inode->fork_type == LIBFSXFS_FORK_TYPE_INLINE_DATA )
		{
			if( libfsxfs_directory_table_initialize(
			     &directory_table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize directory table.",
				 function );

				goto on_error;
			}
			if( libfsxfs_directory_table_read_data(
			     directory_table,
			     io_handle,
			     inode->inline_data,
			     (size_t) inode->size,
			     directory->entries_array,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory table.",
				 function );

				goto on_error;
			}
			if( libfsxfs_directory_table_free(
			     &directory_table,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory table.",
				 function );

				goto on_error;
			}
		}
		else if( ( io_handle->feature_flags & LIBFSXFS_FEATURE_FLAG_DIRECTORY_V2 ) == 0 )
		{
			if( libfsxfs_block_tree_initialize(
			     &node_block_tree,
			     io_handle->volume_size,
			     io_handle->block_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create node block tree.",
				 function );

				goto on_error;
			}
			if( libfsxfs_directory_read_from_block(
			     io_handle,
			     file_io_handle,
			     inode,
			     node_block_tree,
			     0,
			     directory->entries_array,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve entries from directory block: 0.",
				 function );

				goto on_error;
			}
			if( libfsxfs_block_tree_free(
			     &node_block_tree,
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_block_descriptor_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free node block tree.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( libfsxfs_block_tree_initialize(
			     &node_block_tree,
			     io_handle->volume_size,
			     io_handle->directory_block_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create node block tree.",
				 function );

				goto on_error;
			}
			if( libfsxfs_directory_read_from_block_directory(
			     io_handle,
			     file_io_handle,
			     inode,
			     node_block_tree,
			     directory->entries_array,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read directory from a block directory.",
				 function );

				goto on_error;
			}
			if( libfsxfs_block_tree_free(
			     &node_block_tree,
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_block_descriptor_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free node block tree.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( node_block_tree != NULL )
	{
		libfsxfs_block_tree_free(
		 &node_block_tree,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_block_descriptor_free,
		 NULL );
	}
	if( directory_table != NULL )
	{
		libfsxfs_directory_table_free(
		 &directory_table,
		 NULL );
	}
	libcdata_array_empty(
	 directory->entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Retrieves the number of entries
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_get_number_of_entries(
     libfsxfs_directory_t *directory,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_get_number_of_entries";

	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     directory->entries_array,
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific entry
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_get_entry_by_index(
     libfsxfs_directory_t *directory,
     int entry_index,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_get_entry_by_index";

	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     directory->entries_array,
	     entry_index,
	     (intptr_t **) directory_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d.",
		 function,
		 entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the directory entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsxfs_directory_get_entry_by_utf8_name(
     libfsxfs_directory_t *directory,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfsxfs_directory_entry_t *safe_directory_entry = NULL;
	static char *function                            = "libfsxfs_directory_get_entry_by_utf8_name";
	int entry_index                                  = 0;
	int number_of_entries                            = 0;
	int result                                       = 0;

	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     directory->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     directory->entries_array,
		     entry_index,
		     (intptr_t **) &safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		result = libfsxfs_directory_entry_compare_with_utf8_string(
		          safe_directory_entry,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with directory entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			*directory_entry = safe_directory_entry;

			return( 1 );
		}
	}
	*directory_entry = NULL;

	return( 0 );
}

/* Retrieves the directory entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsxfs_directory_get_entry_by_utf16_name(
     libfsxfs_directory_t *directory,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfsxfs_directory_entry_t *safe_directory_entry = NULL;
	static char *function                            = "libfsxfs_directory_get_entry_by_utf16_name";
	int entry_index                                  = 0;
	int number_of_entries                            = 0;
	int result                                       = 0;

	if( directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     directory->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     directory->entries_array,
		     entry_index,
		     (intptr_t **) &safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		result = libfsxfs_directory_entry_compare_with_utf16_string(
		          safe_directory_entry,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with directory entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			*directory_entry = safe_directory_entry;

			return( 1 );
		}
	}
	*directory_entry = NULL;

	return( 0 );
}

