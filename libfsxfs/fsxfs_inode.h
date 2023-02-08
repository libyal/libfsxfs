/*
 * Inode structures of the X File System (XFS)
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

#if !defined( _FSXFS_INODE_H )
#define _FSXFS_INODE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_inode_v1 fsxfs_inode_v1_t;

struct fsxfs_inode_v1
{
	/* Signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* File mode
	 * Consists of 2 bytes
	 */
	uint8_t file_mode[ 2 ];

	/* Format version
	 * Consists of 1 byte
	 */
	uint8_t format_version;

	/* Fork type
	 * Consists of 1 byte
	 */
	uint8_t fork_type;

	/* Number of links
	 * Consists of 2 bytes
	 */
	uint8_t number_of_links[ 2 ];

	/* Owner identifier
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* Group identifier
	 * Consists of 4 bytes
	 */
	uint8_t group_identifier[ 4 ];

	/* Unknown (reserved)
	 * Consists of 14 bytes
	 */
	uint8_t unknown2[ 14 ];

	/* Flush counter
	 * Consists of 2 bytes
	 */
	uint8_t flush_counter[ 2 ];

	/* Access time
	 * Consists of 4 bytes
	 */
	uint8_t access_time[ 4 ];

	/* Access time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t access_time_nano_seconds[ 4 ];

	/* Modification time
	 * Consists of 4 bytes
	 */
	uint8_t modification_time[ 4 ];

	/* Modification time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t modification_time_nano_seconds[ 4 ];

	/* Inode change time
	 * Consists of 4 bytes
	 */
	uint8_t inode_change_time[ 4 ];

	/* Inode change time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t inode_change_time_nano_seconds[ 4 ];

	/* Data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* Number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];

	/* Extent size
	 * Consists of 4 bytes
	 */
	uint8_t extent_size[ 4 ];

	/* Number of data extents
	 * Consists of 4 bytes
	 */
	uint8_t number_of_data_extents[ 4 ];

	/* Number of attributes extents
	 * Consists of 2 bytes
	 */
	uint8_t number_of_attributes_extents[ 2 ];

	/* Attributes fork (descriptor) offset
	 * Consists of 1 byte
	 */
	uint8_t attributes_fork_offset;

	/* Attributes fork type
	 * Consists of 1 byte
	 */
	uint8_t attributes_fork_type;

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown6[ 2 ];

	/* Inode flags
	 * Consists of 2 bytes
	 */
	uint8_t inode_flags[ 2 ];

	/* Generation number
	 * Consists of 4 bytes
	 */
	uint8_t generation_number[ 4 ];

/* TODO verify if this value exists */
	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown7[ 4 ];
};

typedef struct fsxfs_inode_v2 fsxfs_inode_v2_t;

struct fsxfs_inode_v2
{
	/* Signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* File mode
	 * Consists of 2 bytes
	 */
	uint8_t file_mode[ 2 ];

	/* Format version
	 * Consists of 1 byte
	 */
	uint8_t format_version;

	/* Fork type
	 * Consists of 1 byte
	 */
	uint8_t fork_type;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Owner identifier
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* Group identifier
	 * Consists of 4 bytes
	 */
	uint8_t group_identifier[ 4 ];

	/* Number of links
	 * Consists of 4 bytes
	 */
	uint8_t number_of_links[ 4 ];

	/* Project identifier
	 * Consists of 2 bytes
	 */
	uint8_t project_identifier[ 2 ];

	/* Unknown (padding)
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Flush counter
	 * Consists of 2 bytes
	 */
	uint8_t flush_counter[ 2 ];

	/* Access time
	 * Consists of 4 bytes
	 */
	uint8_t access_time[ 4 ];

	/* Access time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t access_time_nano_seconds[ 4 ];

	/* Modification time
	 * Consists of 4 bytes
	 */
	uint8_t modification_time[ 4 ];

	/* Modification time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t modification_time_nano_seconds[ 4 ];

	/* Inode change time
	 * Consists of 4 bytes
	 */
	uint8_t inode_change_time[ 4 ];

	/* Inode change time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t inode_change_time_nano_seconds[ 4 ];

	/* Data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* Number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];

	/* Extent size
	 * Consists of 4 bytes
	 */
	uint8_t extent_size[ 4 ];

	/* Number of data extents
	 * Consists of 4 bytes
	 */
	uint8_t number_of_data_extents[ 4 ];

	/* Number of attributes extents
	 * Consists of 2 bytes
	 */
	uint8_t number_of_attributes_extents[ 2 ];

	/* Attributes fork (descriptor) offset
	 * Consists of 1 byte
	 */
	uint8_t attributes_fork_offset;

	/* Attributes fork type
	 * Consists of 1 byte
	 */
	uint8_t attributes_fork_type;

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown6[ 2 ];

	/* Inode flags
	 * Consists of 2 bytes
	 */
	uint8_t inode_flags[ 2 ];

	/* Generation number
	 * Consists of 4 bytes
	 */
	uint8_t generation_number[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown7[ 4 ];
};

typedef struct fsxfs_inode_v3 fsxfs_inode_v3_t;

struct fsxfs_inode_v3
{
	/* Signature
	 * Consists of 2 bytes
	 */
	uint8_t signature[ 2 ];

	/* File mode
	 * Consists of 2 bytes
	 */
	uint8_t file_mode[ 2 ];

	/* Format version
	 * Consists of 1 byte
	 */
	uint8_t format_version;

	/* Fork type
	 * Consists of 1 byte
	 */
	uint8_t fork_type;

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* Owner identifier
	 * Consists of 4 bytes
	 */
	uint8_t owner_identifier[ 4 ];

	/* Group identifier
	 * Consists of 4 bytes
	 */
	uint8_t group_identifier[ 4 ];

	/* Number of links
	 * Consists of 4 bytes
	 */
	uint8_t number_of_links[ 4 ];

	/* Project identifier
	 * Consists of 2 bytes
	 */
	uint8_t project_identifier[ 2 ];

	/* Unknown (padding)
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Flush counter
	 * Consists of 2 bytes
	 */
	uint8_t flush_counter[ 2 ];

	/* Access time
	 * Consists of 4 bytes
	 */
	uint8_t access_time[ 4 ];

	/* Access time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t access_time_nano_seconds[ 4 ];

	/* Modification time
	 * Consists of 4 bytes
	 */
	uint8_t modification_time[ 4 ];

	/* Modification time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t modification_time_nano_seconds[ 4 ];

	/* Inode change time
	 * Consists of 4 bytes
	 */
	uint8_t inode_change_time[ 4 ];

	/* Inode change time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t inode_change_time_nano_seconds[ 4 ];

	/* Data size
	 * Consists of 8 bytes
	 */
	uint8_t data_size[ 8 ];

	/* Number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];

	/* Extent size
	 * Consists of 4 bytes
	 */
	uint8_t extent_size[ 4 ];

	/* Number of data extents
	 * Consists of 4 bytes
	 */
	uint8_t number_of_data_extents[ 4 ];

	/* Number of attributes extents
	 * Consists of 2 bytes
	 */
	uint8_t number_of_attributes_extents[ 2 ];

	/* Attributes fork (descriptor) offset
	 * Consists of 1 byte
	 */
	uint8_t attributes_fork_offset;

	/* Attributes fork type
	 * Consists of 1 byte
	 */
	uint8_t attributes_fork_type;

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown6[ 2 ];

	/* Inode flags
	 * Consists of 2 bytes
	 */
	uint8_t inode_flags[ 2 ];

	/* Generation number
	 * Consists of 4 bytes
	 */
	uint8_t generation_number[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown7[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Change count
	 * Consists of 8 bytes
	 */
	uint8_t change_count[ 8 ];

	/* Log sequence number
	 * Consists of 8 bytes
	 */
	uint8_t log_sequence_number[ 8 ];

	/* Extended inode flags
	 * Consists of 8 bytes
	 */
	uint8_t extended_inode_flags[ 8 ];

	/* Copy-on-write (COW) extent size
	 * Consists of 4 bytes
	 */
	uint8_t cow_extent_size[ 4 ];

	/* Unknown (padding)
	 * Consists of 12 bytes
	 */
	uint8_t unknown8[ 12 ];

	/* Creation time
	 * Consists of 4 bytes
	 */
	uint8_t creation_time[ 4 ];

	/* Creation time nano seconds
	 * Consists of 4 bytes
	 */
	uint8_t creation_time_nano_seconds[ 4 ];

	/* Inode number
	 * Consists of 8 bytes
	 */
	uint8_t inode_number[ 8 ];

	/* Inode type identifier
	 * Consists of 16 bytes
	 */
	uint8_t inode_type_identifier[ 16 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_INODE_H ) */

