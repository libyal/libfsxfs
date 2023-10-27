AUTOMAKE_OPTIONS = subdir-objects

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
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSXFS_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYFSXFS = \
	test_python_module.sh
endif

TESTS = \
	test_library.sh \
	test_tools.sh \
	test_fsxfsinfo.sh \
	$(TESTS_PYFSXFS)

check_SCRIPTS = \
	pyfsxfs_test_support.py \
	pyfsxfs_test_volume.py \
	test_fsxfsinfo.sh \
	test_library.sh \
	test_manpage.sh \
	test_python_module.sh \
	test_runner.sh \
	test_tools.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	fsxfs_test_attribute_values \
	fsxfs_test_attributes_leaf_block_header \
	fsxfs_test_attributes_table \
	fsxfs_test_attributes_table_header \
	fsxfs_test_block_directory_footer \
	fsxfs_test_block_directory_header \
	fsxfs_test_btree_block \
	fsxfs_test_btree_header \
	fsxfs_test_buffer_data_handle \
	fsxfs_test_directory_entry \
	fsxfs_test_directory_table_header \
	fsxfs_test_error \
	fsxfs_test_extent \
	fsxfs_test_file_system_block_header \
	fsxfs_test_inode_btree \
	fsxfs_test_inode_btree_record \
	fsxfs_test_inode_information \
	fsxfs_test_io_handle \
	fsxfs_test_notify \
	fsxfs_test_superblock \
	fsxfs_test_support \
	fsxfs_test_tools_info_handle \
	fsxfs_test_tools_output \
	fsxfs_test_tools_signal \
	fsxfs_test_volume

fsxfs_test_attribute_values_SOURCES = \
	fsxfs_test_attribute_values.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_attribute_values_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_attributes_leaf_block_header_SOURCES = \
	fsxfs_test_attributes_leaf_block_header.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_attributes_leaf_block_header_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_attributes_table_SOURCES = \
	fsxfs_test_attributes_table.c \
	fsxfs_test_libcdata.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_attributes_table_LDADD = \
	@LIBCDATA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_attributes_table_header_SOURCES = \
	fsxfs_test_attributes_table_header.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_attributes_table_header_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_block_directory_footer_SOURCES = \
	fsxfs_test_block_directory_footer.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_block_directory_footer_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_block_directory_header_SOURCES = \
	fsxfs_test_block_directory_header.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_block_directory_header_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_btree_block_SOURCES = \
	fsxfs_test_btree_block.c \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_btree_block_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_btree_header_SOURCES = \
	fsxfs_test_btree_header.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_btree_header_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_buffer_data_handle_SOURCES = \
	fsxfs_test_buffer_data_handle.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_buffer_data_handle_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_directory_entry_SOURCES = \
	fsxfs_test_directory_entry.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_directory_entry_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_directory_table_header_SOURCES = \
	fsxfs_test_directory_table_header.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_directory_table_header_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_error_SOURCES = \
	fsxfs_test_error.c \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_unused.h

fsxfs_test_error_LDADD = \
	../libfsxfs/libfsxfs.la

fsxfs_test_extent_SOURCES = \
	fsxfs_test_extent.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_extent_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_file_system_block_header_SOURCES = \
	fsxfs_test_file_system_block_header.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_file_system_block_header_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_inode_btree_SOURCES = \
	fsxfs_test_inode_btree.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_inode_btree_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_inode_btree_record_SOURCES = \
	fsxfs_test_inode_btree_record.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_inode_btree_record_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_inode_information_SOURCES = \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_inode_information.c \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_inode_information_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_io_handle_SOURCES = \
	fsxfs_test_io_handle.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_io_handle_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_notify_SOURCES = \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_notify.c \
	fsxfs_test_unused.h

fsxfs_test_notify_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_superblock_SOURCES = \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_superblock.c \
	fsxfs_test_unused.h

fsxfs_test_superblock_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_support_SOURCES = \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_getopt.c fsxfs_test_getopt.h \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libclocale.h \
	fsxfs_test_libcnotify.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_libuna.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_support.c \
	fsxfs_test_unused.h

fsxfs_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_tools_info_handle_SOURCES = \
	../fsxfstools/digest_hash.c ../fsxfstools/digest_hash.h \
	../fsxfstools/info_handle.c ../fsxfstools/info_handle.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_tools_info_handle.c \
	fsxfs_test_unused.h

fsxfs_test_tools_info_handle_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@

fsxfs_test_tools_output_SOURCES = \
	../fsxfstools/fsxfstools_output.c ../fsxfstools/fsxfstools_output.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_macros.h \
	fsxfs_test_tools_output.c \
	fsxfs_test_unused.h

fsxfs_test_tools_output_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_tools_signal_SOURCES = \
	../fsxfstools/fsxfstools_signal.c ../fsxfstools/fsxfstools_signal.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_macros.h \
	fsxfs_test_tools_signal.c \
	fsxfs_test_unused.h

fsxfs_test_tools_signal_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_volume_SOURCES = \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_getopt.c fsxfs_test_getopt.h \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libclocale.h \
	fsxfs_test_libcnotify.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_libuna.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_volume.c \
	fsxfs_test_unused.h

fsxfs_test_volume_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

