TEMPLATE = subdirs

#CONFIG += ordered

SUBDIRS = \
    core \
    math

# project locations
core.subdir = $$PWD/OpenIndy-Core
math.subdir = $$PWD/OpenIndy-Core/lib/OpenIndy-Math/OpenIndy-Math

#internal dependencies
core.depends = math
