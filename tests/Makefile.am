TESTS = \
	test_doc.yaml \
	test_env.yaml \
	test_errors.yaml \
	test_get.yaml \
	test_get2.yaml \
	test_recursive.yaml \
	test_render.yaml \
	test_shell.yaml \
	test_strings.yaml

TEST_EXTENSIONS = .yaml
YAML_LOG_COMPILER = env CONFIG_MANAGER=$(top_builddir)/src/config-manager ${PYTHON} run-test.py

EXTRA_DIST = $(TESTS) run-test.py
