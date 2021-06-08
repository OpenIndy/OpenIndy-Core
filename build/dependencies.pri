
include($$PWD/../lib/OpenIndy-Math/include.pri)

linux: LIBS = -lGLU

CONFIG(release, debug|release) {
    dlltarget.path = $$PWD/../install/release
} else {
    dlltarget.path = $$PWD/../install/debug
}
INSTALLS += dlltarget
