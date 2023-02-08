/*
 * Extents functions
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
#include <memory.h>
#include <types.h>

#include "libfsxfs_definitions.h"
#include "libfsxfs_extent.h"
#include "libfsxfs_extents.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

/* Retrieves the last extent
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_extents_get_last_extent(
     libcdata_array_t *extents_array,
     libfsxfs_extent_t **last_extent,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_extents_get_last_extent";
	int number_of_extents = 0;

	if( last_extent == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid last extent.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     extents_array,
	     &number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of extents.",
		 function );

		return( -1 );
	}
	if( number_of_extents == 0 )
	{
		*last_extent = NULL;
	}
	else if( libcdata_array_get_entry_by_index(
	          extents_array,
	          number_of_extents - 1,
	          (intptr_t **) last_extent,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve extent: %d.",
		 function,
		 number_of_extents - 1 );

		return( -1 );
	}
	return( 1 );
}

/* Reads the extent list data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_extents_read_data(
     libcdata_array_t *extents_array,
     uint32_t number_of_extents,
     const uint8_t *data,
     size_t data_size,
     uint8_t add_sparse_extents,
     libcerror_error_t **error )
{
	libfsxfs_extent_t *last_extent   = NULL;
	libfsxfs_extent_t *sparse_extent = NULL;
	libfsxfs_extent_t *extent        = NULL;
	static char *function            = "libfsxfs_extents_read_data";
	size_t data_offset               = 0;
	uint64_t logical_block_number    = 0;
	uint32_t extent_index            = 0;
	int entry_index                  = 0;

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
	if( ( data_size == 0 )
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
	if( (size_t) number_of_extents > ( data_size / 16 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of extents value out of bounds.",
		 function );

		return( -1 );
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

		logical_block_number = last_extent->logical_block_number + last_extent->number_of_blocks;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: extent list data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 (size_t) number_of_extents * 16,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	for( extent_index = 0;
	     extent_index < number_of_extents;
	     extent_index++ )
	{
		if( libfsxfs_extent_initialize(
		     &extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extent: %" PRIu32 ".",
			 function,
			 extent_index );

			goto on_error;
		}
		if( libfsxfs_extent_read_data(
		     extent,
		     &( data[ data_offset ] ),
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read extent: %" PRIu32 ".",
			 function,
			 extent_index );

			goto on_error;
		}
		data_offset += 16;

		if( ( add_sparse_extents != 0 )
		 && ( extent->logical_block_number > logical_block_number ) )
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
			sparse_extent->number_of_blocks     = extent->logical_block_number - logical_block_number;
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
			sparse_extent = NULL;
		}
		logical_block_number = extent->logical_block_number + extent->number_of_blocks;

		if( libcdata_array_append_entry(
		     extents_array,
		     &entry_index,
		     (intptr_t *) extent,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append extent: %" PRIu32 " to extents array.",
			 function,
			 extent_index );

			goto on_error;
		}
		extent = NULL;
	}
	return( 1 );

on_error:
	if( sparse_extent != NULL )
	{
		libfsxfs_extent_free(
		 &sparse_extent,
		 NULL );
	}
	if( extent != NULL )
	{
		libfsxfs_extent_free(
		 &extent,
		 NULL );
	}
	libcdata_array_empty(
	 extents_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_extent_free,
	 NULL );

	return( -1 );
}

