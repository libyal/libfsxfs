/*
 * Directory entry functions
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

#include "libfsxfs_directory_entry.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libuna.h"

/* Creates directory entry
 * Make sure the value directory_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_entry_initialize(
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_initialize";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory entry value already set.",
		 function );

		return( -1 );
	}
	*directory_entry = memory_allocate_structure(
	                    libfsxfs_directory_entry_t );

	if( *directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_entry,
	     0,
	     sizeof( libfsxfs_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_entry != NULL )
	{
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( -1 );
}

/* Frees directory entry
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_entry_free(
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_free";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( *directory_entry != NULL )
	{
		memory_free(
		 *directory_entry );

		*directory_entry = NULL;
	}
	return( 1 );
}

/* Clones the directory entry value
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_entry_clone(
     libfsxfs_directory_entry_t **destination_directory_entry,
     libfsxfs_directory_entry_t *source_directory_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_clone";

	if( destination_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination directory entry.",
		 function );

		return( -1 );
	}
	if( *destination_directory_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination directory entry value already set.",
		 function );

		return( -1 );
	}
	if( source_directory_entry == NULL )
	{
		*destination_directory_entry = NULL;

		return( 1 );
	}
	*destination_directory_entry = memory_allocate_structure(
	                                libfsxfs_directory_entry_t );

	if( *destination_directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination directory entry.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_directory_entry,
	     source_directory_entry,
	     sizeof( libfsxfs_directory_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy source to destination directory entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_directory_entry != NULL )
	{
		memory_free(
		 *destination_directory_entry );

		*destination_directory_entry = NULL;
	}
	return( -1 );
}

/* Retrieves the inode number
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_entry_get_inode_number(
     libfsxfs_directory_entry_t *directory_entry,
     uint64_t *inode_number,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_get_inode_number";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( inode_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode number.",
		 function );

		return( -1 );
	}
	*inode_number = directory_entry->inode_number;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_entry_get_utf8_name_size(
     libfsxfs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_get_utf8_name_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf8_stream(
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
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
int libfsxfs_directory_entry_get_utf8_name(
     libfsxfs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_get_utf8_name";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
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

/* Compares an UTF-8 string with the name of the directory entry
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsxfs_directory_entry_compare_with_utf8_string(
     libfsxfs_directory_entry_t *directory_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_compare_with_utf8_string";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	result = libuna_utf8_string_compare_with_utf8_stream(
	          utf8_string,
	          utf8_string_length,
	          directory_entry->name,
	          directory_entry->name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-8 string with directory entry name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_entry_get_utf16_name_size(
     libfsxfs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_get_utf16_name_size";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf8_stream(
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
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
int libfsxfs_directory_entry_get_utf16_name(
     libfsxfs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_get_utf16_name";

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     directory_entry->name,
	     (size_t) directory_entry->name_size,
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

/* Compares an UTF-16 string with the name of the directory entry
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libfsxfs_directory_entry_compare_with_utf16_string(
     libfsxfs_directory_entry_t *directory_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_entry_compare_with_utf16_string";
	int result            = 0;

	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	result = libuna_utf16_string_compare_with_utf8_stream(
	          utf16_string,
	          utf16_string_length,
	          directory_entry->name,
	          directory_entry->name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare UTF-16 string with directory entry name.",
		 function );

		return( -1 );
	}
	return( result );
}

