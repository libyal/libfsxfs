/*
 * Short-form (extended) attributes table header functions
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

#include "libfsxfs_attributes_table_header.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_attributes.h"

/* Creates a attributes_table_header
 * Make sure the value attributes_table_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_table_header_initialize(
     libfsxfs_attributes_table_header_t **attributes_table_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_table_header_initialize";

	if( attributes_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes table header.",
		 function );

		return( -1 );
	}
	if( *attributes_table_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes table header value already set.",
		 function );

		return( -1 );
	}
	*attributes_table_header = memory_allocate_structure(
	                            libfsxfs_attributes_table_header_t );

	if( *attributes_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attributes table header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attributes_table_header,
	     0,
	     sizeof( libfsxfs_attributes_table_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attributes table header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attributes_table_header != NULL )
	{
		memory_free(
		 *attributes_table_header );

		*attributes_table_header = NULL;
	}
	return( -1 );
}

/* Frees a attributes_table_header
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_table_header_free(
     libfsxfs_attributes_table_header_t **attributes_table_header,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_table_header_free";

	if( attributes_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes table header.",
		 function );

		return( -1 );
	}
	if( *attributes_table_header != NULL )
	{
		memory_free(
		 *attributes_table_header );

		*attributes_table_header = NULL;
	}
	return( 1 );
}

/* Reads the attributes_table_header data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_table_header_read_data(
     libfsxfs_attributes_table_header_t *attributes_table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_table_header_read_data";

	if( attributes_table_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes table header.",
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
	if( ( data_size < sizeof( fsxfs_attributes_short_form_header_t ) )
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
		 "%s: attributes table header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsxfs_attributes_short_form_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_attributes_short_form_header_t *) data )->data_size,
	 attributes_table_header->data_size );

	attributes_table_header->number_of_entries = ( (fsxfs_attributes_short_form_header_t *) data )->number_of_entries;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data size\t\t\t: %" PRIu16 "\n",
		 function,
		 attributes_table_header->data_size );

		libcnotify_printf(
		 "%s: number of entries\t\t: %" PRIu8 "\n",
		 function,
		 attributes_table_header->number_of_entries );

		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fsxfs_attributes_short_form_header_t *) data )->unknown1 );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

