CONFIG += c++11

TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS = \
    math \
    core \
    test

# project locations
core.subdir = $$PWD/build
math.subdir = $$PWD/lib/OpenIndy-Math
test.subdir = test

# project dependencies
core.depends = math
test.depends = core

QMAKE_EXTRA_TARGETS += run-test
run-test.commands = \
    cd test && $(MAKE) run-test
