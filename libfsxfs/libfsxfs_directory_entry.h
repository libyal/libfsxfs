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

#if !defined( _LIBFSXFS_DIRECTORY_ENTRY_H )
#define _LIBFSXFS_DIRECTORY_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_directory_entry libfsxfs_directory_entry_t;

struct libfsxfs_directory_entry
{
	/* Inode number
	 */
	uint64_t inode_number;

	/* Name
	 */
	uint8_t name[ 255 ];

	/* Name size
	 */
	uint8_t name_size;
};

int libfsxfs_directory_entry_initialize(
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsxfs_directory_entry_free(
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsxfs_directory_entry_clone(
     libfsxfs_directory_entry_t **destination_directory_entry,
     libfsxfs_directory_entry_t *source_directory_entry,
     libcerror_error_t **error );

int libfsxfs_directory_entry_get_inode_number(
     libfsxfs_directory_entry_t *directory_entry,
     uint64_t *inode_number,
     libcerror_error_t **error );

int libfsxfs_directory_entry_get_utf8_name_size(
     libfsxfs_directory_entry_t *directory_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsxfs_directory_entry_get_utf8_name(
     libfsxfs_directory_entry_t *directory_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsxfs_directory_entry_compare_with_utf8_string(
     libfsxfs_directory_entry_t *directory_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libfsxfs_directory_entry_get_utf16_name_size(
     libfsxfs_directory_entry_t *directory_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsxfs_directory_entry_get_utf16_name(
     libfsxfs_directory_entry_t *directory_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfsxfs_directory_entry_compare_with_utf16_string(
     libfsxfs_directory_entry_t *directory_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_DIRECTORY_ENTRY_H ) */

