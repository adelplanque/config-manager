AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = config-manager

config_manager_SOURCES = \
	parser.cpp \
	main.cpp

check-syntax:
	$(CXXCOMPILE) $(config_manager_CPPFLAGS) -Wall -fsyntax-only $(CHK_SOURCES)
