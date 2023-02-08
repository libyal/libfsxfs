/*
 * Block directory functions
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
#include "libfsxfs_block_directory.h"
#include "libfsxfs_block_directory_footer.h"
#include "libfsxfs_block_directory_header.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_block_directory.h"

/* Creates block directory
 * Make sure the value block_directory is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_initialize(
     libfsxfs_block_directory_t **block_directory,
     size_t block_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_initialize";

	if( block_directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory.",
		 function );

		return( -1 );
	}
	if( *block_directory != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block directory value already set.",
		 function );

		return( -1 );
	}
	if( ( block_size == 0 )
	 || ( block_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
	*block_directory = memory_allocate_structure(
	                    libfsxfs_block_directory_t );

	if( *block_directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block directory.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *block_directory,
	     0,
	     sizeof( libfsxfs_block_directory_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear block directory.",
		 function );

		memory_free(
		 *block_directory );

		*block_directory = NULL;

		return( -1 );
	}
	( *block_directory )->data = (uint8_t *) memory_allocate(
	                                          sizeof( uint8_t ) * block_size );

	if( ( *block_directory )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create block directory data.",
		 function );

		goto on_error;
	}
	( *block_directory )->data_size = block_size;

	return( 1 );

on_error:
	if( *block_directory != NULL )
	{
		memory_free(
		 *block_directory );

		*block_directory = NULL;
	}
	return( -1 );
}

/* Frees block directory
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_free(
     libfsxfs_block_directory_t **block_directory,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_free";
	int result            = 1;

	if( block_directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory.",
		 function );

		return( -1 );
	}
	if( *block_directory != NULL )
	{
		if( ( *block_directory )->footer != NULL )
		{
			if( libfsxfs_block_directory_footer_free(
			     &( ( *block_directory )->footer ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free footer.",
				 function );

				result = -1;
			}
		}
		if( ( *block_directory )->header != NULL )
		{
			if( libfsxfs_block_directory_header_free(
			     &( ( *block_directory )->header ),
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
		 ( *block_directory )->data );

		memory_free(
		 *block_directory );

		*block_directory = NULL;
	}
	return( result );
}

/* Reads the block directory
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_read_data(
     libfsxfs_block_directory_t *block_directory,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *entries_array,
     libcerror_error_t **error )
{
	libfsxfs_directory_entry_t *directory_entry = NULL;
	static char *function                       = "libfsxfs_block_directory_read_data";
	size_t alignment_padding_size               = 0;
	size_t data_offset                          = 0;
	size_t entries_data_end_offset              = 0;
	size_t entry_data_size                      = 0;
	size_t hash_values_data_size                = 0;
	uint64_t inode_number                       = 0;
	uint32_t directory_entry_index              = 0;
	uint16_t free_tag                           = 0;
	uint8_t name_size                           = 0;
	int entry_index                             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                        = 0;
#endif

	if( block_directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory.",
		 function );

		return( -1 );
	}
	if( block_directory->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block directory - header already set.",
		 function );

		return( -1 );
	}
	if( block_directory->footer != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block directory - footer already set.",
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
		 "%s: block directory data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfsxfs_block_directory_header_initialize(
	     &( block_directory->header ),
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
	if( libfsxfs_block_directory_header_read_data(
	     block_directory->header,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read block directory header.",
		 function );

		goto on_error;
	}
	if( block_directory->header->has_footer != 0 )
	{
		if( libfsxfs_block_directory_footer_initialize(
		     &( block_directory->footer ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create footer.",
			 function );

			goto on_error;
		}
		if( libfsxfs_block_directory_footer_read_data(
		     block_directory->footer,
		     &( data[ data_size - 8 ] ),
		     8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read block directory footer.",
			 function );

			goto on_error;
		}
		if( (size_t) block_directory->footer->number_of_entries > ( ( data_size - data_offset - 8 ) / 8 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			goto on_error;
		}
		hash_values_data_size = (size_t) block_directory->footer->number_of_entries * 8;

		entries_data_end_offset = data_size - ( 8 + hash_values_data_size );
	}
	else
	{
		entries_data_end_offset = data_size;
	}
	if( block_directory->header->format_version == 3 )
	{
		data_offset = sizeof( fsxfs_block_directory_header_v3_t );
	}
	else
	{
		data_offset = sizeof( fsxfs_block_directory_header_v2_t );
	}
	while( data_offset < entries_data_end_offset )
	{
		if( ( entries_data_end_offset - data_offset ) < 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_big_endian(
		 &( data[ data_offset ] ),
		 free_tag );

		if( free_tag == 0xffff )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( data[ data_offset + 2 ] ),
			 entry_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: unused block signature\t\t: 0x%04" PRIx16 "\n",
				 function,
				 free_tag );

				libcnotify_printf(
				 "%s: entry data size\t\t\t: %" PRIzd "\n",
				 function,
				 entry_data_size );

				libcnotify_printf(
				 "\n" );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			if( entry_data_size < 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid data size value out of bounds.",
				 function );

				goto on_error;
			}
			data_offset += entry_data_size;

			continue;
		}
		if( ( entries_data_end_offset - data_offset ) < 9 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		name_size = data[ data_offset + 8 ];

		entry_data_size = 9 + name_size + 2;

		if( ( io_handle->format_version == 5 )
		 || ( ( io_handle->secondary_feature_flags & LIBFSXFS_SECONDARY_FEATURE_FLAG_FILE_TYPE ) != 0 ) )
		{
			entry_data_size++;
		}
		alignment_padding_size = entry_data_size % 8;

		if( alignment_padding_size != 0 )
		{
			alignment_padding_size = 8 - alignment_padding_size;
			entry_data_size       += alignment_padding_size;
		}
		if( ( entry_data_size < 6 )
		 || ( entry_data_size > ( entries_data_end_offset - data_offset ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

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

		byte_stream_copy_to_uint64_big_endian(
		 &( data[ data_offset ] ),
		 inode_number );

		data_offset += 9;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: inode number\t\t\t: %" PRIu64 "\n",
			 function,
			 inode_number );

			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu8 "\n",
			 function,
			 name_size );

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

		if( ( name_size == 1 )
		 && ( data[ data_offset ] == '.' ) )
		{
			/* Ignore self directory entry "." */
		}
		else if( ( name_size == 2 )
		      && ( data[ data_offset ] == '.' )
		      && ( data[ data_offset + 1 ] == '.' ) )
		{
			/* Ignore parent directory entry ".." */
		}
		else
		{
			if( libfsxfs_directory_entry_initialize(
			     &directory_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create directory entry: %d.",
				 function,
				 directory_entry_index );

				goto on_error;
			}
			directory_entry->inode_number = inode_number;

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
		}
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
		if( alignment_padding_size > 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: alignment padding:\n",
				 function,
				 directory_entry_index );
				libcnotify_print_data(
				 &( data[ data_offset ] ),
				 alignment_padding_size,
				 0 );
			}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

			data_offset += alignment_padding_size;
		}
		if( directory_entry != NULL )
		{
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
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint16_big_endian(
			 &( data[ data_offset ] ),
			 value_16bit );
			libcnotify_printf(
			 "%s: tag offset\t\t\t\t: %" PRIu16 "\n",
			 function,
			 value_16bit );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += 2;

		directory_entry_index++;
	}
	if( hash_values_data_size > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: hash values data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ entries_data_end_offset ] ),
			 hash_values_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	return( 1 );

on_error:
	if( directory_entry != NULL )
	{
		libfsxfs_directory_entry_free(
		 &directory_entry,
		 NULL );
	}
	if( block_directory->footer != NULL )
	{
		libfsxfs_block_directory_footer_free(
		 &( block_directory->footer ),
		 NULL );
	}
	if( block_directory->header != NULL )
	{
		libfsxfs_block_directory_header_free(
		 &( block_directory->header ),
		 NULL );
	}
	libcdata_array_empty(
	 entries_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_directory_entry_free,
	 NULL );

	return( -1 );
}

/* Reads the block directory from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_block_directory_read_file_io_handle(
     libfsxfs_block_directory_t *block_directory,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcdata_array_t *entries_array,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_block_directory_read_file_io_handle";
	ssize_t read_count    = 0;

	if( block_directory == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block directory.",
		 function );

		return( -1 );
	}
	if( block_directory->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid block directory - header value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading block directory at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              block_directory->data,
	              block_directory->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) block_directory->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read block directory data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsxfs_block_directory_read_data(
	     block_directory,
	     io_handle,
	     block_directory->data,
	     block_directory->data_size,
	     entries_array,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read block directory at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

