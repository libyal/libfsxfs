/*
 * Volume functions
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

#if !defined( _LIBFSXFS_INTERNAL_VOLUME_H )
#define _LIBFSXFS_INTERNAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfsxfs_extern.h"
#include "libfsxfs_file_system.h"
#include "libfsxfs_inode_btree.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcthreads.h"
#include "libfsxfs_superblock.h"
#include "libfsxfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_internal_volume libfsxfs_internal_volume_t;

struct libfsxfs_internal_volume
{
	/* The superblock
	 */
	libfsxfs_superblock_t *superblock;

	/* The file system
	 */
	libfsxfs_file_system_t *file_system;

	/* The inode B+ tree
	 */
	libfsxfs_inode_btree_t *inode_btree;

	/* The IO handle
	 */
	libfsxfs_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBFSXFS_EXTERN \
int libfsxfs_volume_initialize(
     libfsxfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_free(
     libfsxfs_volume_t **volume,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_signal_abort(
     libfsxfs_volume_t *volume,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_open(
     libfsxfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFSXFS_EXTERN \
int libfsxfs_volume_open_wide(
     libfsxfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFSXFS_EXTERN \
int libfsxfs_volume_open_file_io_handle(
     libfsxfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_close(
     libfsxfs_volume_t *volume,
     libcerror_error_t **error );

int libfsxfs_internal_volume_open_read(
     libfsxfs_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_format_version(
     libfsxfs_volume_t *volume,
     uint8_t *format_version,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf8_label_size(
     libfsxfs_volume_t *volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf8_label(
     libfsxfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf16_label_size(
     libfsxfs_volume_t *volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf16_label(
     libfsxfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_root_directory(
     libfsxfs_volume_t *volume,
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsxfs_internal_volume_get_file_entry_by_inode(
     libfsxfs_internal_volume_t *internal_volume,
     uint64_t inode_number,
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_file_entry_by_inode(
     libfsxfs_volume_t *volume,
     uint64_t inode_number,
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsxfs_internal_volume_get_file_entry_by_utf8_path(
     libfsxfs_internal_volume_t *internal_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_file_entry_by_utf8_path(
     libfsxfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

int libfsxfs_internal_volume_get_file_entry_by_utf16_path(
     libfsxfs_internal_volume_t *internal_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_volume_get_file_entry_by_utf16_path(
     libfsxfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_INTERNAL_VOLUME_H ) */

