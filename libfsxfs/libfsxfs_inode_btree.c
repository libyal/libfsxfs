/*
 * Inode B+ tree functions
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
#include "libfsxfs_btree_header.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_inode_btree.h"
#include "libfsxfs_inode_btree_record.h"
#include "libfsxfs_inode_information.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

/* Creates an inode B+ tree
 * Make sure the value inode_btree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_initialize(
     libfsxfs_inode_btree_t **inode_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_btree_initialize";

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
		 function );

		return( -1 );
	}
	if( *inode_btree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid inode B+ tree value already set.",
		 function );

		return( -1 );
	}
	*inode_btree = memory_allocate_structure(
	                libfsxfs_inode_btree_t );

	if( *inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create inode B+ tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *inode_btree,
	     0,
	     sizeof( libfsxfs_inode_btree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear inode B+ tree.",
		 function );

		memory_free(
		 *inode_btree );

		*inode_btree = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *inode_btree )->inode_information_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create inode information array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *inode_btree != NULL )
	{
		memory_free(
		 *inode_btree );

		*inode_btree = NULL;
	}
	return( -1 );
}

/* Frees an inode B+ tree
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_free(
     libfsxfs_inode_btree_t **inode_btree,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_inode_btree_free";
	int result            = 1;

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
		 function );

		return( -1 );
	}
	if( *inode_btree != NULL )
	{
		if( libcdata_array_free(
		     &( ( *inode_btree )->inode_information_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_inode_information_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free inode information array.",
			 function );

			result = -1;
		}
		memory_free(
		 *inode_btree );

		*inode_btree = NULL;
	}
	return( result );
}

/* Reads the inode information
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_read_inode_information(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfsxfs_inode_information_t *inode_information = NULL;
	static char *function                           = "libfsxfs_file_system_read_inode_information";
	int entry_index                                 = 0;

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
		 function );

		return( -1 );
	}
	if( libfsxfs_inode_information_initialize(
	     &inode_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create inode information.",
		 function );

		goto on_error;
	}
	if( libfsxfs_inode_information_read_file_io_handle(
	     inode_information,
	     io_handle,
	     file_io_handle,
	     file_offset,
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

		goto on_error;
	}
	if( libcdata_array_append_entry(
	     inode_btree->inode_information_array,
	     &entry_index,
	     (intptr_t *) inode_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append inode information to array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( inode_information != NULL )
	{
		libfsxfs_inode_information_free(
		 &inode_information,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the inode from the inode B+ tree branch node
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_get_inode_from_branch_node(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t allocation_group_block_number,
     uint16_t number_of_records,
     const uint8_t *records_data,
     size_t records_data_size,
     uint64_t relative_inode_number,
     int recursion_depth,
     libcerror_error_t **error )
{
	static char *function              = "libfsxfs_inode_btree_get_inode_from_branch_node";
	size_t number_of_key_value_pairs   = 0;
	size_t records_data_offset         = 0;
	uint32_t relative_key_inode_number = 0;
	uint32_t relative_sub_block_number = 0;
	uint16_t record_index              = 0;
	int result                         = 0;

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
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
	number_of_key_value_pairs = records_data_size / 8;

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
		byte_stream_copy_to_uint32_big_endian(
		 &( records_data[ records_data_offset ] ),
		 relative_key_inode_number );

		records_data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: inode number\t\t: %" PRIu32 "\n",
			 function,
			 relative_key_inode_number );
		}
#endif
		if( relative_inode_number < relative_key_inode_number )
		{
			break;
		}
	}
	if( ( record_index > 0 )
	 && ( record_index <= number_of_records ) )
	{
		records_data_offset = ( number_of_key_value_pairs + record_index - 1 ) * 4;

		byte_stream_copy_to_uint32_big_endian(
		 &( records_data[ records_data_offset ] ),
		 relative_sub_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: sub block number\t: %" PRIu32 "\n",
			 function,
			 relative_sub_block_number );

			libcnotify_printf(
			 "\n" );
		}
#endif
		result = libfsxfs_inode_btree_get_inode_from_node(
		          inode_btree,
		          io_handle,
		          file_io_handle,
		          allocation_group_block_number,
		          relative_sub_block_number,
		          relative_inode_number,
		          recursion_depth + 1,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve inode from node.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( result );
}

/* Retrieves the inode from the inode B+ tree leaf node
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_get_inode_from_leaf_node(
     libfsxfs_inode_btree_t *inode_btree,
     uint16_t number_of_records,
     const uint8_t *records_data,
     size_t records_data_size,
     uint64_t inode_number,
     libcerror_error_t **error )
{
	libfsxfs_inode_btree_record_t *inode_btree_record = NULL;
	static char *function                             = "libfsxfs_inode_btree_get_inode_from_leaf_node";
	size_t records_data_offset                        = 0;
	uint16_t record_index                             = 0;
	int result                                        = 0;

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
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
	if( (size_t) number_of_records > ( records_data_size / 16 ) )
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
		if( libfsxfs_inode_btree_record_initialize(
		     &inode_btree_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create inode B+ tree record: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( libfsxfs_inode_btree_record_read_data(
		     inode_btree_record,
		     &( records_data[ records_data_offset ] ),
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read inode B+ tree record: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		records_data_offset += 16;

		if( ( inode_number >= inode_btree_record->inode_number )
		 && ( inode_number < ( inode_btree_record->inode_number + 64 ) ) )
		{
/* TODO check bitmap */
			result = 1;
		}
/* TODO cache records in block */
		if( libfsxfs_inode_btree_record_free(
		     &inode_btree_record,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free inode B+ tree record: %" PRIu16 ".",
			 function,
			 record_index );

			goto on_error;
		}
		if( result != 0 )
		{
			break;
		}
	}
	return( result );

on_error:
	if( inode_btree_record != NULL )
	{
		libfsxfs_inode_btree_record_free(
		 &inode_btree_record,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the inode from the inode B+ tree node
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_inode_btree_get_inode_from_node(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t allocation_group_block_number,
     uint64_t relative_block_number,
     uint64_t relative_inode_number,
     int recursion_depth,
     libcerror_error_t **error )
{
	libfsxfs_btree_block_t *btree_block = NULL;
	static char *function               = "libfsxfs_inode_btree_get_inode_from_node";
	off64_t btree_block_offset          = 0;
	int compare_result                  = 0;
	int result                          = 0;

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
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
	if( allocation_group_block_number > (uint64_t) ( INT64_MAX / io_handle->block_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid allocation group block number value out of bounds.",
		 function );

		return( -1 );
	}
	if( relative_block_number > ( (uint64_t) ( INT64_MAX / io_handle->block_size ) - allocation_group_block_number ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid relative block number value out of bounds.",
		 function );

		return( -1 );
	}
	btree_block_offset = ( allocation_group_block_number + relative_block_number ) * io_handle->block_size;

	if( libfsxfs_btree_block_initialize(
	     &btree_block,
	     io_handle->block_size,
	     4,
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
		 "%s: unable to read inode B+ tree block: %" PRIu32 " at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 relative_block_number,
		 btree_block_offset,
		 btree_block_offset );

		goto on_error;
	}
	if( io_handle->format_version == 5 )
	{
		compare_result = memory_compare(
		                  btree_block->header->signature,
		                  "IAB3",
		                  4 );
	}
	else
	{
		compare_result = memory_compare(
		                  btree_block->header->signature,
		                  "IABT",
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
/* TODO
	if( btree_block->header->level > inode_btree->maximum_depth )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported B+ tree node level.",
		 function );

		goto on_error;
	}
*/
	if( btree_block->header->level == 0 )
	{
		result = libfsxfs_inode_btree_get_inode_from_leaf_node(
		          inode_btree,
		          btree_block->header->number_of_records,
		          btree_block->records_data,
		          btree_block->records_data_size,
		          relative_inode_number,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve inode from leaf node.",
			 function );

			goto on_error;
		}
	}
	else
	{
		result = libfsxfs_inode_btree_get_inode_from_branch_node(
		          inode_btree,
		          io_handle,
		          file_io_handle,
		          allocation_group_block_number,
		          btree_block->header->number_of_records,
		          btree_block->records_data,
		          btree_block->records_data_size,
		          relative_inode_number,
		          recursion_depth,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve inode from branch node.",
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
	return( result );

on_error:
	if( btree_block != NULL )
	{
		libfsxfs_btree_block_free(
		 &btree_block,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a specific inode from the inode B+ tree
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfsxfs_inode_btree_get_inode_by_number(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t absolute_inode_number,
     off64_t *file_offset,
     libcerror_error_t **error )
{
	libfsxfs_inode_information_t *inode_information = NULL;
	static char *function                           = "libfsxfs_inode_btree_get_inode_by_number";
	uint64_t allocation_group_block_number          = 0;
	uint64_t relative_inode_number                  = 0;
	int allocation_group_index                      = 0;
	int result                                      = 0;

	if( inode_btree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode B+ tree.",
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
	if( file_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file offset.",
		 function );

		return( -1 );
	}
	allocation_group_index = (int) ( absolute_inode_number >> io_handle->number_of_relative_inode_number_bits );
	relative_inode_number  = absolute_inode_number & ( ( (uint64_t) 1 << io_handle->number_of_relative_inode_number_bits ) - 1 );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: allocation group index\t: %d\n",
		 function,
		 allocation_group_index );

		libcnotify_printf(
		 "%s: relative inode number\t\t: %" PRIu64 "\n",
		 function,
		 relative_inode_number );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libcdata_array_get_entry_by_index(
	     inode_btree->inode_information_array,
	     allocation_group_index,
	     (intptr_t **) &inode_information,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode information: %d.",
		 function,
		 allocation_group_index );

		return( -1 );
	}
	allocation_group_block_number = (uint64_t) allocation_group_index * io_handle->allocation_group_size;

	if( inode_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing inode information.",
		 function );

		return( -1 );
	}
	result = libfsxfs_inode_btree_get_inode_from_node(
	          inode_btree,
	          io_handle,
	          file_io_handle,
	          allocation_group_block_number,
	          inode_information->inode_btree_root_block_number,
	          relative_inode_number,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode: %" PRIu64 " from root node: %" PRIu32 ".",
		 function,
		 relative_inode_number,
		 inode_information->inode_btree_root_block_number );

		return( -1 );
	}
	else if( result != 0 )
	{
		*file_offset = ( (off64_t) allocation_group_block_number * io_handle->block_size ) + ( (off64_t) relative_inode_number * io_handle->inode_size );
	}
	return( result );
}

