/*
 * Superblock structures of the X File System (XFS)
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

#if !defined( _FSXFS_SUPERBLOCK_H )
#define _FSXFS_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_superblock fsxfs_superblock_t;

struct fsxfs_superblock
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Block size
	 * Consists of 4 bytes
	 */
	uint8_t block_size[ 4 ];

	/* Number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];

	/* Number of real-time (device) blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_realtime_blocks[ 8 ];

	/* Number of real-time (device) extents
	 * Consists of 8 bytes
	 */
	uint8_t number_of_realtime_extents[ 8 ];

	/* File system identifier
	 * Consists of 16 bytes
	 */
	uint8_t file_system_identifier[ 16 ];

	/* Journal block number
	 * Consists of 8 bytes
	 */
	uint8_t journal_block_number[ 8 ];

	/* Root directory inode number
	 * Consists of 8 bytes
	 */
	uint8_t root_directory_inode_number[ 8 ];

	/* Real-time bitmap extents inode number
	 * Consists of 8 bytes
	 */
	uint8_t realtime_bitmap_extents_inode_number[ 8 ];

	/* Real-time bitmap summary inode number
	 * Consists of 8 bytes
	 */
	uint8_t realtime_bitmap_summary_inode_number[ 8 ];

	/* Real-time extents size
	 * Consists of 4 bytes
	 */
	uint8_t realtime_extents_size[ 4 ];

	/* Allocation group size
	 * Consists of 4 bytes
	 */
	uint8_t allocation_group_size[ 4 ];

	/* Number of allocation groups
	 * Consists of 4 bytes
	 */
	uint8_t number_of_allocation_groups[ 4 ];

	/* Real-time bitmap size
	 * Consists of 4 bytes
	 */
	uint8_t realtime_bitmap_size[ 4 ];

	/* Journal size
	 * Consists of 4 bytes
	 */
	uint8_t journal_size[ 4 ];

	/* Version and feature flags
	 * Consists of 2 bytes
	 */
	uint8_t version_and_feature_flags[ 2 ];

	/* Sector size
	 * Consists of 2 bytes
	 */
	uint8_t sector_size[ 2 ];

	/* Inode size
	 * Consists of 2 bytes
	 */
	uint8_t inode_size[ 2 ];

	/* Number of inodes per block
	 * Consists of 2 bytes
	 */
	uint8_t number_of_inodes_per_block[ 2 ];

	/* Volume label
	 * Consists of 12 bytes
	 */
	uint8_t volume_label[ 12 ];

	/* Block size in log2
	 * Consists of 1 byte
	 */
	uint8_t block_size_log2;

	/* Sector size in log2
	 * Consists of 1 byte
	 */
	uint8_t sector_size_log2;

	/* Inode size in log2
	 * Consists of 1 byte
	 */
	uint8_t inode_size_log2;

	/* Number of inodes per block in log2
	 * Consists of 1 byte
	 */
	uint8_t number_of_inodes_per_block_log2;

	/* Allocation group size in log2
	 * Consists of 1 byte
	 */
	uint8_t allocation_group_size_log2;

	/* Number of real-time (device) extents in log2
	 * Consist of 1 byte
	 */
	uint8_t number_of_realtime_extents_log2;

	/* Creation flag
	 * Consist of 1 byte
	 */
	uint8_t creation_flag;

	/* Inodes percentage
	 * Consist of 1 byte
	 */
	uint8_t inodes_percentage;

	/* Number of inodes
	 * Consists of 8 bytes
	 */
	uint8_t number_of_inodes[ 8 ];

	/* Number of free inodes
	 * Consists of 8 bytes
	 */
	uint8_t number_of_free_inodes[ 8 ];

	/* Number of free data blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_free_data_blocks[ 8 ];

	/* Number of free real-time extents
	 * Consists of 8 bytes
	 */
	uint8_t number_of_free_realtime_extents[ 8 ];

	/* User quota inode number
	 * Consists of 8 bytes
	 */
	uint8_t user_quota_inode_number[ 8 ];

	/* Group quota inode number
	 * Consists of 8 bytes
	 */
	uint8_t group_quota_inode_number[ 8 ];

	/* Quota flags
	 * Consists of 2 bytes
	 */
	uint8_t quota_flags[ 2 ];

	/* Miscellaneous flags
	 * Consists of 1 byte
	 */
	uint8_t miscellaneous_flags;

	/* Unknown (reserved)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* Inode chunk alignment size
	 * Consists of 4 bytes
	 */
	uint8_t inode_chunk_alignment_size[ 4 ];

	/* RAID unit size
	 * Consists of 4 bytes
	 */
	uint8_t raid_unit_size[ 4 ];

	/* RAID width
	 * Consists of 4 bytes
	 */
	uint8_t raid_width[ 4 ];

	/* Directory block size in log2
	 * Consist of 1 byte
	 */
	uint8_t directory_block_size_log2;

	/* Journal device sector size in log2
	 * Consist of 1 byte
	 */
	uint8_t journal_device_sector_size_log2;

	/* Journal device sector size
	 * Consist of 2 bytes
	 */
	uint8_t journal_device_sector_size[ 2 ];

	/* Journal device RAID unit size
	 * Consist of 4 bytes
	 */
	uint8_t journal_device_raid_unit_size[ 4 ];

	/* Secondary feature flags
	 * Consists of 4 bytes
	 */
	uint8_t secondary_feature_flags[ 4 ];

	/* Copy of secondary feature flags
	 * Consists of 4 bytes
	 */
	uint8_t secondary_feature_flags_copy[ 4 ];
};

typedef struct fsxfs_superblock_v5 fsxfs_superblock_v5_t;

struct fsxfs_superblock_v5
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Block size
	 * Consists of 4 bytes
	 */
	uint8_t block_size[ 4 ];

	/* Number of blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_blocks[ 8 ];

	/* Number of real-time (device) blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_realtime_blocks[ 8 ];

	/* Number of real-time (device) extents
	 * Consists of 8 bytes
	 */
	uint8_t number_of_realtime_extents[ 8 ];

	/* File system identifier
	 * Consists of 16 bytes
	 */
	uint8_t file_system_identifier[ 16 ];

	/* Journal block number
	 * Consists of 8 bytes
	 */
	uint8_t journal_block_number[ 8 ];

	/* Root directory inode number
	 * Consists of 8 bytes
	 */
	uint8_t root_directory_inode_number[ 8 ];

	/* Real-time bitmap extents inode number
	 * Consists of 8 bytes
	 */
	uint8_t realtime_bitmap_extents_inode_number[ 8 ];

	/* Real-time bitmap summary inode number
	 * Consists of 8 bytes
	 */
	uint8_t realtime_bitmap_summary_inode_number[ 8 ];

	/* Real-time extents size
	 * Consists of 4 bytes
	 */
	uint8_t realtime_extents_size[ 4 ];

	/* Allocation group size
	 * Consists of 4 bytes
	 */
	uint8_t allocation_group_size[ 4 ];

	/* Number of allocation groups
	 * Consists of 4 bytes
	 */
	uint8_t number_of_allocation_groups[ 4 ];

	/* Real-time bitmap size
	 * Consists of 4 bytes
	 */
	uint8_t realtime_bitmap_size[ 4 ];

	/* Journal size
	 * Consists of 4 bytes
	 */
	uint8_t journal_size[ 4 ];

	/* Version and feature flags
	 * Consists of 2 bytes
	 */
	uint8_t version_and_feature_flags[ 2 ];

	/* Sector size
	 * Consists of 2 bytes
	 */
	uint8_t sector_size[ 2 ];

	/* Inode size
	 * Consists of 2 bytes
	 */
	uint8_t inode_size[ 2 ];

	/* Number of inodes per block
	 * Consists of 2 bytes
	 */
	uint8_t number_of_inodes_per_block[ 2 ];

	/* Volume label
	 * Consists of 12 bytes
	 */
	uint8_t volume_label[ 12 ];

	/* Block size in log2
	 * Consists of 1 byte
	 */
	uint8_t block_size_log2;

	/* Sector size in log2
	 * Consists of 1 byte
	 */
	uint8_t sector_size_log2;

	/* Inode size in log2
	 * Consists of 1 byte
	 */
	uint8_t inode_size_log2;

	/* Number of inodes per block in log2
	 * Consists of 1 byte
	 */
	uint8_t number_of_inodes_per_block_log2;

	/* Allocation group size in log2
	 * Consists of 1 byte
	 */
	uint8_t allocation_group_size_log2;

	/* Number of real-time (device) extents in log2
	 * Consist of 1 byte
	 */
	uint8_t number_of_realtime_extents_log2;

	/* Creation flag
	 * Consist of 1 byte
	 */
	uint8_t creation_flag;

	/* Inodes percentage
	 * Consist of 1 byte
	 */
	uint8_t inodes_percentage;

	/* Number of inodes
	 * Consists of 8 bytes
	 */
	uint8_t number_of_inodes[ 8 ];

	/* Number of free inodes
	 * Consists of 8 bytes
	 */
	uint8_t number_of_free_inodes[ 8 ];

	/* Number of free data blocks
	 * Consists of 8 bytes
	 */
	uint8_t number_of_free_data_blocks[ 8 ];

	/* Number of free real-time extents
	 * Consists of 8 bytes
	 */
	uint8_t number_of_free_realtime_extents[ 8 ];

	/* User quota inode number
	 * Consists of 8 bytes
	 */
	uint8_t user_quota_inode_number[ 8 ];

	/* Group quota inode number
	 * Consists of 8 bytes
	 */
	uint8_t group_quota_inode_number[ 8 ];

	/* Quota flags
	 * Consists of 2 bytes
	 */
	uint8_t quota_flags[ 2 ];

	/* Miscellaneous flags
	 * Consists of 1 byte
	 */
	uint8_t miscellaneous_flags;

	/* Unknown (reserved)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;

	/* Inode chunk alignment size
	 * Consists of 4 bytes
	 */
	uint8_t inode_chunk_alignment_size[ 4 ];

	/* RAID unit size
	 * Consists of 4 bytes
	 */
	uint8_t raid_unit_size[ 4 ];

	/* RAID width
	 * Consists of 4 bytes
	 */
	uint8_t raid_width[ 4 ];

	/* Directory block size in log2
	 * Consist of 1 byte
	 */
	uint8_t directory_block_size_log2;

	/* Journal device sector size in log2
	 * Consist of 1 byte
	 */
	uint8_t journal_device_sector_size_log2;

	/* Journal device sector size
	 * Consist of 2 bytes
	 */
	uint8_t journal_device_sector_size[ 2 ];

	/* Journal device RAID unit size
	 * Consist of 4 bytes
	 */
	uint8_t journal_device_raid_unit_size[ 4 ];

	/* Secondary feature flags
	 * Consists of 4 bytes
	 */
	uint8_t secondary_feature_flags[ 4 ];

	/* Copy of secondary feature flags
	 * Consists of 4 bytes
	 */
	uint8_t secondary_feature_flags_copy[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_SUPERBLOCK_H ) */

