/*
 * Extended attribute values functions
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
#include <narrow_string.h>
#include <types.h>

#include "libfsxfs_attribute_values.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libuna.h"

#include "fsxfs_attributes.h"

/* Creates a attribute values
 * Make sure the value attribute_values is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_initialize(
     libfsxfs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_initialize";

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
	if( *attribute_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values value already set.",
		 function );

		return( -1 );
	}
	*attribute_values = memory_allocate_structure(
	                     libfsxfs_attribute_values_t );

	if( *attribute_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attribute_values,
	     0,
	     sizeof( libfsxfs_attribute_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *attribute_values != NULL )
	{
		memory_free(
		 *attribute_values );

		*attribute_values = NULL;
	}
	return( -1 );
}

/* Frees a attribute values
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_free(
     libfsxfs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_free";

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
	if( *attribute_values != NULL )
	{
		if( ( *attribute_values )->name != NULL )
		{
			memory_free(
			 ( *attribute_values )->name );
		}
		if( ( *attribute_values )->value_data != NULL )
		{
			memory_free(
			 ( *attribute_values )->value_data );
		}
		memory_free(
		 *attribute_values );

		*attribute_values = NULL;
	}
	return( 1 );
}

/* Sets the name including prefix
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_set_name(
     libfsxfs_attribute_values_t *attribute_values,
     const uint8_t *name,
     size_t name_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	static char *function     = "libfsxfs_attribute_values_set_name";
	const char *name_prefix   = NULL;
	size_t name_prefix_length = 0;

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
	if( attribute_values->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values - name value already set.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( ( name_size == 0 )
	 || ( name_size > (size_t) 255 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
		 function );

		return( -1 );
	}
	switch( flags & 0x7e )
	{
		case 0:
			name_prefix = "user.";
			break;

		case 2:
			name_prefix = "trusted.";
			break;

		case 4:
			name_prefix = "secure.";
			break;

		default:
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported flags: 0x%02" PRIx8 ".",
			 function,
			 flags );

			return( -1 );
	}
	name_prefix_length = narrow_string_length(
	                      name_prefix );

	attribute_values->name_size = name_prefix_length + name_size + 1;

	if( attribute_values->name_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
		 function );

		return( -1 );
	}
	attribute_values->name = (uint8_t *) memory_allocate(
	                                      sizeof( uint8_t ) * attribute_values->name_size );

	if( attribute_values->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	if( name_prefix_length > 0 )
	{
		if( memory_copy(
		     attribute_values->name,
		     name_prefix,
		     name_prefix_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name prefix to extended attribute name.",
			 function );

			goto on_error;
		}
	}
	if( memory_copy(
	     &( ( attribute_values->name )[ name_prefix_length ] ),
	     name,
	     name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name to extended attribute name.",
		 function );

		goto on_error;
	}
	( attribute_values->name )[ attribute_values->name_size - 1 ] = 0;

	return( 1 );

on_error:
	if( attribute_values->name != NULL )
	{
		memory_free(
		 attribute_values->name );

		attribute_values->name = NULL;
	}
	attribute_values->name_size = 0;

	return( -1 );
}

/* Sets the value data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_set_value_data(
     libfsxfs_attribute_values_t *attribute_values,
     const uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_set_value_data";

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
	if( attribute_values->value_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute values - value data value already set.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( ( value_data_size == 0 )
	 || ( value_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		return( -1 );
	}
	attribute_values->value_data = (uint8_t *) memory_allocate(
	                                            sizeof( uint8_t ) * value_data_size );

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
	     value_data,
	     value_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		goto on_error;
	}
	attribute_values->value_data_size = value_data_size;

	return( 1 );

on_error:
	if( attribute_values->value_data != NULL )
	{
		memory_free(
		 attribute_values->value_data );

		attribute_values->value_data = NULL;
	}
	return( -1 );
}

/* Compares an UTF-8 string with an attribute name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsxfs_attribute_values_compare_name_with_utf8_string(
     libfsxfs_attribute_values_t *attribute_values,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_compare_name_with_utf8_string";
	int result            = 0;

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
	result = libuna_utf8_string_compare_with_utf8_stream(
	          utf8_string,
	          utf8_string_length,
	          attribute_values->name,
	          attribute_values->name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_get_utf8_name_size(
     libfsxfs_attribute_values_t *attribute_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_get_utf8_name_size";

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
	if( libuna_utf8_string_size_from_utf8_stream(
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_get_utf8_name(
     libfsxfs_attribute_values_t *attribute_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_get_utf8_name";

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
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares an UTF-16 string with an attribute name
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsxfs_attribute_values_compare_name_with_utf16_string(
     libfsxfs_attribute_values_t *attribute_values,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_compare_name_with_utf16_string";
	int result            = 0;

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
	result = libuna_utf16_string_compare_with_utf8_stream(
	          utf16_string,
	          utf16_string_length,
	          attribute_values->name,
	          attribute_values->name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_get_utf16_name_size(
     libfsxfs_attribute_values_t *attribute_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_get_utf16_name_size";

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
	if( libuna_utf16_string_size_from_utf8_stream(
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attribute_values_get_utf16_name(
     libfsxfs_attribute_values_t *attribute_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attribute_values_get_utf16_name";

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
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     attribute_values->name,
	     (size_t) attribute_values->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

