AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
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
	@LIBHMAC_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBFUSE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSXFS_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fsxfsinfo \
	fsxfsmount

fsxfsinfo_SOURCES = \
	digest_hash.c digest_hash.h \
	fsxfsinfo.c \
	fsxfstools_getopt.c fsxfstools_getopt.h \
	fsxfstools_i18n.h \
	fsxfstools_libbfio.h \
	fsxfstools_libcerror.h \
	fsxfstools_libclocale.h \
	fsxfstools_libcnotify.h \
	fsxfstools_libfcache.h \
	fsxfstools_libfdata.h \
	fsxfstools_libfdatetime.h \
	fsxfstools_libfsxfs.h \
	fsxfstools_libhmac.h \
	fsxfstools_libuna.h \
	fsxfstools_output.c fsxfstools_output.h \
	fsxfstools_signal.c fsxfstools_signal.h \
	fsxfstools_unused.h \
	info_handle.c info_handle.h

fsxfsinfo_LDADD = \
	@LIBHMAC_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@LIBDL_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

fsxfsmount_SOURCES = \
	fsxfsmount.c \
	fsxfstools_getopt.c fsxfstools_getopt.h \
	fsxfstools_i18n.h \
	fsxfstools_libbfio.h \
	fsxfstools_libcerror.h \
	fsxfstools_libclocale.h \
	fsxfstools_libcnotify.h \
	fsxfstools_libcpath.h \
	fsxfstools_libfsxfs.h \
	fsxfstools_libuna.h \
	fsxfstools_output.c fsxfstools_output.h \
	fsxfstools_signal.c fsxfstools_signal.h \
	fsxfstools_unused.h \
	mount_dokan.c mount_dokan.h \
	mount_file_entry.c mount_file_entry.h \
	mount_file_system.c mount_file_system.h \
	mount_fuse.c mount_fuse.h \
	mount_handle.c mount_handle.h

fsxfsmount_LDADD = \
	@LIBFUSE_LIBADD@ \
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
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f Makefile

splint-local:
	@echo "Running splint on fsxfsinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fsxfsinfo_SOURCES)

