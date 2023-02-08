/*
 * B+ tree structures of the X File System (XFS)
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

#if !defined( _FSXFS_BTREE_H )
#define _FSXFS_BTREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_btree_header_v1_32bit fsxfs_btree_header_v1_32bit_t;

struct fsxfs_btree_header_v1_32bit
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Level
	 * Consists of 2 bytes
	 */
	uint8_t level[ 2 ];

	/* Number of records
	 * Consists of 2 bytes
	 */
	uint8_t number_of_records[ 2 ];

	/* Previous B+ tree block number
	 * Consists of 4 bytes
	 */
	uint8_t previous_btree_block_number[ 4 ];

	/* Next B+ tree block number
	 * Consists of 4 bytes
	 */
	uint8_t next_btree_block_number[ 4 ];
};

typedef struct fsxfs_btree_header_v1_64bit fsxfs_btree_header_v1_64bit_t;

struct fsxfs_btree_header_v1_64bit
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Level
	 * Consists of 2 bytes
	 */
	uint8_t level[ 2 ];

	/* Number of records
	 * Consists of 2 bytes
	 */
	uint8_t number_of_records[ 2 ];

	/* Previous B+ tree block number
	 * Consists of 8 bytes
	 */
	uint8_t previous_btree_block_number[ 8 ];

	/* Next B+ tree block number
	 * Consists of 8 bytes
	 */
	uint8_t next_btree_block_number[ 8 ];
};

typedef struct fsxfs_btree_header_v5_32bit fsxfs_btree_header_v5_32bit_t;

struct fsxfs_btree_header_v5_32bit
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Level
	 * Consists of 2 bytes
	 */
	uint8_t level[ 2 ];

	/* Number of records
	 * Consists of 2 bytes
	 */
	uint8_t number_of_records[ 2 ];

	/* Previous B+ tree block number
	 * Consists of 4 bytes
	 */
	uint8_t previous_btree_block_number[ 4 ];

	/* Next B+ tree block number
	 * Consists of 4 bytes
	 */
	uint8_t next_btree_block_number[ 4 ];

	/* Block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* Log sequence number
	 * Consists of 8 bytes
	 */
	uint8_t log_sequence_number[ 8 ];

	/* Block type identifier
	 * Consists of 16 bytes
	 */
	uint8_t block_type_identifier[ 16 ];

	/* Owner allocation group
	 * Consists of 4 bytes
	 */
	uint8_t owner_allocation_group[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];
};

typedef struct fsxfs_btree_header_v5_64bit fsxfs_btree_header_v5_64bit_t;

struct fsxfs_btree_header_v5_64bit
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Level
	 * Consists of 2 bytes
	 */
	uint8_t level[ 2 ];

	/* Number of records
	 * Consists of 2 bytes
	 */
	uint8_t number_of_records[ 2 ];

	/* Previous B+ tree block number
	 * Consists of 8 bytes
	 */
	uint8_t previous_btree_block_number[ 8 ];

	/* Next B+ tree block number
	 * Consists of 8 bytes
	 */
	uint8_t next_btree_block_number[ 8 ];

	/* Block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* Log sequence number
	 * Consists of 8 bytes
	 */
	uint8_t log_sequence_number[ 8 ];

	/* Block type identifier
	 * Consists of 16 bytes
	 */
	uint8_t block_type_identifier[ 16 ];

	/* Owner allocation group
	 * Consists of 8 bytes
	 */
	uint8_t owner_allocation_group[ 8 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Unknown (padding)
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

typedef struct fsxfs_inode_btree_record fsxfs_inode_btree_record_t;

struct fsxfs_inode_btree_record
{
	/* Inode number
	 * Consists of 4 bytes
	 */
	uint8_t inode_number[ 4 ];

	/* Number of unused inodes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_unused_inodes[ 4 ];

	/* Inode chunk allocation bitmap
	 * Consists of 8 bytes
	 */
	uint8_t chunk_allocation_bitmap[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_BTREE_H ) */

