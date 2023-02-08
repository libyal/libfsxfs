/*
 * Extended attribute functions
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

#if !defined( _LIBFSXFS_EXTENDED_ATTRIBUTE_H )
#define _LIBFSXFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsxfs_attribute_values.h"
#include "libfsxfs_extern.h"
#include "libfsxfs_file_system.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcthreads.h"
#include "libfsxfs_libfdata.h"
#include "libfsxfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_internal_extended_attribute libfsxfs_internal_extended_attribute_t;

struct libfsxfs_internal_extended_attribute
{
	/* The IO handle
	 */
	libfsxfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The file system
	 */
	libfsxfs_file_system_t *file_system;

	/* The inode
	 */
	libfsxfs_inode_t *inode;

	/* The attribute values
	 */
	libfsxfs_attribute_values_t *attribute_values;

	/* The data stream
	 */
	libfdata_stream_t *data_stream;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsxfs_extended_attribute_initialize(
     libfsxfs_extended_attribute_t **extended_attribute,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_file_system_t *file_system,
     libfsxfs_inode_t *inode,
     libfsxfs_attribute_values_t *attribute_values,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_free(
     libfsxfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf8_name_size(
     libfsxfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf8_name(
     libfsxfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf16_name_size(
     libfsxfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf16_name(
     libfsxfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsxfs_internal_extended_attribute_get_data_stream(
     libfsxfs_internal_extended_attribute_t *internal_extended_attribute,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
ssize_t libfsxfs_extended_attribute_read_buffer(
         libfsxfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFSXFS_EXTERN \
ssize_t libfsxfs_extended_attribute_read_buffer_at_offset(
         libfsxfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

LIBFSXFS_EXTERN \
off64_t libfsxfs_extended_attribute_seek_offset(
         libfsxfs_extended_attribute_t *extended_attribute,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_offset(
     libfsxfs_extended_attribute_t *extended_attribute,
     off64_t *offset,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_size(
     libfsxfs_extended_attribute_t *extended_attribute,
     size64_t *size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_number_of_extents(
     libfsxfs_extended_attribute_t *extended_attribute,
     int *number_of_extents,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_extent_by_index(
     libfsxfs_extended_attribute_t *extended_attribute,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_EXTENDED_ATTRIBUTE_H ) */

