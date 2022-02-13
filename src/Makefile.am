AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = config-manager

config_manager_SOURCES = \
	config.cpp \
	env.cpp \
	main.cpp \
	parser.cpp \
	render.cpp \
	settings.cpp

noinst_HEADERS = \
	config.h \
	env.h \
	env_reflector.h \
	parser.h \
	render.h \
	settings.h

config_manager_CPPFLAGS = \
	$(CODE_COVERAGE_CPPFLAGS) \
	-I$(top_srcdir)/ext/CLI11/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/json/rapid/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/expected-lite/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/optional-lite/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/string-view-lite/include \
	-I$(top_srcdir)/ext/Jinja2Cpp/thirdparty/nonstd/variant-lite/include \
	-I$(top_srcdir)/ext/fmt/include \
	-I$(top_srcdir)/ext/subprocess/src/cpp

config_manager_LDADD = \
	$(CODE_CODERAGE_LIBS) -lpthread \
	$(top_builddir)/ext/libjinja2cpp.la \
	$(top_builddir)/ext/libsubprocess.la \
	$(top_builddir)/ext/libfmt.la

config_manager_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)

config_manager_LDFLAGS = -Wl,--version-script=$(srcdir)/version.script

EXTRA_DIST = version.script

clean-local:
	rm -rf *.gcda *.gcno

check-syntax:
	$(CXXCOMPILE) $(config_manager_CPPFLAGS) -Wall -fsyntax-only $(CHK_SOURCES)
