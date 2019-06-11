
# include paths
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/geometry
INCLUDEPATH += $$PWD/include/util
INCLUDEPATH += $$PWD/include/plugin
INCLUDEPATH += $$PWD/include/plugin/exchange
INCLUDEPATH += $$PWD/include/plugin/function
INCLUDEPATH += $$PWD/include/plugin/networkAdjustment
INCLUDEPATH += $$PWD/include/plugin/sensor
INCLUDEPATH += $$PWD/include/plugin/simulation
INCLUDEPATH += $$PWD/include/plugin/tool
INCLUDEPATH += $$PWD/lib/OpenIndy-Math/include

# TODO OI-482 check config:
win32:CONFIG(release, debug|release): INCLUDEPATH += -L$$PWD/bin/release
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += -L$$PWD/bin/debug
else:linux:CONFIG(debug, debug|release): INCLUDEPATH += -L$$PWD/bin/debug
else:linux:CONFIG(release, debug|release): INCLUDEPATH += -L$$PWD/bin/release
win32:CONFIG(release, debug|release): INCLUDEPATH += -L$$PWD/lib/OpenIndy-Math/bin/release
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += -L$$PWD/lib/OpenIndy-Math/bin/debug
else:linux:CONFIG(debug, debug|release): INCLUDEPATH += -L$$PWD/lib/OpenIndy-Math/bin/debug
else:linux:CONFIG(release, debug|release): INCLUDEPATH += -L$$PWD/lib/OpenIndy-Math/bin/release


# depend paths
win32:CONFIG(release, debug|release): DEPENDPATH += -L$$PWD/bin/release
else:win32:CONFIG(debug, debug|release): DEPENDPATH += -L$$PWD/bin/debug
else:linux: CONFIG(debug, debug|release): DEPENDPATH += -L$$PWD/bin/debug
else:linux: CONFIG(release, debug|release): DEPENDPATH += -L$$PWD/bin/release
win32:CONFIG(release, debug|release): DEPENDPATH += -L$$PWD/lib/OpenIndy-Math/bin/release
else:win32:CONFIG(debug, debug|release): DEPENDPATH += -L$$PWD/lib/OpenIndy-Math/bin/debug
else:linux: CONFIG(debug, debug|release): DEPENDPATH += -L$$PWD/lib/OpenIndy-Math/bin/debug
else:linux: CONFIG(release, debug|release): DEPENDPATH += -L$$PWD/lib/OpenIndy-Math/bin/release

# header files
HEADERS  += \
    $$PWD/lib/OpenIndy-Math/include/chooselalib.h \
    $$PWD/lib/OpenIndy-Math/include/global.h \
    $$PWD/lib/OpenIndy-Math/include/linearalgebra.h \
    $$PWD/lib/OpenIndy-Math/include/oimat.h \
    $$PWD/lib/OpenIndy-Math/include/oivec.h

# libraries
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/bin/release/ -lopenIndyCore1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/bin/debug/ -lopenIndyCore1
else:linux:CONFIG(release, debug|release): LIBS += -L$$PWD/bin/release/ -lopenIndyCore
else:linux:CONFIG(debug, debug|release): LIBS += -L$$PWD/bin/debug/ -lopenIndyCore
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/release/ -lopenIndyMath1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/debug/ -lopenIndyMath1
else:linux:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/release/ -lopenIndyMath
else:linux:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/OpenIndy-Math/bin/debug/ -lopenIndyMath

