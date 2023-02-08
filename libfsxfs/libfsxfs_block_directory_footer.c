/*
 * Block directory footer functions
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

#include "libfsxfs_block_directory_footer.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"

#include "fsxfs_block_directory.h"

/* Creates a block_directory_footer
 * Make sure the value block_directory_footer is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_footer_initialize(
     libfsxfs_block_directory_footer_t **block_directory_footer,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_footer_initialize";

	if( block_directory_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory footer.",
		 function );

		return( -1 );
	}
	if( *block_directory_footer != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block directory footer value already set.",
		 function );

		return( -1 );
	}
	*block_directory_footer = memory_allocate_structure(
	                           libfsxfs_block_directory_footer_t );

	if( *block_directory_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block directory footer.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_directory_footer,
	     0,
	     sizeof( libfsxfs_block_directory_footer_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block directory footer.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *block_directory_footer != NULL )
	{
		memory_free(
		 *block_directory_footer );

		*block_directory_footer = NULL;
	}
	return( -1 );
}

/* Frees a block_directory_footer
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_footer_free(
     libfsxfs_block_directory_footer_t **block_directory_footer,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_footer_free";

	if( block_directory_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory footer.",
		 function );

		return( -1 );
	}
	if( *block_directory_footer != NULL )
	{
		memory_free(
		 *block_directory_footer );

		*block_directory_footer = NULL;
	}
	return( 1 );
}

/* Reads the block_directory_footer data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_footer_read_data(
     libfsxfs_block_directory_footer_t *block_directory_footer,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_footer_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( block_directory_footer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory footer.",
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
	if( ( data_size < 8 )
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
		 "%s: block directory footer data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 8,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint32_big_endian(
	 &( data[ 0 ] ),
	 block_directory_footer->number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of directory entries\t: %" PRIu32 "\n",
		 function,
		 block_directory_footer->number_of_entries );

		byte_stream_copy_to_uint32_big_endian(
		 &( data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: number of unused entries\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

