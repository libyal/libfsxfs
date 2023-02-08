/*
 * Short-form directory table functions
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

#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_directory_entry.h"
#include "libfsxfs_directory_table.h"
#include "libfsxfs_directory_table_header.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

/* Creates directory table
 * Make sure the value directory_table is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_table_initialize(
     libfsxfs_directory_table_t **directory_table,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_table_initialize";

	if( directory_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory table.",
		 function );

		return( -1 );
	}
	if( *directory_table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory table value already set.",
		 function );

		return( -1 );
	}
	*directory_table = memory_allocate_structure(
	                    libfsxfs_directory_table_t );

	if( *directory_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create directory table.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *directory_table,
	     0,
	     sizeof( libfsxfs_directory_table_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear directory table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *directory_table != NULL )
	{
		memory_free(
		 *directory_table );

		*directory_table = NULL;
	}
	return( -1 );
}

/* Frees directory table
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_table_free(
     libfsxfs_directory_table_t **directory_table,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_directory_table_free";
	int result            = 1;

	if( directory_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory table.",
		 function );

		return( -1 );
	}
	if( *directory_table != NULL )
	{
		if( ( *directory_table )->header != NULL )
		{
			if( libfsxfs_directory_table_header_free(
			     &( ( *directory_table )->header ),
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
		 *directory_table );

		*directory_table = NULL;
	}
	return( result );
}

/* Reads the directory table
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_directory_table_read_data(
     libfsxfs_directory_table_t *directory_table,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *entries_array,
     libcerror_error_t **error )
{
	libfsxfs_directory_entry_t *directory_entry = NULL;
	static char *function                       = "libfsxfs_directory_table_read_data";
	size_t data_offset                          = 0;
	size_t entry_data_size                      = 0;
	uint32_t directory_entry_index              = 0;
	uint8_t name_size                           = 0;
	int entry_index                             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                        = 0;
#endif

	if( directory_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory table.",
		 function );

		return( -1 );
	}
	if( directory_table->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid directory table - header already set.",
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
	if( libfsxfs_directory_table_header_initialize(
	     &( directory_table->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	if( libfsxfs_directory_table_header_read_data(
	     directory_table->header,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read directory table header.",
		 function );

		goto on_error;
	}
	if( directory_table->header->inode_number_data_size == 4 )
	{
		data_offset = 6;
	}
	else
	{
		data_offset = 10;
	}
	for( directory_entry_index = 0;
	     directory_entry_index < directory_table->header->number_of_entries;
	     directory_entry_index++ )
	{
		if( ( data_size - data_offset ) < 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid directory entry: %d data size value out of bounds.",
			 function,
			 directory_entry_index );

			goto on_error;
		}
		name_size = data[ data_offset ];

		entry_data_size = 3 + name_size;

		if( ( io_handle->format_version == 5 )
		 || ( ( io_handle->secondary_feature_flags & LIBFSXFS_SECONDARY_FEATURE_FLAG_FILE_TYPE ) != 0 ) )
		{
			entry_data_size++;
		}
		entry_data_size += directory_table->header->inode_number_data_size;

		if( entry_data_size > ( data_size - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid directory entry: %d data size value out of bounds.",
			 function,
			 directory_entry_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: directory entry: %d data:\n",
			 function,
			 directory_entry_index );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 entry_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset++;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu8 "\n",
			 function,
			 name_size );

			byte_stream_copy_to_uint16_big_endian(
			 &( data[ data_offset ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: tag offset\t\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfsxfs_debug_print_utf8_string_value(
			     function,
			     "name\t\t\t\t",
			     &( data[ data_offset ] ),
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

		if( libfsxfs_directory_entry_initialize(
		     &directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create entry.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     directory_entry->name,
		     &( data[ data_offset ] ),
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

		data_offset += name_size;

		if( ( io_handle->format_version == 5 )
		 || ( ( io_handle->secondary_feature_flags & LIBFSXFS_SECONDARY_FEATURE_FLAG_FILE_TYPE ) != 0 ) )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: file type\t\t\t\t: %" PRIu8 "\n",
				 function,
				 data[ data_offset ] );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			data_offset++;
		}
		if( directory_table->header->inode_number_data_size == 4 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 &( data[ data_offset ] ),
			 directory_entry->inode_number );

			data_offset += 4;
		}
		else
		{
			byte_stream_copy_to_uint64_big_endian(
			 &( data[ data_offset ] ),
			 directory_entry->inode_number );

			data_offset += 8;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: inode number\t\t\t: %" PRIu64 "\n",
			 function,
			 directory_entry->inode_number );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsxfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( directory_table->header != NULL )
	{
		libfsxfs_directory_table_header_free(
		 &( directory_table->header ),
		 NULL );
	}
	libcdata_array_empty(
	 entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	 NULL );

	return( -1 );
}

