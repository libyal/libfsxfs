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

#if !defined( _LIBFSXFS_ATTRIBUTE_VALUES_H )
#define _LIBFSXFS_ATTRIBUTE_VALUES_H

#include <common.h>
#include <types.h>

#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_attribute_values libfsxfs_attribute_values_t;

struct libfsxfs_attribute_values
{
	/* Name
	 */
	uint8_t *name;

	/* Name size
	 */
	size_t name_size;

	/* Value data size
	 */
	uint32_t value_data_size;

	/* Value data
	 */
	uint8_t *value_data;

	/* Value data block number
	 */
	uint32_t value_data_block_number;
};

int libfsxfs_attribute_values_initialize(
     libfsxfs_attribute_values_t **attribute_values,
     libcerror_error_t **error );

int libfsxfs_attribute_values_free(
     libfsxfs_attribute_values_t **attribute_values,
     libcerror_error_t **error );

int libfsxfs_attribute_values_set_name(
     libfsxfs_attribute_values_t *attribute_values,
     const uint8_t *name,
     size_t name_size,
     uint8_t flags,
     libcerror_error_t **error );

int libfsxfs_attribute_values_set_value_data(
     libfsxfs_attribute_values_t *attribute_values,
     const uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

int libfsxfs_attribute_values_compare_name_with_utf8_string(
     libfsxfs_attribute_values_t *attribute_values,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

int libfsxfs_attribute_values_get_utf8_name_size(
     libfsxfs_attribute_values_t *attribute_values,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsxfs_attribute_values_get_utf8_name(
     libfsxfs_attribute_values_t *attribute_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsxfs_attribute_values_compare_name_with_utf16_string(
     libfsxfs_attribute_values_t *attribute_values,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

int libfsxfs_attribute_values_get_utf16_name_size(
     libfsxfs_attribute_values_t *attribute_values,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsxfs_attribute_values_get_utf16_name(
     libfsxfs_attribute_values_t *attribute_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_ATTRIBUTE_VALUES_H ) */

