
TEMPLATE = subdirs

SUBDIRS = \
    math \
    core

# project locations
core.subdir = $$PWD/build
math.subdir = $$PWD/lib/OpenIndy-Math

# project dependencies
core.depends = math
