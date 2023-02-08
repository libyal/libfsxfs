/*
 * Extent list functions
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

#if !defined( _LIBFSXFS_EXTENT_LIST_H )
#define _LIBFSXFS_EXTENT_LIST_H

#include <common.h>
#include <types.h>

#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsxfs_extent_list_read_data(
     libcdata_array_t *extents_array,
     uint64_t number_of_blocks,
     uint32_t number_of_extents,
     const uint8_t *data,
     size_t data_size,
     uint8_t add_sparse_extents,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_EXTENT_LIST_H ) */

