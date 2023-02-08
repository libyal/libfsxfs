/*
 * Inode information structures of the X File System (XFS)
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

#if !defined( _FSXFS_INODE_INFORMATION_H )
#define _FSXFS_INODE_INFORMATION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_inode_information fsxfs_inode_information_t;

struct fsxfs_inode_information
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Format version
	 * Consists of 4 bytes
	 */
	uint8_t format_version[ 4 ];

	/* Sequence number
	 * Consists of 4 bytes
	 */
	uint8_t sequence_number[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Number of inodes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_inodes[ 4 ];

	/* Inode B+ tree root block number
	 * Consists of 4 bytes
	 */
	uint8_t inode_btree_root_block_number[ 4 ];

	/* Inode B+ tree depth
	 * Consists of 4 bytes
	 */
	uint8_t inode_btree_depth[ 4 ];

	/* Number of unused inodes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_unused_inodes[ 4 ];

	/* Last allocated chunk
	 * Consists of 4 bytes
	 */
	uint8_t last_allocated_chunk[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* Hash table of unlinked inodes
	 * Consists of 64 x 4 bytes
	 */
	uint8_t unlinked_hash_table[ 64 * 4 ];
};

typedef struct fsxfs_inode_information_v5 fsxfs_inode_information_v5_t;

struct fsxfs_inode_information_v5
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Format version
	 * Consists of 4 bytes
	 */
	uint8_t format_version[ 4 ];

	/* Sequence number
	 * Consists of 4 bytes
	 */
	uint8_t sequence_number[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Number of inodes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_inodes[ 4 ];

	/* Inode B+ tree root block number
	 * Consists of 4 bytes
	 */
	uint8_t inode_btree_root_block_number[ 4 ];

	/* Inode B+ tree depth
	 * Consists of 4 bytes
	 */
	uint8_t inode_btree_depth[ 4 ];

	/* Number of unused inodes
	 * Consists of 4 bytes
	 */
	uint8_t number_of_unused_inodes[ 4 ];

	/* Last allocated chunk
	 * Consists of 4 bytes
	 */
	uint8_t last_allocated_chunk[ 4 ];

	/* Unknown (reserved)
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* Hash table of unlinked inodes
	 * Consists of 64 x 4 bytes
	 */
	uint8_t unlinked_hash_table[ 64 * 4 ];

	/* Block type identifier
	 * Consists of 16 bytes
	 */
	uint8_t block_type_identifier[ 16 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Unknown (padding)
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* Log sequence number
	 * Consists of 8 bytes
	 */
	uint8_t log_sequence_number[ 8 ];

	/* Free inode B+ tree root block number
	 * Consists of 4 bytes
	 */
	uint8_t free_inode_btree_root_block_number[ 4 ];

	/* Free inode B+ tree depth
	 * Consists of 4 bytes
	 */
	uint8_t free_inode_btree_depth[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_INODE_INFORMATION_H ) */

