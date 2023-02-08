/*
 * (Extended) attributes functions
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

#if !defined( _LIBFSXFS_ATTRIBUTES_H )
#define _LIBFSXFS_ATTRIBUTES_H

#include <common.h>
#include <types.h>

#include "libfsxfs_attribute_values.h"
#include "libfsxfs_attributes_leaf_block_header.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsxfs_attributes_read_branch_values(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extended_attributes_array,
     int recursion_depth,
     libcerror_error_t **error );

int libfsxfs_attributes_read_leaf_values(
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extended_attributes_array,
     libcerror_error_t **error );

int libfsxfs_attributes_get_from_block(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     uint32_t block_number,
     libcdata_array_t *extended_attributes_array,
     int recursion_depth,
     libcerror_error_t **error );

int libfsxfs_attributes_get_from_inode(
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libcdata_array_t *extended_attributes_array,
     libcerror_error_t **error );

int libfsxfs_attributes_get_value_data_stream(
     libfsxfs_io_handle_t *io_handle,
     libfsxfs_inode_t *inode,
     libfsxfs_attribute_values_t *attribute_values,
     libfdata_stream_t **data_stream,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_ATTRIBUTES_H ) */

