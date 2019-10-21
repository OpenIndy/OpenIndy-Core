CONFIG += c++11

TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS = \
    math \
    core

# project locations
core.subdir = $$PWD/build
math.subdir = $$PWD/lib/OpenIndy-Math

# project dependencies
core.depends = math
