/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBFSXFS_IO_HANDLE_H )
#define _LIBFSXFS_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_io_handle libfsxfs_io_handle_t;

struct libfsxfs_io_handle
{
	/* The format version
	 */
	int format_version;

	/* Secondary feature flags
	 */
	uint32_t secondary_feature_flags;

	/* The block size
	 */
	uint32_t block_size;

	/* Allocation group size
	 */
	uint32_t allocation_group_size;

	/* Inode size
	 */
	uint16_t inode_size;

	/* Directory block size
	 */
	uint32_t directory_block_size;

	/* Number of bits used for the relative block number
	 */
	uint8_t number_of_relative_block_number_bits;

	/* Number of bits used for the relative inode number
	 */
	uint8_t number_of_relative_inode_number_bits;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfsxfs_io_handle_initialize(
     libfsxfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsxfs_io_handle_free(
     libfsxfs_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfsxfs_io_handle_clear(
     libfsxfs_io_handle_t *io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_IO_HANDLE_H ) */

