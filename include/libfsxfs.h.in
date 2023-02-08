/*
 * Library to access the X File System (XFS) format
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

#if !defined( _LIBFSXFS_H )
#define _LIBFSXFS_H

#include <libfsxfs/codepage.h>
#include <libfsxfs/definitions.h>
#include <libfsxfs/error.h>
#include <libfsxfs/extern.h>
#include <libfsxfs/features.h>
#include <libfsxfs/types.h>

#include <stdio.h>

#if defined( LIBFSXFS_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBFSXFS_EXTERN \
const char *libfsxfs_get_version(
             void );

/* Returns the access flags for reading
 */
LIBFSXFS_EXTERN \
int libfsxfs_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_get_codepage(
     int *codepage,
     libfsxfs_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_set_codepage(
     int codepage,
     libfsxfs_error_t **error );

/* Determines if a file contains an ext volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_check_volume_signature(
     const char *filename,
     libfsxfs_error_t **error );

#if defined( LIBFSXFS_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains an ext volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_check_volume_signature_wide(
     const wchar_t *filename,
     libfsxfs_error_t **error );

#endif /* defined( LIBFSXFS_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFSXFS_HAVE_BFIO )

/* Determines if a file contains an ext volume signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libfsxfs_error_t **error );

#endif /* defined( LIBFSXFS_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBFSXFS_EXTERN \
void libfsxfs_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_notify_set_stream(
     FILE *stream,
     libfsxfs_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_notify_stream_open(
     const char *filename,
     libfsxfs_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_notify_stream_close(
     libfsxfs_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBFSXFS_EXTERN \
void libfsxfs_error_free(
      libfsxfs_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_error_fprint(
     libfsxfs_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_error_sprint(
     libfsxfs_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_error_backtrace_fprint(
     libfsxfs_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_error_backtrace_sprint(
     libfsxfs_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Volume functions
 * ------------------------------------------------------------------------- */

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_initialize(
     libfsxfs_volume_t **volume,
     libfsxfs_error_t **error );

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_free(
     libfsxfs_volume_t **volume,
     libfsxfs_error_t **error );

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_signal_abort(
     libfsxfs_volume_t *volume,
     libfsxfs_error_t **error );

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_open(
     libfsxfs_volume_t *volume,
     const char *filename,
     int access_flags,
     libfsxfs_error_t **error );

#if defined( LIBFSXFS_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_open_wide(
     libfsxfs_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libfsxfs_error_t **error );

#endif /* defined( LIBFSXFS_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFSXFS_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_open_file_io_handle(
     libfsxfs_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfsxfs_error_t **error );

#endif /* defined( LIBFSXFS_HAVE_BFIO ) */

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_close(
     libfsxfs_volume_t *volume,
     libfsxfs_error_t **error );

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_format_version(
     libfsxfs_volume_t *volume,
     uint8_t *format_version,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded label
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf8_label_size(
     libfsxfs_volume_t *volume,
     size_t *utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-8 encoded label
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf8_label(
     libfsxfs_volume_t *volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded label
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf16_label_size(
     libfsxfs_volume_t *volume,
     size_t *utf16_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-16 encoded label
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_utf16_label(
     libfsxfs_volume_t *volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsxfs_error_t **error );

/* Retrieves the root directory file entry
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_root_directory(
     libfsxfs_volume_t *volume,
     libfsxfs_file_entry_t **file_entry,
     libfsxfs_error_t **error );

/* Retrieves the file entry of a specific inode number
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_file_entry_by_inode(
     libfsxfs_volume_t *volume,
     uint64_t inode_number,
     libfsxfs_file_entry_t **file_entry,
     libfsxfs_error_t **error );

/* Retrieves the file entry for an UTF-8 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_file_entry_by_utf8_path(
     libfsxfs_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_file_entry_t **file_entry,
     libfsxfs_error_t **error );

/* Retrieves the file entry for an UTF-16 encoded path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_volume_get_file_entry_by_utf16_path(
     libfsxfs_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_file_entry_t **file_entry,
     libfsxfs_error_t **error );

/* -------------------------------------------------------------------------
 * File entry functions
 * ------------------------------------------------------------------------- */

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_free(
     libfsxfs_file_entry_t **file_entry,
     libfsxfs_error_t **error );

/* Retrieves the inode number
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_inode_number(
     libfsxfs_file_entry_t *file_entry,
     uint64_t *inode_number,
     libfsxfs_error_t **error );

/* Retrieves the creation date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_creation_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsxfs_error_t **error );

/* Retrieves the modification date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_modification_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsxfs_error_t **error );

/* Retrieves the access date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_access_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsxfs_error_t **error );

/* Retrieves the inode change date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_inode_change_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libfsxfs_error_t **error );

/* Retrieves the file mode
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_file_mode(
     libfsxfs_file_entry_t *file_entry,
     uint16_t *file_mode,
     libfsxfs_error_t **error );

/* Retrieves the number of (hard) links
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_number_of_links(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *number_of_links,
     libfsxfs_error_t **error );

/* Retrieves the owner (user) identifier
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_owner_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libfsxfs_error_t **error );

/* Retrieves the group identifier
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_group_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libfsxfs_error_t **error );

/* Retrieves the device identifier
 * This value is retrieved from the inode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_device_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *device_identifier,
     libfsxfs_error_t **error );

/* Retrieves the device number
 * This value is retrieved from the inode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_device_number(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *major_device_number,
     uint32_t *minor_device_number,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf8_name_size(
     libfsxfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf16_name_size(
     libfsxfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf8_symbolic_link_target_size(
     libfsxfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf8_symbolic_link_target(
     libfsxfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf16_symbolic_link_target_size(
     libfsxfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-16 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_utf16_symbolic_link_target(
     libfsxfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsxfs_error_t **error );

/* Retrieves the number of extended attributes
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_number_of_extended_attributes(
     libfsxfs_file_entry_t *file_entry,
     int *number_of_extended_attributes,
     libfsxfs_error_t **error );

/* Retrieves the extended attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_extended_attribute_by_index(
     libfsxfs_file_entry_t *file_entry,
     int extended_attribute_index,
     libfsxfs_extended_attribute_t **extended_attribute,
     libfsxfs_error_t **error );

/* Determines if there is an extended attribute for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_has_extended_attribute_by_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_error_t **error );

/* Determines if there is an extended attribute for an UTF-16 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_has_extended_attribute_by_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_error_t **error );

/* Retrieves the extended attribute for an UTF-8 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_extended_attribute_by_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_extended_attribute_t **extended_attribute,
     libfsxfs_error_t **error );

/* Retrieves the extended attribute for an UTF-16 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_extended_attribute_by_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_extended_attribute_t **extended_attribute,
     libfsxfs_error_t **error );

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_number_of_sub_file_entries(
     libfsxfs_file_entry_t *file_entry,
     int *number_of_sub_entries,
     libfsxfs_error_t **error );

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_sub_file_entry_by_index(
     libfsxfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsxfs_file_entry_t **sub_file_entry,
     libfsxfs_error_t **error );

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_file_entry_t **sub_file_entry,
     libfsxfs_error_t **error );

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if the file entry does not contain such value or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_file_entry_t **sub_file_entry,
     libfsxfs_error_t **error );

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSXFS_EXTERN \
ssize_t libfsxfs_file_entry_read_buffer(
         libfsxfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libfsxfs_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSXFS_EXTERN \
ssize_t libfsxfs_file_entry_read_buffer_at_offset(
         libfsxfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfsxfs_error_t **error );

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
LIBFSXFS_EXTERN \
off64_t libfsxfs_file_entry_seek_offset(
         libfsxfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libfsxfs_error_t **error );

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_offset(
     libfsxfs_file_entry_t *file_entry,
     off64_t *offset,
     libfsxfs_error_t **error );

/* Retrieves the size of the data
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_size(
     libfsxfs_file_entry_t *file_entry,
     size64_t *size,
     libfsxfs_error_t **error );

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_number_of_extents(
     libfsxfs_file_entry_t *file_entry,
     int *number_of_extents,
     libfsxfs_error_t **error );

/* Retrieves a specific extent
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_extent_by_index(
     libfsxfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libfsxfs_error_t **error );

/* -------------------------------------------------------------------------
 * Extended attribute functions
 * ------------------------------------------------------------------------- */

/* Frees an extended attribute
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_free(
     libfsxfs_extended_attribute_t **extended_attribute,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf8_name_size(
     libfsxfs_extended_attribute_t *extended_attribute,
     size_t *utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf8_name(
     libfsxfs_extended_attribute_t *extended_attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfsxfs_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf16_name_size(
     libfsxfs_extended_attribute_t *extended_attribute,
     size_t *utf16_string_size,
     libfsxfs_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_utf16_name(
     libfsxfs_extended_attribute_t *extended_attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfsxfs_error_t **error );

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBFSXFS_EXTERN \
ssize_t libfsxfs_extended_attribute_read_buffer(
         libfsxfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         libfsxfs_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFSXFS_EXTERN \
ssize_t libfsxfs_extended_attribute_read_buffer_at_offset(
         libfsxfs_extended_attribute_t *extended_attribute,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfsxfs_error_t **error );

/* Seeks a certain offset
 * Returns the offset if seek is successful or -1 on error
 */
LIBFSXFS_EXTERN \
off64_t libfsxfs_extended_attribute_seek_offset(
         libfsxfs_extended_attribute_t *extended_attribute,
         off64_t offset,
         int whence,
         libfsxfs_error_t **error );

/* Retrieves the current offset
 * Returns the offset if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_offset(
     libfsxfs_extended_attribute_t *extended_attribute,
     off64_t *offset,
     libfsxfs_error_t **error );

/* Retrieves the size of the data stream object
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_size(
     libfsxfs_extended_attribute_t *extended_attribute,
     size64_t *size,
     libfsxfs_error_t **error );

/* Retrieves the number of extents
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_number_of_extents(
     libfsxfs_extended_attribute_t *extended_attribute,
     int *number_of_extents,
     libfsxfs_error_t **error );

/* Retrieves a specific extent
 * Returns 1 if successful or -1 on error
 */
LIBFSXFS_EXTERN \
int libfsxfs_extended_attribute_get_extent_by_index(
     libfsxfs_extended_attribute_t *extended_attribute,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libfsxfs_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_H ) */

