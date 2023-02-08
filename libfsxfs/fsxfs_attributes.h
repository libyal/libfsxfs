/*
 * Extended attributes of the X File System (XFS)
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

#if !defined( _FSXFS_ATTRIBUTES_H )
#define _FSXFS_ATTRIBUTES_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_attributes_short_form_header fsxfs_attributes_short_form_header_t;

struct fsxfs_attributes_short_form_header
{
	/* Data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* Number of entries
	 * Consists of 1 byte
	 */
	uint8_t number_of_entries;

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown1;
};

typedef struct fsxfs_attributes_short_form_entry fsxfs_attributes_short_form_entry_t;

struct fsxfs_attributes_short_form_entry
{
	/* Name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* Value data size
	 * Consists of 1 byte
	 */
	uint8_t value_data_size;

	/* Flags
	 * Consists of 1 byte
	 */
	uint8_t flags;
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_ATTRIBUTES_H ) */

