/*
 * Data stream functions
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

#if !defined( _LIBFSXFS_DATA_STREAM_H )
#define _LIBFSXFS_DATA_STREAM_H

#include <common.h>
#include <types.h>

#include "libfsxfs_inode.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libfsxfs_data_stream_initialize_from_data(
     libfdata_stream_t **data_stream,
     const uint8_t *data,
     size64_t data_size,
     libcerror_error_t **error );

int libfsxfs_data_stream_initialize_from_extents(
     libfdata_stream_t **data_stream,
     libfsxfs_io_handle_t *io_handle,
     libfsxfs_inode_t *inode,
     size64_t data_size,
     libcerror_error_t **error );

int libfsxfs_data_stream_initialize(
     libfdata_stream_t **data_stream,
     libfsxfs_io_handle_t *io_handle,
     libfsxfs_inode_t *inode,
     size64_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_DATA_STREAM_H ) */

