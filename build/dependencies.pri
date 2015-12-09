
# include paths
INCLUDEPATH += $$PWD/../lib/OpenIndy-Math/include
win32:CONFIG(release, debug|release): INCLUDEPATH += $$PWD/../lib/OpenIndy-Math/bin/release
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$PWD/../lib/OpenIndy-Math/bin/debug
else:unix: CONFIG(debug, debug|release): INCLUDEPATH += $$PWD/../lib/OpenIndy-Math/bin/debug
else:unix: CONFIG(release, debug|release): INCLUDEPATH += $$PWD/../lib/OpenIndy-Math/bin/release

# depend paths
win32:CONFIG(release, debug|release): DEPENDPATH += $$PWD/../lib/OpenIndy-Math/bin/release
else:win32:CONFIG(debug, debug|release): DEPENDPATH += $$PWD/../lib/OpenIndy-Math/bin/debug
else:unix: CONFIG(debug, debug|release): DEPENDPATH += $$PWD/../lib/OpenIndy-Math/bin/debug
else:unix: CONFIG(release, debug|release): DEPENDPATH += $$PWD/../lib/OpenIndy-Math/bin/release

# header files
HEADERS  += \
    $$PWD/../lib/OpenIndy-Math/include/chooselalib.h \
    $$PWD/../lib/OpenIndy-Math/include/global.h \
    $$PWD/../lib/OpenIndy-Math/include/linearalgebra.h \
    $$PWD/../lib/OpenIndy-Math/include/oimat.h \
    $$PWD/../lib/OpenIndy-Math/include/oivec.h

# libraries
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/OpenIndy-Math/bin/release/ -lopenIndyMath1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/OpenIndy-Math/bin/debug/ -lopenIndyMath1
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/OpenIndy-Math/bin/debug -lopenIndyMath1
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/OpenIndy-Math/bin/release -lopenIndyMath1

linux: LIBS = -lGLU
