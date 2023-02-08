/*
 * Short-form (extended) attributes table functions
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
	uint8_t flags                                 = 0;
	uint8_t name_size                             = 0;
	uint8_t value_data_size                       = 0;
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

	if( data_size < sizeof( fsxfs_attributes_short_form_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < attributes_table->header->number_of_entries;
	     attribute_index++ )
	{
		if( data_offset > ( data_size - sizeof( fsxfs_attributes_short_form_entry_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid attribute: %d entry data size value out of bounds.",
			 function,
			 attribute_index );

			return( -1 );
		}
		name_size = ( (fsxfs_attributes_short_form_entry_t *) &( data[ data_offset ] ) )->name_size;

		if( name_size > ( data_size - sizeof( fsxfs_attributes_short_form_entry_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: attribute: %d entry data:\n",
			 function,
			 attribute_index );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 sizeof( fsxfs_attributes_short_form_entry_t ) + name_size,
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
			 "%s: unable to create attribute values.",
			 function );

			goto on_error;
		}
		value_data_size = ( (fsxfs_attributes_short_form_entry_t *) &( data[ data_offset ] ) )->value_data_size;

		flags = ( (fsxfs_attributes_short_form_entry_t *) &( data[ data_offset ] ) )->flags;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu8 "\n",
			 function,
			 name_size );

			libcnotify_printf(
			 "%s: value data size\t\t\t: %" PRIu8 "\n",
			 function,
			 value_data_size );

			libcnotify_printf(
			 "%s: flags\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 flags );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsxfs_attributes_short_form_entry_t );

		if( name_size > 0 )
		{
			if( libfsxfs_attribute_values_set_name(
			     attribute_values,
			     &( data[ data_offset ] ),
			     (size_t) name_size,
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
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += name_size;

		if( value_data_size > ( data_size - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
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
			 &( data[ data_offset ] ),
			 (size_t) value_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( value_data_size > 0 )
		{
			if( libfsxfs_attribute_values_set_value_data(
			     attribute_values,
			     &( data[ data_offset ] ),
			     (size_t) value_data_size,
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
			data_offset += value_data_size;
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

