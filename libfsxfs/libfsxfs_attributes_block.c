/*
 * (Extended) attributes block functions
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
#include "libfsxfs_attributes_block.h"
#include "libfsxfs_attributes_block_header.h"
#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"

#include "fsxfs_attributes_block.h"

/* Creates attributes block
 * Make sure the value attributes_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_block_initialize(
     libfsxfs_attributes_block_t **attributes_block,
     size_t block_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_block_initialize";

	if( attributes_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes block.",
		 function );

		return( -1 );
	}
	if( *attributes_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes block value already set.",
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
	*attributes_block = memory_allocate_structure(
	                    libfsxfs_attributes_block_t );

	if( *attributes_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attributes block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attributes_block,
	     0,
	     sizeof( libfsxfs_attributes_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attributes block.",
		 function );

		memory_free(
		 *attributes_block );

		*attributes_block = NULL;

		return( -1 );
	}
	( *attributes_block )->data = (uint8_t *) memory_allocate(
	                                          sizeof( uint8_t ) * block_size );

	if( ( *attributes_block )->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attributes block data.",
		 function );

		goto on_error;
	}
	( *attributes_block )->data_size = block_size;

	return( 1 );

on_error:
	if( *attributes_block != NULL )
	{
		memory_free(
		 *attributes_block );

		*attributes_block = NULL;
	}
	return( -1 );
}

/* Frees attributes block
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_block_free(
     libfsxfs_attributes_block_t **attributes_block,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_block_free";
	int result            = 1;

	if( attributes_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes block.",
		 function );

		return( -1 );
	}
	if( *attributes_block != NULL )
	{
		if( ( *attributes_block )->header != NULL )
		{
			if( libfsxfs_attributes_block_header_free(
			     &( ( *attributes_block )->header ),
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
		 ( *attributes_block )->data );

		memory_free(
		 *attributes_block );

		*attributes_block = NULL;
	}
	return( result );
}

/* Reads the attributes block
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_block_read_data(
     libfsxfs_attributes_block_t *attributes_block,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extended_attributes_array,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values = NULL;
	static char *function                         = "libfsxfs_attributes_block_read_data";
	size_t data_offset                            = 0;
	size_t entries_data_size                      = 0;
	size_t header_data_size                       = 0;
	size_t values_data_end_offset                 = 0;
	size_t values_data_size                       = 0;
	size_t values_data_start_offset               = 0;
	uint16_t attribute_entry_index                = 0;
	uint16_t values_offset                        = 0;
	uint8_t flags                                 = 0;
	uint8_t name_size                             = 0;
	int entry_index                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                          = 0;
#endif

	if( attributes_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes block.",
		 function );

		return( -1 );
	}
	if( attributes_block->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes block - header already set.",
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
		 "%s: attributes block data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfsxfs_attributes_block_header_initialize(
	     &( attributes_block->header ),
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
	if( libfsxfs_attributes_block_header_read_data(
	     attributes_block->header,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attributes block header.",
		 function );

		goto on_error;
	}
	if( attributes_block->header->format_version == 3 )
	{
		header_data_size = sizeof( fsxfs_attributes_block_header_v3_t );
	}
	else
	{
		header_data_size = sizeof( fsxfs_attributes_block_header_v2_t );
	}
	if( ( attributes_block->header->values_data_offset < header_data_size )
	 || ( attributes_block->header->values_data_offset >= data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid values data offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( attributes_block->header->values_data_size > ( data_size - attributes_block->header->values_data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid values data size value out of bounds.",
		 function );

		return( -1 );
	}
	data_offset = header_data_size;

	entries_data_size = sizeof( fsxfs_attributes_block_entry_t ) * attributes_block->header->number_of_entries;

	if( entries_data_size > ( data_size - data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of entries value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute entries data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 entries_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	values_data_start_offset = attributes_block->header->values_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: attribute values data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ values_data_start_offset ] ),
		 attributes_block->header->values_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	values_data_end_offset = values_data_start_offset + attributes_block->header->values_data_size;

	for( attribute_entry_index = 0;
	     attribute_entry_index < attributes_block->header->number_of_entries;
	     attribute_entry_index++ )
	{
		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_attributes_block_entry_t *) &( data[ data_offset ] ) )->values_offset,
		 values_offset );

		flags = ( (fsxfs_attributes_block_entry_t *) &( data[ data_offset ] ) )->flags;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_block_entry_t *) &( data[ data_offset ] ) )->name_hash,
			 value_32bit );
			libcnotify_printf(
			 "%s: value hash\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );

			libcnotify_printf(
			 "%s: values offset\t\t\t: %" PRIu16 "\n",
			 function,
			 values_offset );

			libcnotify_printf(
			 "%s: flags\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 flags );

			libcnotify_printf(
			 "%s: unknown1\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 ( (fsxfs_attributes_block_entry_t *) &( data[ data_offset ] ) )->unknown1 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( fsxfs_attributes_block_entry_t );

		if( ( values_offset < values_data_start_offset )
		 || ( values_offset >= values_data_end_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid values offset value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( flags & 0x01 ) != 0 )
		{
			values_data_size = sizeof( fsxfs_attributes_block_values_local_t );
		}
		else
		{
			values_data_size = sizeof( fsxfs_attributes_block_values_remote_t );
		}
		if( values_data_size > ( values_data_end_offset - values_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid values data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( flags & 0x01 ) != 0 )
		{
			name_size = ( (fsxfs_attributes_block_values_local_t *) &( data[ values_offset ] ) )->name_size;
		}
		else
		{
			name_size = ( (fsxfs_attributes_block_values_remote_t *) &( data[ values_offset ] ) )->name_size;
		}
		if( name_size > ( values_data_end_offset - values_offset - values_data_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: values data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ values_offset ] ),
			 values_data_size + name_size,
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
			 "%s: unable to create attribute: %d values.",
			 function,
			 attribute_entry_index );

			goto on_error;
		}
		if( ( flags & 0x01 ) != 0 )
		{
			byte_stream_copy_to_uint16_big_endian(
			 ( (fsxfs_attributes_block_values_local_t *) &( data[ values_offset ] ) )->value_data_size,
			 attribute_values->value_data_size );
		}
		else
		{
			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_block_values_remote_t *) &( data[ values_offset ] ) )->value_data_block_number,
			 attribute_values->value_data_block_number );

			byte_stream_copy_to_uint32_big_endian(
			 ( (fsxfs_attributes_block_values_remote_t *) &( data[ values_offset ] ) )->value_data_size,
			 attribute_values->value_data_size );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( flags & 0x01 ) == 0 )
			{
				libcnotify_printf(
				 "%s: value data block number\t\t: %" PRIu32 "\n",
				 function,
				 attribute_values->value_data_block_number );
			}
			libcnotify_printf(
			 "%s: value data size\t\t\t: %" PRIu32 "\n",
			 function,
			 attribute_values->value_data_size );

			libcnotify_printf(
			 "%s: name size\t\t\t\t: %" PRIu8 "\n",
			 function,
			 name_size );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( name_size > 0 )
		{
			if( libfsxfs_attribute_values_set_name(
			     attribute_values,
			     &( data[ values_offset + values_data_size ] ),
			     name_size,
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

		if( ( flags & 0x01 ) != 0 )
		{
/* TODO set local value data in attribute values */
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
			 "%s: unable to append attribute values to extended attributes array.",
			 function );

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
	if( attributes_block->header != NULL )
	{
		libfsxfs_attributes_block_header_free(
		 &( attributes_block->header ),
		 NULL );
	}
	libcdata_array_empty(
	 extended_attributes_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_attribute_values_free,
	 NULL );

	return( -1 );
}

/* Reads the attributes block from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_attributes_block_read_file_io_handle(
     libfsxfs_attributes_block_t *attributes_block,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcdata_array_t *extended_attributes_array,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_attributes_block_read_file_io_handle";
	ssize_t read_count    = 0;

	if( attributes_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attributes block.",
		 function );

		return( -1 );
	}
	if( attributes_block->header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attributes block - header value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading attributes block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              attributes_block->data,
	              attributes_block->data_size,
	              file_offset,
	              error );

	if( read_count != (ssize_t) attributes_block->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attributes block data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsxfs_attributes_block_read_data(
	     attributes_block,
	     io_handle,
	     attributes_block->data,
	     attributes_block->data_size,
	     extended_attributes_array,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read attributes block at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

