/*
 * Extent B+ tree functions
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

#include "libfsxfs_btree_block.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_extent.h"
#include "libfsxfs_extent_btree.h"
#include "libfsxfs_extents.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

/* Creates an extent B+ tree
 * Make sure the value extent_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_extent_btree_initialize(
     libfsxfs_extent_btree_t **extent_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_extent_btree_initialize";

	if( extent_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent B+ tree.",
		 function );

		return( -1 );
	}
	if( *extent_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extent B+ tree value already set.",
		 function );

		return( -1 );
	}
	*extent_btree = memory_allocate_structure(
	                 libfsxfs_extent_btree_t );

	if( *extent_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create extent B+ tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *extent_btree,
	     0,
	     sizeof( libfsxfs_extent_btree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear extent B+ tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *extent_btree != NULL )
	{
		memory_free(
		 *extent_btree );

		*extent_btree = NULL;
	}
	return( -1 );
}

/* Frees an extent B+ tree
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_extent_btree_free(
     libfsxfs_extent_btree_t **extent_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_extent_btree_free";

	if( extent_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent B+ tree.",
		 function );

		return( -1 );
	}
	if( *extent_btree != NULL )
	{
		memory_free(
		 *extent_btree );

		*extent_btree = NULL;
	}
	return( 1 );
}

/* Retrieves the extents from the extent B+ tree root node
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_extent_btree_get_extents_from_root_node(
     libfsxfs_extent_btree_t *extent_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t number_of_blocks,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extents_array,
     uint8_t add_sparse_extents,
     libcerror_error_t **error )
{
	libfsxfs_extent_t *last_extent   = NULL;
	libfsxfs_extent_t *sparse_extent = NULL;
	static char *function            = "libfsxfs_extent_btree_get_extents_from_root_node";
	uint64_t logical_block_number    = 0;
	uint16_t level                   = 0;
	uint16_t number_of_records       = 0;
	int entry_index                  = 0;

	if( extent_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent B+ tree.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extents B+ tree root node data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint16_big_endian(
	 &( data[ 0 ] ),
	 level );

	byte_stream_copy_to_uint16_big_endian(
	 &( data[ 2 ] ),
	 number_of_records );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: node level\t\t: %" PRIu16 "\n",
		 function,
		 level );

		libcnotify_printf(
		 "%s: number of records\t: %" PRIu16 "\n",
		 function,
		 number_of_records );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( level == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported B+ tree root node level.",
		 function );

		goto on_error;
	}
	if( libfsxfs_extent_btree_get_extents_from_branch_node(
	     extent_btree,
	     io_handle,
	     file_io_handle,
	     number_of_records,
	     &( data[ 4 ] ),
	     data_size - 4,
	     extents_array,
	     add_sparse_extents,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extents from root node.",
		 function );

		goto on_error;
	}
	if( libfsxfs_extents_get_last_extent(
	     extents_array,
	     &last_extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve last extent.",
		 function );

		goto on_error;
	}
	if( last_extent != NULL )
	{
		logical_block_number = last_extent->logical_block_number + last_extent->number_of_blocks;
	}
	if( ( add_sparse_extents != 0 )
	 && ( logical_block_number < number_of_blocks ) )
	{
		if( ( last_extent == NULL )
		 || ( ( last_extent->range_flags & LIBFSXFS_EXTENT_FLAG_IS_SPARSE ) == 0 ) )
		{
			if( libfsxfs_extent_initialize(
			     &sparse_extent,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sparse extent.",
				 function );

				goto on_error;
			}
			sparse_extent->logical_block_number = logical_block_number;
			sparse_extent->range_flags          = LIBFSXFS_EXTENT_FLAG_IS_SPARSE;

			if( libcdata_array_append_entry(
			     extents_array,
			     &entry_index,
			     (intptr_t *) sparse_extent,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append sparse extent to array.",
				 function );

				goto on_error;
			}
			last_extent   = sparse_extent;
			sparse_extent = NULL;
		}
		last_extent->number_of_blocks += number_of_blocks - logical_block_number;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: logical block number\t\t\t: %" PRIu64 "\n",
			 function,
			 last_extent->logical_block_number );

			libcnotify_printf(
			 "%s: physical block number\t\t\t: %" PRIu64 "\n",
			 function,
			 last_extent->physical_block_number );

			libcnotify_printf(
			 "%s: number of blocks\t\t\t: %" PRIu64 "\n",
			 function,
			 last_extent->number_of_blocks );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	return( 1 );

on_error:
	if( sparse_extent != NULL )
	{
		libfsxfs_extent_free(
		 &sparse_extent,
		 NULL );
	}
	libcdata_array_empty(
	 extents_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
	 NULL );

	return( -1 );
}

/* Retrieves the extents from the extent B+ tree branch node
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_extent_btree_get_extents_from_branch_node(
     libfsxfs_extent_btree_t *extent_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t number_of_records,
     const uint8_t *records_data,
     size_t records_data_size,
     libcdata_array_t *extents_array,
     uint8_t add_sparse_extents,
     int recursion_depth,
     libcerror_error_t **error )
{
	static char *function            = "libfsxfs_extent_btree_get_extents_from_branch_node";
	size_t number_of_key_value_pairs = 0;
	size_t records_data_offset       = 0;
	uint64_t sub_block_number        = 0;
	uint16_t record_index            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit             = 0;
#endif

	if( extent_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent B+ tree.",
		 function );

		return( -1 );
	}
	if( records_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid records data.",
		 function );

		return( -1 );
	}
	if( ( records_data_size == 0 )
	 || ( records_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid records data size value out of bounds.",
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
	number_of_key_value_pairs = records_data_size / 16;

	if( (size_t) number_of_records > number_of_key_value_pairs )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of records value out of bounds.",
		 function );

		return( -1 );
	}
	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_big_endian(
			 &( records_data[ records_data_offset ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: key: %" PRIu16 "\t\t: %" PRIu64 "\n",
			 function,
			 record_index,
			 value_64bit );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		records_data_offset += 8;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	records_data_offset = number_of_key_value_pairs * 8;

	for( record_index = 0;
	     record_index < number_of_records;
	     record_index++ )
	{
		byte_stream_copy_to_uint64_big_endian(
		 &( records_data[ records_data_offset ] ),
		 sub_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value: %" PRIu16 "\t\t: %" PRIu64 "\n",
			 function,
			 record_index,
			 sub_block_number );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		records_data_offset += 8;

		if( libfsxfs_extent_btree_get_extents_from_node(
		     extent_btree,
		     io_handle,
		     file_io_handle,
		     sub_block_number,
		     extents_array,
		     add_sparse_extents,
		     recursion_depth + 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extents from branch node.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	libcdata_array_empty(
	 extents_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
	 NULL );

	return( -1 );
}

/* Retrieves the extents from the extent B+ tree node
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_extent_btree_get_extents_from_node(
     libfsxfs_extent_btree_t *extent_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t block_number,
     libcdata_array_t *extents_array,
     uint8_t add_sparse_extents,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsxfs_btree_block_t *btree_block = NULL;
	static char *function               = "libfsxfs_extent_btree_get_exents_from_node";
	off64_t btree_block_offset          = 0;
	uint64_t relative_block_number      = 0;
	int allocation_group_index          = 0;
	int compare_result                  = 0;

	if( extent_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extent B+ tree.",
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
	allocation_group_index = (int) ( block_number >> io_handle->number_of_relative_block_number_bits );
	relative_block_number  = block_number & ( ( 1 << io_handle->number_of_relative_block_number_bits ) - 1 );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: allocation group index\t: %d\n",
		 function,
		 allocation_group_index );

		libcnotify_printf(
		 "%s: relative block number\t: %" PRIu64 "\n",
		 function,
		 relative_block_number );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	btree_block_offset = ( ( (off64_t) allocation_group_index * io_handle->allocation_group_size ) + relative_block_number ) * io_handle->block_size;

	if( libfsxfs_btree_block_initialize(
	     &btree_block,
	     io_handle->block_size,
	     8,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create B+ tree block.",
		 function );

		goto on_error;
	}
	if( libfsxfs_btree_block_read_file_io_handle(
	     btree_block,
	     io_handle,
	     file_io_handle,
	     btree_block_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read extent B+ tree block: %" PRIu32 " at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 block_number,
		 btree_block_offset,
		 btree_block_offset );

		goto on_error;
	}
	if( io_handle->format_version == 5 )
	{
		compare_result = memory_compare(
		                  btree_block->header->signature,
		                  "BMA3",
		                  4 );
	}
	else
	{
		compare_result = memory_compare(
		                  btree_block->header->signature,
		                  "BMAP",
		                  4 );
	}
	if( compare_result != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block signature.",
		 function );

		goto on_error;
	}
	if( btree_block->header->level > extent_btree->maximum_depth )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported B+ tree node level.",
		 function );

		goto on_error;
	}
	if( btree_block->header->level == 0 )
	{
		if( libfsxfs_extents_read_data(
		     extents_array,
		     (uint32_t) btree_block->header->number_of_records,
		     btree_block->records_data,
		     btree_block->records_data_size,
		     add_sparse_extents,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data extents.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfsxfs_extent_btree_get_extents_from_branch_node(
		     extent_btree,
		     io_handle,
		     file_io_handle,
		     btree_block->header->number_of_records,
		     btree_block->records_data,
		     btree_block->records_data_size,
		     extents_array,
		     add_sparse_extents,
		     recursion_depth,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extents from branch node.",
			 function );

			goto on_error;
		}
	}
	if( libfsxfs_btree_block_free(
	     &btree_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free B+ tree block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( btree_block != NULL )
	{
		libfsxfs_btree_block_free(
		 &btree_block,
		 NULL );
	}
	libcdata_array_empty(
	 extents_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
	 NULL );

	return( -1 );
}

