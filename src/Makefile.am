AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = config-manager

config_manager_SOURCES = \
	config.cpp \
	main.cpp \
	parser.cpp \
	render.cpp \
	settings.cpp
config_manager_includedir = $(includedir)/config-manager
config_manager_include_HEADERS = \
	config.h \
	parser.h \
	render.h \
	settings.h

config_manager_CPPFLAGS = \
	-I$(top_srcdir)/ext/CLI11/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/json/rapid/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/expected-lite/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/optional-lite/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/string-view-lite/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/variant-lite/include \
	-I$(top_srcdir)/ext/fmt/include

config_manager_LDADD = $(top_builddir)/ext/libfmt.la $(top_builddir)/ext/libjinja2cpp.la

check-syntax:
	$(CXXCOMPILE) $(config_manager_CPPFLAGS) -Wall -fsyntax-only $(CHK_SOURCES)
