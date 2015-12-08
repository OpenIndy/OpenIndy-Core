TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    core \
    math

# project locations
core.subdir = $$PWD
math.subdir = $$PWD/lib/OpenIndy-Math

#internal dependencies
core.depends = math
