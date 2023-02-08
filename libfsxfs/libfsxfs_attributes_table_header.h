/*
 * Short-form (extended) attributes table header functions
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

#if !defined( _LIBFSXFS_ATTRIBUTES_TABLE_HEADER_H )
#define _LIBFSXFS_ATTRIBUTES_TABLE_HEADER_H

#include <common.h>
#include <types.h>

#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_attributes_table_header libfsxfs_attributes_table_header_t;

struct libfsxfs_attributes_table_header
{
	/* Table data size
	 */
	uint16_t data_size;

	/* Number of entries
	 */
	uint8_t number_of_entries;
};

int libfsxfs_attributes_table_header_initialize(
     libfsxfs_attributes_table_header_t **attributes_table_header,
     libcerror_error_t **error );

int libfsxfs_attributes_table_header_free(
     libfsxfs_attributes_table_header_t **attributes_table_header,
     libcerror_error_t **error );

int libfsxfs_attributes_table_header_read_data(
     libfsxfs_attributes_table_header_t *attributes_table_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_ATTRIBUTES_TABLE_HEADER_H ) */

