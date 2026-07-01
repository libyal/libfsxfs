AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_SCRIPTS = \
	pyfsxfs_test_support.py \
	pyfsxfs_test_volume.py

check_PROGRAMS = \
	fsxfs_test_attribute_values \
	fsxfs_test_attributes \
	fsxfs_test_attributes_leaf_block_header \
	fsxfs_test_attributes_table \
	fsxfs_test_attributes_table_header \
	fsxfs_test_block_directory \
	fsxfs_test_block_directory_footer \
	fsxfs_test_block_directory_header \
	fsxfs_test_btree_block \
	fsxfs_test_btree_header \
	fsxfs_test_buffer_data_handle \
	fsxfs_test_directory_entry \
	fsxfs_test_directory_table \
	fsxfs_test_directory_table_header \
	fsxfs_test_error \
	fsxfs_test_extent \
	fsxfs_test_file_entry \
	fsxfs_test_file_system_block \
	fsxfs_test_file_system_block_header \
	fsxfs_test_inode_btree \
	fsxfs_test_inode_btree_record \
	fsxfs_test_inode_information \
	fsxfs_test_io_handle \
	fsxfs_test_notify \
	fsxfs_test_superblock \
	fsxfs_test_support \
	fsxfs_test_tools_info_handle \
	fsxfs_test_tools_mount_path_string \
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

fsxfs_test_attributes_SOURCES = \
	fsxfs_test_attributes.c \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcdata.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_attributes_LDADD = \
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

fsxfs_test_block_directory_SOURCES = \
	fsxfs_test_block_directory.c \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcdata.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_block_directory_LDADD = \
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

fsxfs_test_directory_table_SOURCES = \
	fsxfs_test_directory_table.c \
	fsxfs_test_libcdata.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_directory_table_LDADD = \
	@LIBCDATA_LIBADD@ \
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

fsxfs_test_file_entry_SOURCES = \
	fsxfs_test_file_entry.c \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_file_entry_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_file_system_block_SOURCES = \
	fsxfs_test_file_system_block.c \
	fsxfs_test_functions.c fsxfs_test_functions.h \
	fsxfs_test_libbfio.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libfsxfs.h \
	fsxfs_test_macros.h \
	fsxfs_test_memory.c fsxfs_test_memory.h \
	fsxfs_test_unused.h

fsxfs_test_file_system_block_LDADD = \
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

fsxfs_test_tools_mount_path_string_SOURCES = \
	../fsxfstools/mount_path_string.c ../fsxfstools/mount_path_string.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_libcpath.h \
	fsxfs_test_macros.h \
	fsxfs_test_tools_mount_path_string.c \
	fsxfs_test_unused.h

fsxfs_test_tools_mount_path_string_LDADD = \
	@LIBUNA_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@

fsxfs_test_tools_output_SOURCES = \
	../fsxfstools/fsxfstools_output.c ../fsxfstools/fsxfstools_output.h \
	fsxfs_test_libcerror.h \
	fsxfs_test_macros.h \
	fsxfs_test_tools_output.c \
	fsxfs_test_unused.h

fsxfs_test_tools_output_LDADD = \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

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

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libfsxfs.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libfsxfs

test_inputs_pyfsxfs.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyfsxfs

test_inputs_fsxfsinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) fsxfsinfo

test_inputs_fsxfsinfo_fs.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) fsxfsinfo_fs

test_library: \
	package.m4 \
	test_inputs_libfsxfs.at \
	test_library.at \
	test_macros.at

test_manpages: \
	package.m4 \
	test_manpages.at

test_python_module: \
	package.m4 \
	test_inputs_pyfsxfs.at \
	test_macros.at \
	test_python_module.at

test_tools: \
	package.m4 \
	test_inputs_fsxfsinfo.at \
	test_inputs_fsxfsinfo_fs.at \
	test_macros.at \
	test_tools.at

EXTRA_DIST = \
	$(check_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_library.at \
	test_macros.at \
	test_manpages.at \
	test_python_module.at \
	test_tools.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

