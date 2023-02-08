/*
 * Superblock functions
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsxfs_debug.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfguid.h"
#include "libfsxfs_libuna.h"
#include "libfsxfs_superblock.h"

#include "fsxfs_superblock.h"

const char *fsxfs_superblock_signature = "XFSB";

/* Creates a superblock
 * Make sure the value superblock is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_superblock_initialize(
     libfsxfs_superblock_t **superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_superblock_initialize";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( *superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid superblock value already set.",
		 function );

		return( -1 );
	}
	*superblock = memory_allocate_structure(
	               libfsxfs_superblock_t );

	if( *superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create superblock.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *superblock,
	     0,
	     sizeof( libfsxfs_superblock_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear superblock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *superblock != NULL )
	{
		memory_free(
		 *superblock );

		*superblock = NULL;
	}
	return( -1 );
}

/* Frees a superblock
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_superblock_free(
     libfsxfs_superblock_t **superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_superblock_free";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( *superblock != NULL )
	{
		memory_free(
		 *superblock );

		*superblock = NULL;
	}
	return( 1 );
}

/* Reads the superblock data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_superblock_read_data(
     libfsxfs_superblock_t *superblock,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function               = "libfsxfs_superblock_read_data";
	uint32_t supported_feature_flags    = 0;
	uint16_t number_of_inodes_per_block = 0;
	uint16_t version_and_feature_flags  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                = 0;
	uint32_t value_32bit                = 0;
	uint16_t value_16bit                = 0;
#endif

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsxfs_superblock_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: superblock data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsxfs_superblock_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_compare(
	     ( (fsxfs_superblock_t *) data )->signature,
	     fsxfs_superblock_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_superblock_t *) data )->block_size,
	 superblock->block_size );

	byte_stream_copy_to_uint64_big_endian(
	 ( (fsxfs_superblock_t *) data )->number_of_blocks,
	 superblock->number_of_blocks );

	byte_stream_copy_to_uint64_big_endian(
	 ( (fsxfs_superblock_t *) data )->journal_block_number,
	 superblock->journal_block_number );

	byte_stream_copy_to_uint64_big_endian(
	 ( (fsxfs_superblock_t *) data )->root_directory_inode_number,
	 superblock->root_directory_inode_number );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_superblock_t *) data )->allocation_group_size,
	 superblock->allocation_group_size );

	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_superblock_t *) data )->number_of_allocation_groups,
	 superblock->number_of_allocation_groups );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_superblock_t *) data )->version_and_feature_flags,
	 version_and_feature_flags );

	superblock->format_version = (uint8_t) ( version_and_feature_flags & 0x000f );
	superblock->feature_flags  = version_and_feature_flags & 0xfff0;

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_superblock_t *) data )->sector_size,
	 superblock->sector_size );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_superblock_t *) data )->inode_size,
	 superblock->inode_size );

	byte_stream_copy_to_uint16_big_endian(
	 ( (fsxfs_superblock_t *) data )->number_of_inodes_per_block,
	 number_of_inodes_per_block );

	if( memory_copy(
	     superblock->volume_label,
	     ( (fsxfs_superblock_t *) data )->volume_label,
	     12 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume label.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_big_endian(
	 ( (fsxfs_superblock_t *) data )->secondary_feature_flags,
	 superblock->secondary_feature_flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->signature[ 0 ],
		 ( (fsxfs_superblock_t *) data )->signature[ 1 ],
		 ( (fsxfs_superblock_t *) data )->signature[ 2 ],
		 ( (fsxfs_superblock_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: block size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 superblock->block_size );

		libcnotify_printf(
		 "%s: number of blocks\t\t\t\t: %" PRIu64 "\n",
		 function,
		 superblock->number_of_blocks );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->number_of_realtime_blocks,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of real-time blocks\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->number_of_realtime_extents,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of real-time extents\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		if( libfsxfs_debug_print_guid_value(
		     function,
		     "file system identifier\t\t\t",
		     ( (fsxfs_superblock_t *) data )->file_system_identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print GUID value.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "%s: journal block number\t\t\t: %" PRIu64 "\n",
		 function,
		 superblock->journal_block_number );

		libcnotify_printf(
		 "%s: root directory inode number\t\t: %" PRIu64 "\n",
		 function,
		 superblock->root_directory_inode_number );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->realtime_bitmap_extents_inode_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: real-time bitmap extents inode number\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->realtime_bitmap_summary_inode_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: real-time bitmap summary inode number\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->realtime_extents_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: real-time extents size\t\t\t: %" PRIu32 " blocks\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: allocation group size\t\t\t: %" PRIu32 " blocks\n",
		 function,
		 superblock->allocation_group_size );

		libcnotify_printf(
		 "%s: number of allocation groups\t\t: %" PRIu32 "\n",
		 function,
		 superblock->number_of_allocation_groups );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->realtime_bitmap_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: real-time bitmap size\t\t\t: %" PRIu32 " blocks\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->journal_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: journal size\t\t\t\t: %" PRIu32 " blocks\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: version and feature flags\t\t: 0x%04" PRIx16 " (version: %" PRIu8 ", feature flags: 0x%04" PRIx16 ")\n",
		 function,
		 version_and_feature_flags,
		 superblock->format_version,
		 superblock->feature_flags );
		libfsxfs_debug_print_feature_flags(
		 superblock->feature_flags );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: sector size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 superblock->sector_size );

		libcnotify_printf(
		 "%s: inode size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 superblock->inode_size );

		libcnotify_printf(
		 "%s: number of inodes per block\t\t: %" PRIu16 "\n",
		 function,
		 number_of_inodes_per_block );

/* TODO print as string */
		libcnotify_printf(
		 "%s: volume label:\n",
		 function );
		libcnotify_print_data(
		 superblock->volume_label,
		 12,
		 0 );

		libcnotify_printf(
		 "%s: block size (log2)\t\t\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->block_size_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->block_size_log2 );

		libcnotify_printf(
		 "%s: sector size (log2)\t\t\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->sector_size_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->sector_size_log2 );

		libcnotify_printf(
		 "%s: inode size (log2)\t\t\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->inode_size_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->inode_size_log2 );

		libcnotify_printf(
		 "%s: number of inodes per block (log2)\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->number_of_inodes_per_block_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->number_of_inodes_per_block_log2 );

		libcnotify_printf(
		 "%s: allocation group size (log2)\t\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->allocation_group_size_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->allocation_group_size_log2 );

		libcnotify_printf(
		 "%s: number of real-time extents (log2)\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->number_of_realtime_extents_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->number_of_realtime_extents_log2 );

		libcnotify_printf(
		 "%s: creation flag\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->creation_flag );

		libcnotify_printf(
		 "%s: inodes percentage\t\t\t: %" PRIu8 " %%\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->inodes_percentage );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->number_of_inodes,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of inodes\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->number_of_free_inodes,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of free inodes\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->number_of_free_data_blocks,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of free data blocks\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->number_of_free_realtime_extents,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of free real-time extents\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->user_quota_inode_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: user quota extents inode number\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_big_endian(
		 ( (fsxfs_superblock_t *) data )->group_quota_inode_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: group quota extents inode number\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_superblock_t *) data )->quota_flags,
		 value_16bit );
		libcnotify_printf(
		 "%s: quota flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: miscellaneous flags\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->miscellaneous_flags );

		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->unknown1 );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->inode_chunk_alignment_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: inode chunk alignment size\t\t: %" PRIu32 " blocks\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->raid_unit_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: RAID unit size\t\t\t\t: %" PRIu32 " blocks\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->raid_width,
		 value_32bit );
		libcnotify_printf(
		 "%s: RAID width\t\t\t\t: %" PRIu32 " blocks\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: directory block size (log2)\t\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->directory_block_size_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->directory_block_size_log2 );

		libcnotify_printf(
		 "%s: journal device sector size (log2)\t: %" PRIu8 " (%" PRIu64 ")\n",
		 function,
		 ( (fsxfs_superblock_t *) data )->journal_device_sector_size_log2,
		 (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->journal_device_sector_size_log2 );

		byte_stream_copy_to_uint16_big_endian(
		 ( (fsxfs_superblock_t *) data )->journal_device_sector_size,
		 value_16bit );
		libcnotify_printf(
		 "%s: journal device sector size\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->journal_device_raid_unit_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: journal device RAID unit size\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: secondary feature flags\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 superblock->secondary_feature_flags );
		libfsxfs_debug_print_secondary_feature_flags(
		 superblock->secondary_feature_flags );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_big_endian(
		 ( (fsxfs_superblock_t *) data )->secondary_feature_flags_copy,
		 value_32bit );
		libcnotify_printf(
		 "%s: secondary feature flags (copy)\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
		libfsxfs_debug_print_secondary_feature_flags(
		 value_32bit );
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( superblock->format_version != 4 )
	 && ( superblock->format_version != 5 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu8 ".",
		 function,
		 superblock->format_version );

		return( -1 );
	}
	supported_feature_flags = 0x0010
	                        | 0x0020
	                        | 0x0080
	                        | 0x0400
	                        | 0x0800
	                        | 0x1000
	                        | 0x2000
	                        | 0x4000
	                        | 0x8000;

	if( ( (uint32_t) superblock->feature_flags & ~( supported_feature_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported features flags: 0x%04" PRIx16 ".",
		 function,
		 superblock->feature_flags );

		return( -1 );
	}
/* TODO check if block size a multitude of 2 */
	if( ( superblock->block_size < 512 )
	 || ( superblock->block_size > 65536 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block size: %" PRIu32 ".",
		 function,
		 superblock->block_size );

		return( -1 );
	}
	if( ( superblock->sector_size != 512 )
	 && ( superblock->sector_size != 1024 )
	 && ( superblock->sector_size != 2048 )
	 && ( superblock->sector_size != 4096 )
	 && ( superblock->sector_size != 8192 )
	 && ( superblock->sector_size != 16384 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported sector size: %" PRIu16 ".",
		 function,
		 superblock->sector_size );

		return( -1 );
	}
/* TODO check if inode size a multitude of 2 */
	if( ( superblock->inode_size < 256 )
	 || ( superblock->inode_size > 2048 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported inode size: %" PRIu16 ".",
		 function,
		 superblock->inode_size );

		return( -1 );
	}
	if( ( (fsxfs_superblock_t *) data )->directory_block_size_log2 == 0 )
	{
		superblock->directory_block_size = superblock->block_size;
	}
	else
	{
		if( ( (fsxfs_superblock_t *) data )->directory_block_size_log2 >= 32 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid directory block size log2 value out of bounds.",
			 function );

			return( -1 );
		}
		superblock->directory_block_size = (uint32_t) 1 << ( (fsxfs_superblock_t *) data )->directory_block_size_log2;

		if( (size_t) superblock->directory_block_size > (size_t) ( UINT32_MAX / superblock->block_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid directory block size log2 value out of bounds.",
			 function );

			return( -1 );
		}
		superblock->directory_block_size *= superblock->block_size;
	}
	if( ( superblock->allocation_group_size < 5 )
	 || ( superblock->allocation_group_size > (uint32_t) INT32_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid allocation group size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( (fsxfs_superblock_t *) data )->allocation_group_size_log2 == 0 )
	 || ( ( (fsxfs_superblock_t *) data )->allocation_group_size_log2 > 31 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid allocation group size log2 value out of bounds.",
		 function );

		return( -1 );
	}
	superblock->number_of_relative_block_number_bits = (uint8_t) ( (fsxfs_superblock_t *) data )->allocation_group_size_log2;

	if( ( ( (fsxfs_superblock_t *) data )->number_of_inodes_per_block_log2 == 0 )
	 || ( ( (fsxfs_superblock_t *) data )->number_of_inodes_per_block_log2 > ( 32 - superblock->number_of_relative_block_number_bits ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid number of inodes per block log2 value out of bounds.",
		 function );

		return( -1 );
	}
	superblock->number_of_relative_inode_number_bits = superblock->number_of_relative_block_number_bits + (uint8_t) ( (fsxfs_superblock_t *) data )->number_of_inodes_per_block_log2;

	if( ( superblock->number_of_relative_inode_number_bits == 0 )
	 || ( superblock->number_of_relative_inode_number_bits >= 32 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid number of relative inode number bits value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( ( (uint64_t) 1UL << ( (fsxfs_superblock_t *) data )->number_of_inodes_per_block_log2 ) != (uint64_t) number_of_inodes_per_block ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch between number of inodes per block and log2 values.",
		 function );

		return( -1 );
	}
/* TODO read version 5 additional values */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: directory block size\t\t\t: %" PRIu32 "\n",
		 function,
		 superblock->directory_block_size );

		libcnotify_printf(
		 "%s: number of relative block number bits\t: %" PRIu8 "\n",
		 function,
		 superblock->number_of_relative_block_number_bits );

		libcnotify_printf(
		 "%s: number of relative inode number bits\t: %" PRIu8 "\n",
		 function,
		 superblock->number_of_relative_inode_number_bits );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the superblock from a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_superblock_read_file_io_handle(
     libfsxfs_superblock_t *superblock,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t data[ 512 ];

	static char *function = "libfsxfs_superblock_read_file_io_handle";
	ssize_t read_count    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading superblock at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              data,
	              512,
	              file_offset,
	              error );

	if( read_count != (ssize_t) 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfsxfs_superblock_read_data(
	     superblock,
	     data,
	     512,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded volume label
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_superblock_get_utf8_volume_label_size(
     libfsxfs_superblock_t *superblock,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_superblock_get_utf8_volume_label_size";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_utf8_stream(
	     superblock->volume_label,
	     12,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded volume label
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_superblock_get_utf8_volume_label(
     libfsxfs_superblock_t *superblock,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_superblock_get_utf8_volume_label";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     superblock->volume_label,
	     12,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded volume label
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_superblock_get_utf16_volume_label_size(
     libfsxfs_superblock_t *superblock,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_superblock_get_utf16_volume_label_size";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_utf8_stream(
	     superblock->volume_label,
	     12,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded volume label
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_superblock_get_utf16_volume_label(
     libfsxfs_superblock_t *superblock,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_superblock_get_utf16_volume_label";

	if( superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     superblock->volume_label,
	     12,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

