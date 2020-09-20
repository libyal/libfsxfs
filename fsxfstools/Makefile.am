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
	@LIBFSXFS_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	fsxfsinfo

fsxfsinfo_SOURCES = \
	fsxfsinfo.c \
	fsxfstools_getopt.c fsxfstools_getopt.h \
	fsxfstools_i18n.h \
	fsxfstools_libbfio.h \
	fsxfstools_libcerror.h \
	fsxfstools_libclocale.h \
	fsxfstools_libcnotify.h \
	fsxfstools_libfcache.h \
	fsxfstools_libfdata.h \
	fsxfstools_libfsxfs.h \
	fsxfstools_libuna.h \
	fsxfstools_output.c fsxfstools_output.h \
	fsxfstools_signal.c fsxfstools_signal.h \
	fsxfstools_unused.h \
	info_handle.c info_handle.h

fsxfsinfo_LDADD = \
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

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on fsxfsinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(fsxfsinfo_SOURCES)

