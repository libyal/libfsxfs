AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSXFS_DLL_EXPORT@

lib_LTLIBRARIES = libfsxfs.la

libfsxfs_la_SOURCES = \
	fsxfs_attributes.h \
	fsxfs_attributes_block.h \
	fsxfs_block_directory.h \
	fsxfs_btree.h \
	fsxfs_file_system_block.h \
	fsxfs_inode.h \
	fsxfs_inode_information.h \
	fsxfs_superblock.h \
	libfsxfs.c \
	libfsxfs_attributes.c libfsxfs_attributes.h \
	libfsxfs_attributes_leaf_block_header.c libfsxfs_attributes_leaf_block_header.h \
	libfsxfs_attributes_table.c libfsxfs_attributes_table.h \
	libfsxfs_attributes_table_header.c libfsxfs_attributes_table_header.h \
	libfsxfs_attribute_values.c libfsxfs_attribute_values.h \
	libfsxfs_block_data_handle.c libfsxfs_block_data_handle.h \
	libfsxfs_block_directory.c libfsxfs_block_directory.h \
	libfsxfs_block_directory_footer.c libfsxfs_block_directory_footer.h \
	libfsxfs_block_directory_header.c libfsxfs_block_directory_header.h \
	libfsxfs_btree_block.c libfsxfs_btree_block.h \
	libfsxfs_btree_header.c libfsxfs_btree_header.h \
	libfsxfs_buffer_data_handle.c libfsxfs_buffer_data_handle.h \
	libfsxfs_data_stream.c libfsxfs_data_stream.h \
	libfsxfs_debug.c libfsxfs_debug.h \
	libfsxfs_definitions.h \
	libfsxfs_directory.c libfsxfs_directory.h \
	libfsxfs_directory_entry.c libfsxfs_directory_entry.h \
	libfsxfs_directory_table.c libfsxfs_directory_table.h \
	libfsxfs_directory_table_header.c libfsxfs_directory_table_header.h \
	libfsxfs_error.c libfsxfs_error.h \
	libfsxfs_extended_attribute.c libfsxfs_extended_attribute.h \
	libfsxfs_extent.c libfsxfs_extent.h \
	libfsxfs_extent_btree.c libfsxfs_extent_btree.h \
	libfsxfs_extent_list.c libfsxfs_extent_list.h \
	libfsxfs_extents.c libfsxfs_extents.h \
	libfsxfs_extern.h \
	libfsxfs_file_entry.c libfsxfs_file_entry.h \
	libfsxfs_file_system.c libfsxfs_file_system.h \
	libfsxfs_file_system_block.c libfsxfs_file_system_block.h \
	libfsxfs_file_system_block_header.c libfsxfs_file_system_block_header.h \
	libfsxfs_inode.c libfsxfs_inode.h \
	libfsxfs_inode_btree.c libfsxfs_inode_btree.h \
	libfsxfs_inode_btree_record.c libfsxfs_inode_btree_record.h \
	libfsxfs_inode_information.c libfsxfs_inode_information.h \
	libfsxfs_io_handle.c libfsxfs_io_handle.h \
	libfsxfs_libbfio.h \
	libfsxfs_libcdata.h \
	libfsxfs_libcerror.h \
	libfsxfs_libclocale.h \
	libfsxfs_libcnotify.h \
	libfsxfs_libcthreads.h \
	libfsxfs_libfcache.h \
	libfsxfs_libfdata.h \
	libfsxfs_libfdatetime.h \
	libfsxfs_libfguid.h \
	libfsxfs_libuna.h \
	libfsxfs_notify.c libfsxfs_notify.h \
	libfsxfs_superblock.c libfsxfs_superblock.h \
	libfsxfs_support.c libfsxfs_support.h \
	libfsxfs_types.h \
	libfsxfs_unused.h \
	libfsxfs_volume.c libfsxfs_volume.h

libfsxfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@PTHREAD_LIBADD@

libfsxfs_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfsxfs_definitions.h.in \
	libfsxfs.rc \
	libfsxfs.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libfsxfs_definitions.h
	-rm -f libfsxfs.rc
	-rm -f Makefile

sources-local: $(BUILT_SOURCES)

splint-local:
	@echo "Running splint on libfsxfs ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfsxfs_la_SOURCES)

