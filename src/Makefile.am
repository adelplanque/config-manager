AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = config-manager

config_manager_SOURCES = \
	settings.cpp \
	parser.cpp \
	main.cpp
config_manager_includedir = $(includedir)/config-manager
config_manager_include_HEADERS = \
	settings.h \
	parser.h

config_manager_CPPFLAGS = -I$(top_srcdir)/ext/fmt/include -I$(top_srcdir)/ext/CLI11/include
config_manager_LDADD = $(top_builddir)/ext/libfmt.la

check-syntax:
	$(CXXCOMPILE) $(config_manager_CPPFLAGS) -Wall -fsyntax-only $(CHK_SOURCES)
