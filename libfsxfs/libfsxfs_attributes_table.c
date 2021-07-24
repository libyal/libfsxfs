/*
 * Short-form (extended) attributes table functions
 *
 * Copyright (C) 2020-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfsxfs_attributes_table.h"
#include "libfsxfs_attributes_table_header.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_attributes.h"

/* Creates attributes table
 * Make sure the value attributes_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_table_initialize(
     libfsxfs_attributes_table_t **attributes_table,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_table_initialize";

	if( attributes_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes table.",
		 function );

		return( -1 );
	}
	if( *attributes_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes table value already set.",
		 function );

		return( -1 );
	}
	*attributes_table = memory_allocate_structure(
	                     libfsxfs_attributes_table_t );

	if( *attributes_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attributes table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attributes_table,
	     0,
	     sizeof( libfsxfs_attributes_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attributes table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attributes_table != NULL )
	{
		memory_free(
		 *attributes_table );

		*attributes_table = NULL;
	}
	return( -1 );
}

/* Frees attributes table
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_table_free(
     libfsxfs_attributes_table_t **attributes_table,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_table_free";
	int result            = 1;

	if( attributes_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes table.",
		 function );

		return( -1 );
	}
	if( *attributes_table != NULL )
	{
		if( ( *attributes_table )->header != NULL )
		{
			if( libfsxfs_attributes_table_header_free(
			     &( ( *attributes_table )->header ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free header.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *attributes_table );

		*attributes_table = NULL;
	}
	return( result );
}

/* Reads the attributes table
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_table_read_data(
     libfsxfs_attributes_table_t *attributes_table,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extended_attributes_array,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values = NULL;
	static char *function                         = "libfsxfs_attributes_table_read_data";
	size_t data_offset                            = 0;
	uint32_t attribute_index                      = 0;
	int entry_index                               = 0;

	if( attributes_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes table.",
		 function );

		return( -1 );
	}
	if( attributes_table->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes table - header already set.",
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
	if( ( data_size < 2 )
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
	if( libfsxfs_attributes_table_header_initialize(
	     &( attributes_table->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create attributes table header.",
		 function );

		goto on_error;
	}
	if( libfsxfs_attributes_table_header_read_data(
	     attributes_table->header,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attributes table header.",
		 function );

		goto on_error;
	}
	if( ( attributes_table->header->data_size < sizeof( fsxfs_attributes_short_form_header_t ) )
	 || ( attributes_table->header->data_size > data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid attributes table header - data size value out of bounds.",
		 function );

		return( -1 );
	}
	data_offset = sizeof( fsxfs_attributes_short_form_header_t );

	for( attribute_index = 0;
	     attribute_index < attributes_table->header->number_of_entries;
	     attribute_index++ )
	{
		if( libfsxfs_attribute_values_initialize(
		     &attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create attribute values.",
			 function );

			goto on_error;
		}
		if( libfsxfs_attribute_values_read_data(
		     attribute_values,
		     &( data[ data_offset ] ),
		     data_size - data_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read attribute: %d values.",
			 function,
			 attribute_index );

			goto on_error;
		}
		data_offset += sizeof( fsxfs_attributes_short_form_entry_t ) + attribute_values->name_data_size;

/* TODO add value data bounds check */

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 attribute_values->value_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( attribute_values->value_data_size > 0 )
		{
			attribute_values->value_data = (uint8_t *) memory_allocate(
			                                            sizeof( uint8_t ) * attribute_values->value_data_size );

			if( attribute_values->value_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value data.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     attribute_values->value_data,
			     &( data[ data_offset ] ),
			     attribute_values->value_data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy value data to attribute: %d values.",
				 function,
				 attribute_index );

				goto on_error;
			}
			data_offset += attribute_values->value_data_size;
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
			 "%s: unable to append attribute: %d values to extended attributes array.",
			 function,
			 attribute_index );

			goto on_error;
		}
		attribute_values = NULL;
	}
	return( 1 );

on_error:
	if( attribute_values != NULL )
	{
		libfsxfs_attribute_values_free(
		 &attribute_values,
		 NULL );
	}
	if( attributes_table->header != NULL )
	{
		libfsxfs_attributes_table_header_free(
		 &( attributes_table->header ),
		 NULL );
	}
	libcdata_array_empty(
	 extended_attributes_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_attribute_values_free,
	 NULL );

	return( -1 );
}

