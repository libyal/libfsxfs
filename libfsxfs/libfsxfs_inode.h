/*
 * Inode functions
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

#if !defined( _LIBFSXFS_INODE_H )
#define _LIBFSXFS_INODE_H

#include <common.h>
#include <types.h>

#include "libfsxfs_extent.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_inode libfsxfs_inode_t;

struct libfsxfs_inode
{
	/* Data
	 */
	uint8_t *data;

	/* Data size
	 */
	size_t data_size;

	/* Format version
	 */
	uint8_t format_version;

	/* File mode
	 */
	uint16_t file_mode;

	/* Owner identifier
	 */
	uint32_t owner_identifier;

	/* Group identifier
	 */
	uint32_t group_identifier;

	/* Number of links
	 */
	uint32_t number_of_links;

	/* Access time
	 */
	uint64_t access_time;

	/* Modification time
	 */
	int64_t modification_time;

	/* Inode change time
	 */
	int64_t inode_change_time;

	/* Creation time
	 */
	int64_t creation_time;

	/* Size
	 */
	uint64_t size;

	/* (Data) fork type
	 */
	uint8_t fork_type;

	/* Device identifier
	 */
	uint32_t device_identifier;

	/* Inline data
	 */
	const uint8_t *inline_data;

	/* Number of data extents
	 */
	uint32_t number_of_data_extents;

	/* Data extents array
	 */
	libcdata_array_t *data_extents_array;

	/* Data fork offset
	 */
	uint16_t data_fork_offset;

	/* Data fork size
	 */
	uint16_t data_fork_size;

	/* (Extended) attributes fork type
	 */
	uint8_t attributes_fork_type;

	/* Inline (extended) attributes data
	 */
	const uint8_t *inline_attributes_data;

	/* Number of (extended) attributes extents
	 */
	uint32_t number_of_attributes_extents;

	/* (Extended) attributes extents array
	 */
	libcdata_array_t *attributes_extents_array;

	/* (Extended) attributes fork offset
	 */
	uint16_t attributes_fork_offset;

	/* (Extended) attributes fork size
	 */
	uint16_t attributes_fork_size;
};

int libfsxfs_inode_initialize(
     libfsxfs_inode_t **inode,
     size_t inode_size,
     libcerror_error_t **error );

int libfsxfs_inode_free(
     libfsxfs_inode_t **inode,
     libcerror_error_t **error );

int libfsxfs_inode_read_data(
     libfsxfs_inode_t *inode,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsxfs_inode_read_file_io_handle(
     libfsxfs_inode_t *inode,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsxfs_inode_get_creation_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsxfs_inode_get_modification_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsxfs_inode_get_access_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsxfs_inode_get_inode_change_time(
     libfsxfs_inode_t *inode,
     int64_t *posix_time,
     libcerror_error_t **error );

int libfsxfs_inode_get_owner_identifier(
     libfsxfs_inode_t *inode,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

int libfsxfs_inode_get_group_identifier(
     libfsxfs_inode_t *inode,
     uint32_t *group_identifier,
     libcerror_error_t **error );

int libfsxfs_inode_get_file_mode(
     libfsxfs_inode_t *inode,
     uint16_t *file_mode,
     libcerror_error_t **error );

int libfsxfs_inode_get_number_of_links(
     libfsxfs_inode_t *inode,
     uint32_t *number_of_links,
     libcerror_error_t **error );

int libfsxfs_inode_get_data_size(
     libfsxfs_inode_t *inode,
     uint64_t *data_size,
     libcerror_error_t **error );

int libfsxfs_inode_get_device_identifier(
     libfsxfs_inode_t *inode,
     uint32_t *device_identifier,
     libcerror_error_t **error );

int libfsxfs_inode_get_device_number(
     libfsxfs_inode_t *inode,
     uint32_t *major_device_number,
     uint32_t *minor_device_number,
     libcerror_error_t **error );

int libfsxfs_inode_get_number_of_data_extents(
     libfsxfs_inode_t *inode,
     int *number_of_extents,
     libcerror_error_t **error );

int libfsxfs_inode_get_data_extent_by_index(
     libfsxfs_inode_t *inode,
     int extent_index,
     libfsxfs_extent_t **extent,
     libcerror_error_t **error );

int libfsxfs_inode_get_number_of_attributes_extents(
     libfsxfs_inode_t *inode,
     int *number_of_extents,
     libcerror_error_t **error );

int libfsxfs_inode_get_attributes_extent_by_index(
     libfsxfs_inode_t *inode,
     int extent_index,
     libfsxfs_extent_t **extent,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_INODE_H ) */

